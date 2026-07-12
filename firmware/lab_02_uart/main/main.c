#include <stdint.h>

#include "driver/uart.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define UART_PORT       UART_NUM_2
#define UART_TX_PIN     17
#define UART_RX_PIN     16
#define UART_BAUD_RATE  9600
#define UART_RX_BUFFER  256

static const char *TAG = "UART_TEST";

static void uart_init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 0,
        .source_clk = UART_SCLK_DEFAULT,
    };

    ESP_ERROR_CHECK(uart_param_config(UART_PORT, &uart_config));

    ESP_ERROR_CHECK(uart_set_pin(
        UART_PORT,
        UART_TX_PIN,
        UART_RX_PIN,
        UART_PIN_NO_CHANGE,
        UART_PIN_NO_CHANGE
    ));

    ESP_ERROR_CHECK(uart_driver_install(
        UART_PORT,
        UART_RX_BUFFER,
        0,
        0,
        NULL,
        0
    ));
}

void app_main(void)
{
    /*
     * Não utilizar strlen() neste vetor.
     *
     * O primeiro byte é 0x00, que seria interpretado como
     * final de uma string convencional em C.
     */
    static const uint8_t test_bytes[] = {
        0x00,
        0xFF,
        0x55,
        0xAA,
        0x41
    };

    uart_init();

    ESP_LOGI(TAG, "UART2 configurada em 9600 baud, formato 8N1");
    ESP_LOGI(TAG, "TX = GPIO%d | RX = GPIO%d", UART_TX_PIN, UART_RX_PIN);

    ESP_LOGI(TAG, "Transmissao iniciara em 2 segundos");
    vTaskDelay(pdMS_TO_TICKS(2000));

    while (1) {
        ESP_LOGI(TAG, "Iniciando sequencia");

        for (size_t i = 0; i < sizeof(test_bytes); i++) {
            const int bytes_written = uart_write_bytes(
                UART_PORT,
                &test_bytes[i],
                1
            );

            if (bytes_written != 1) {
                ESP_LOGE(
                    TAG,
                    "Falha ao transmitir 0x%02X",
                    (unsigned int)test_bytes[i]
                );
                continue;
            }

            ESP_ERROR_CHECK(
                uart_wait_tx_done(UART_PORT, pdMS_TO_TICKS(100))
            );

            ESP_LOGI(
                TAG,
                "TX enviado: 0x%02X",
                (unsigned int)test_bytes[i]
            );

            /*
             * Intervalo proposital para separar visualmente
             * os quadros no analisador lógico.
             */
            vTaskDelay(pdMS_TO_TICKS(500));
        }

        ESP_LOGI(TAG, "Sequencia concluida");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}