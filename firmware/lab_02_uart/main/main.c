#include <stdint.h>

#include "driver/gpio.h"
#include "driver/uart.h"

#include "esp_err.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LAB_UART_PORT       UART_NUM_2
#define LAB_UART_TX_GPIO    GPIO_NUM_17
#define LAB_UART_RX_GPIO    GPIO_NUM_16
#define LAB_UART_BAUD_RATE  9600
#define LAB_UART_RX_BUFFER  256

static const char *TAG = "UART_LAB";

static void uart_lab_init(void)
{
    const uart_config_t uart_config = {
        .baud_rate = LAB_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    ESP_ERROR_CHECK(
        uart_driver_install(
            LAB_UART_PORT,
            LAB_UART_RX_BUFFER,
            0,
            0,
            NULL,
            0
        )
    );

    ESP_ERROR_CHECK(
        uart_param_config(
            LAB_UART_PORT,
            &uart_config
        )
    );

    ESP_ERROR_CHECK(
        uart_set_pin(
            LAB_UART_PORT,
            LAB_UART_TX_GPIO,
            LAB_UART_RX_GPIO,
            UART_PIN_NO_CHANGE,
            UART_PIN_NO_CHANGE
        )
    );

    ESP_LOGI(TAG, "UART2 inicializada");
    ESP_LOGI(TAG, "TX: GPIO%d", LAB_UART_TX_GPIO);
    ESP_LOGI(TAG, "RX: GPIO%d", LAB_UART_RX_GPIO);
    ESP_LOGI(TAG, "Configuracao: %d baud, 8N1", LAB_UART_BAUD_RATE);
}

void app_main(void)
{
    const uint8_t test_byte = 0x55;

    uart_lab_init();

    while (1) {
        const int written_bytes = uart_write_bytes(
            LAB_UART_PORT,
            &test_byte,
            sizeof(test_byte)
        );

        if (written_bytes == sizeof(test_byte)) {
            ESP_ERROR_CHECK(
                uart_wait_tx_done(
                    LAB_UART_PORT,
                    pdMS_TO_TICKS(100)
                )
            );

            ESP_LOGI(
                TAG,
                "Byte transmitido: HEX=0x%02X DEC=%u",
                test_byte,
                test_byte
            );
        } else {
            ESP_LOGE(
                TAG,
                "Falha na transmissao: retorno=%d",
                written_bytes
            );
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
