#include <stddef.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "driver/uart.h"

#include "esp_err.h"
#include "esp_log.h"

/*
 * Configuração da UART utilizada no experimento.
 */
#define LOOPBACK_UART       UART_NUM_2
#define LOOPBACK_TX_GPIO    GPIO_NUM_17
#define LOOPBACK_RX_GPIO    GPIO_NUM_16

#define UART_BAUD_RATE      9600
#define UART_RX_BUFFER_SIZE 1024

/*
 * Tempos utilizados no experimento.
 *
 * BYTE_INTERVAL_MS:
 * intervalo entre cada byte da sequência.
 *
 * SEQUENCE_INTERVAL_MS:
 * intervalo entre o final de uma sequência e o início da próxima.
 */
#define RX_TIMEOUT_MS        500
#define TX_TIMEOUT_MS        100
#define BYTE_INTERVAL_MS     200
#define SEQUENCE_INTERVAL_MS 3000

static const char *TAG = "UART_LOOPBACK";

/*
 * Inicializa a UART2 com:
 *
 * TX: GPIO17
 * RX: GPIO16
 * Baud rate: 9600
 * Formato: 8N1
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
     * Instala o driver da UART.
     *
     * Buffer RX: 1024 bytes
     * Buffer TX: desabilitado
     * Fila de eventos: não utilizada nesta prática
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

    /*
     * Aplica baud rate, bits de dados, paridade e stop bit.
     */
    ESP_ERROR_CHECK(
        uart_param_config(
            LOOPBACK_UART,
            &uart_config
        )
    );

    /*
     * Define os GPIOs utilizados pela UART2.
     */
    ESP_ERROR_CHECK(
        uart_set_pin(
            LOOPBACK_UART,
            LOOPBACK_TX_GPIO,
            LOOPBACK_RX_GPIO,
            UART_PIN_NO_CHANGE,
            UART_PIN_NO_CHANGE
        )
    );

    /*
     * Remove qualquer dado antigo que possa estar no buffer RX.
     */
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

    ESP_LOGI(
        TAG,
        "Intervalo entre bytes: %d ms",
        BYTE_INTERVAL_MS
    );

    ESP_LOGI(
        TAG,
        "Intervalo entre sequencias: %d ms",
        SEQUENCE_INTERVAL_MS
    );
}

void app_main(void)
{
    /*
     * Bytes utilizados para comparar diferentes formas de onda.
     *
     * 0x00: todos os bits de dados em nível baixo
     * 0xFF: todos os bits de dados em nível alto
     * 0x55: padrão alternado
     * 0xAA: padrão alternado inverso
     * 0x41: caractere ASCII 'A'
     */
    static const uint8_t test_bytes[] = {
        0x00,
        0xFF,
        0x55,
        0xAA,
        0x41
    };

    const size_t test_count =
        sizeof(test_bytes) / sizeof(test_bytes[0]);

    uart_loopback_init();

    while (true) {
        ESP_LOGI(
            TAG,
            "========================================"
        );

        ESP_LOGI(
            TAG,
            "Iniciando nova sequencia de testes"
        );

        for (size_t i = 0; i < test_count; i++) {
            const uint8_t tx_byte = test_bytes[i];
            uint8_t rx_byte = 0x00;

            /*
             * Limpa o buffer antes de cada novo teste para evitar
             * que bytes antigos sejam confundidos com a resposta atual.
             */
            ESP_ERROR_CHECK(
                uart_flush_input(LOOPBACK_UART)
            );

            /*
             * Envia exatamente um byte pela UART2.
             */
            const int bytes_sent = uart_write_bytes(
                LOOPBACK_UART,
                &tx_byte,
                sizeof(tx_byte)
            );

            /*
             * Aguarda a transmissão sair completamente da UART.
             */
            const esp_err_t tx_status = uart_wait_tx_done(
                LOOPBACK_UART,
                pdMS_TO_TICKS(TX_TIMEOUT_MS)
            );

            /*
             * Aguarda o mesmo byte chegar ao RX pelo jumper físico.
             */
            const int bytes_received = uart_read_bytes(
                LOOPBACK_UART,
                &rx_byte,
                sizeof(rx_byte),
                pdMS_TO_TICKS(RX_TIMEOUT_MS)
            );

            /*
             * Validação da transmissão e da recepção.
             */
            if (bytes_sent != 1) {
                ESP_LOGE(
                    TAG,
                    "[%u] Falha na transmissao de 0x%02X: enviados=%d",
                    (unsigned int)i,
                    (unsigned int)tx_byte,
                    bytes_sent
                );
            } else if (tx_status != ESP_OK) {
                ESP_LOGE(
                    TAG,
                    "[%u] Timeout aguardando TX de 0x%02X",
                    (unsigned int)i,
                    (unsigned int)tx_byte
                );
            } else if (bytes_received == 0) {
                ESP_LOGE(
                    TAG,
                    "[%u] Nenhum dado recebido para TX=0x%02X",
                    (unsigned int)i,
                    (unsigned int)tx_byte
                );
            } else if (bytes_received < 0) {
                ESP_LOGE(
                    TAG,
                    "[%u] Erro na leitura da UART",
                    (unsigned int)i
                );
            } else if (rx_byte == tx_byte) {
                if (tx_byte == 0x41) {
                    ESP_LOGI(
                        TAG,
                        "[%u] TX=0x%02X ('A') | RX=0x%02X | OK",
                        (unsigned int)i,
                        (unsigned int)tx_byte,
                        (unsigned int)rx_byte
                    );
                } else {
                    ESP_LOGI(
                        TAG,
                        "[%u] TX=0x%02X | RX=0x%02X | OK",
                        (unsigned int)i,
                        (unsigned int)tx_byte,
                        (unsigned int)rx_byte
                    );
                }
            } else {
                ESP_LOGE(
                    TAG,
                    "[%u] TX=0x%02X | RX=0x%02X | FALHOU",
                    (unsigned int)i,
                    (unsigned int)tx_byte,
                    (unsigned int)rx_byte
                );
            }

            /*
             * Mantém os bytes próximos o suficiente para que toda
             * a sequência apareça na captura do analisador lógico.
             */
            vTaskDelay(
                pdMS_TO_TICKS(BYTE_INTERVAL_MS)
            );
        }

        ESP_LOGI(
            TAG,
            "Sequencia concluida: 00 FF 55 AA 41"
        );

        /*
         * Pausa maior para separar visualmente os grupos no Saleae.
         */
        vTaskDelay(
            pdMS_TO_TICKS(SEQUENCE_INTERVAL_MS)
        );
    }
}