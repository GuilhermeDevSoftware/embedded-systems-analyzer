#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"

#define LOOPBACK_UART       UART_NUM_2
#define LOOPBACK_TX_GPIO    GPIO_NUM_17
#define LOOPBACK_RX_GPIO    GPIO_NUM_16

#define UART_BAUD_RATE      9600
#define UART_RX_BUFFER_SIZE 1024

#define RX_TIMEOUT_MS       500
#define TEST_INTERVAL_MS    1000

static const char *TAG = "UART_LOOPBACK";

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
     * Buffer RX habilitado.
     * Buffer TX igual a zero: a transmissão utiliza diretamente
     * a FIFO de hardware.
     *
     * Nesta primeira prática não utilizaremos fila de eventos.
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
        uart_param_config(LOOPBACK_UART, &uart_config)
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

    ESP_ERROR_CHECK(uart_flush_input(LOOPBACK_UART));

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
    const uint8_t tx_byte = 0x55;

    uart_loopback_init();

    while (true) {
        uint8_t rx_byte = 0x00;

        /*
         * Remove dados antigos que possam ter permanecido no
         * buffer de recepção.
         */
        ESP_ERROR_CHECK(uart_flush_input(LOOPBACK_UART));

        const int bytes_sent = uart_write_bytes(
            LOOPBACK_UART,
            &tx_byte,
            sizeof(tx_byte)
        );

        const esp_err_t tx_status = uart_wait_tx_done(
            LOOPBACK_UART,
            pdMS_TO_TICKS(100)
        );

        const int bytes_received = uart_read_bytes(
            LOOPBACK_UART,
            &rx_byte,
            sizeof(rx_byte),
            pdMS_TO_TICKS(RX_TIMEOUT_MS)
        );

        if (bytes_sent != sizeof(tx_byte)) {
            ESP_LOGE(
                TAG,
                "Falha na transmissão: enviados=%d",
                bytes_sent
            );
        } else if (tx_status != ESP_OK) {
            ESP_LOGE(
                TAG,
                "Timeout aguardando final da transmissão"
            );
        } else if (bytes_received == 0) {
            ESP_LOGE(
                TAG,
                "Nenhum byte recebido: verifique o jumper GPIO17-GPIO16"
            );
        } else if (bytes_received < 0) {
            ESP_LOGE(
                TAG,
                "Erro ao ler a UART2"
            );
        } else if (rx_byte == tx_byte) {
            ESP_LOGI(
                TAG,
                "TX=0x%02X | RX=0x%02X | LOOPBACK OK",
                (unsigned int)tx_byte,
                (unsigned int)rx_byte
            );
        } else {
            ESP_LOGE(
                TAG,
                "TX=0x%02X | RX=0x%02X | LOOPBACK FALHOU",
                (unsigned int)tx_byte,
                (unsigned int)rx_byte
            );
        }

        vTaskDelay(pdMS_TO_TICKS(TEST_INTERVAL_MS));
    }
}