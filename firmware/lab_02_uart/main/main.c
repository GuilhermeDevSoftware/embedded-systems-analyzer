#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "driver/uart.h"

#include "esp_err.h"
#include "esp_log.h"

/*
 * UART utilizada no experimento.
 */
#define LOOPBACK_UART       UART_NUM_2
#define LOOPBACK_TX_GPIO    GPIO_NUM_17
#define LOOPBACK_RX_GPIO    GPIO_NUM_16

/*
 * Configuração da comunicação.
 */
#define UART_BAUD_RATE      9600
#define UART_RX_BUFFER_SIZE 1024

/*
 * Tempos da prática.
 */
#define TX_TIMEOUT_MS        100
#define RX_TIMEOUT_MS        500
#define MESSAGE_INTERVAL_MS 3000

static const char *TAG = "UART_MESSAGE";

/*
 * Inicializa a UART2 em:
 *
 * 9600 baud
 * 8 bits de dados
 * sem paridade
 * 1 stop bit
 * sem controle de fluxo
 */
static void uart_loopback_init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    /*
     * Instala o driver:
     *
     * RX buffer: 1024 bytes
     * TX buffer: desabilitado
     * fila de eventos: não utilizada
     */
    ESP_ERROR_CHECK(
        uart_driver_install(
            LOOPBACK_UART,
            UART_RX_BUFFER_SIZE,
            0,
            0,
            NULL,
            0
        )
    );

    ESP_ERROR_CHECK(
        uart_param_config(
            LOOPBACK_UART,
            &uart_config
        )
    );

    ESP_ERROR_CHECK(
        uart_set_pin(
            LOOPBACK_UART,
            LOOPBACK_TX_GPIO,
            LOOPBACK_RX_GPIO,
            UART_PIN_NO_CHANGE,
            UART_PIN_NO_CHANGE
        )
    );

    ESP_ERROR_CHECK(
        uart_flush_input(LOOPBACK_UART)
    );

    ESP_LOGI(
        TAG,
        "UART2 configurada: TX=GPIO%d RX=GPIO%d, %d baud, 8N1",
        LOOPBACK_TX_GPIO,
        LOOPBACK_RX_GPIO,
        UART_BAUD_RATE
    );
}

void app_main(void)
{
    /*
     * O compilador armazena:
     *
     * 'U' 'A' 'R' 'T' '\0'
     *
     * Portanto:
     *
     * sizeof(tx_message) = 5
     * strlen(tx_message) = 4
     */
    static const char tx_message[] = "UART";

    const size_t tx_length = strlen(tx_message);

    /*
     * O buffer possui cinco posições para acomodar:
     *
     * U A R T \0
     */
    uint8_t rx_buffer[sizeof(tx_message)];

    uart_loopback_init();

    ESP_LOGI(
        TAG,
        "sizeof(tx_message)=%u bytes",
        (unsigned int)sizeof(tx_message)
    );

    ESP_LOGI(
        TAG,
        "strlen(tx_message)=%u bytes",
        (unsigned int)tx_length
    );

    ESP_LOGI(
        TAG,
        "Mensagem em hexadecimal: 55 41 52 54"
    );

    while (true) {
        /*
         * Limpa o buffer de software.
         */
        memset(
            rx_buffer,
            0,
            sizeof(rx_buffer)
        );

        /*
         * Remove possíveis dados antigos do buffer RX da UART.
         */
        ESP_ERROR_CHECK(
            uart_flush_input(LOOPBACK_UART)
        );

        ESP_LOGI(
            TAG,
            "----------------------------------------"
        );

        /*
         * Transmite quatro bytes em uma única chamada.
         *
         * O terminador '\0' não é transmitido porque tx_length
         * foi obtido com strlen().
         */
        const int bytes_sent = uart_write_bytes(
            LOOPBACK_UART,
            tx_message,
            tx_length
        );

        /*
         * Aguarda todos os bytes saírem fisicamente pelo TX.
         */
        const esp_err_t tx_status = uart_wait_tx_done(
            LOOPBACK_UART,
            pdMS_TO_TICKS(TX_TIMEOUT_MS)
        );

        /*
         * Solicita a leitura de até quatro bytes.
         */
        const int bytes_received = uart_read_bytes(
            LOOPBACK_UART,
            rx_buffer,
            tx_length,
            pdMS_TO_TICKS(RX_TIMEOUT_MS)
        );

        if (bytes_sent != (int)tx_length) {
            ESP_LOGE(
                TAG,
                "Falha no TX: esperados=%u enviados=%d",
                (unsigned int)tx_length,
                bytes_sent
            );
        } else if (tx_status != ESP_OK) {
            ESP_LOGE(
                TAG,
                "Timeout aguardando o final da transmissao"
            );
        } else if (bytes_received < 0) {
            ESP_LOGE(
                TAG,
                "Erro durante a leitura da UART2"
            );
        } else if (bytes_received == 0) {
            ESP_LOGE(
                TAG,
                "Nenhum dado recebido: verifique GPIO17-GPIO16"
            );
        } else {
            /*
             * uart_read_bytes() não adiciona '\0'.
             *
             * Como o buffer possui cinco posições e a mensagem
             * possui quatro bytes, adicionamos o terminador
             * manualmente.
             */
            rx_buffer[bytes_received] = '\0';

            ESP_LOGI(
                TAG,
                "TX [%d bytes]: %s",
                bytes_sent,
                tx_message
            );

            ESP_LOGI(
                TAG,
                "RX [%d bytes]: %s",
                bytes_received,
                (char *)rx_buffer
            );

            ESP_LOGI(
                TAG,
                "TX HEX: 55 41 52 54"
            );

            ESP_LOG_BUFFER_HEX(
                TAG,
                rx_buffer,
                bytes_received
            );

            if (bytes_received != (int)tx_length) {
                ESP_LOGW(
                    TAG,
                    "RECEPCAO PARCIAL: esperados=%u recebidos=%d",
                    (unsigned int)tx_length,
                    bytes_received
                );
            } else if (
                memcmp(
                    tx_message,
                    rx_buffer,
                    tx_length
                ) == 0
            ) {
                ESP_LOGI(
                    TAG,
                    "COMPARACAO: LOOPBACK OK"
                );
            } else {
                ESP_LOGE(
                    TAG,
                    "COMPARACAO: LOOPBACK FALHOU"
                );
            }
        }

        /*
         * Separa visualmente cada mensagem no analisador lógico.
         */
        vTaskDelay(
            pdMS_TO_TICKS(MESSAGE_INTERVAL_MS)
        );
    }
}