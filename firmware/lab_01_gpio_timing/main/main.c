#include <stdbool.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_rom_sys.h"

/*
 * Laboratório 01:
 * GPIO, níveis lógicos, frequência, período, duty cycle
 * e comparação entre temporização FreeRTOS e microssegundos.
 */

#define SIGNAL_GPIO GPIO_NUM_25

/*
 * Modos disponíveis.
 *
 * Altere apenas LAB_MODE para selecionar o experimento.
 */
#define MODE_STATIC_LOW          0
#define MODE_STATIC_HIGH         1
#define MODE_1HZ_FREERTOS        2
#define MODE_100HZ_DELAY_US      3
#define MODE_1KHZ_DELAY_US       4
#define MODE_100HZ_FREERTOS      5


#define LAB_MODE MODE_100HZ_FREERTOS

/*
 * Os modos com delay em microssegundos trabalham em rajadas.
 *
 * Isso evita manter a tarefa ocupando continuamente a CPU
 * e facilita identificar o começo e o fim da captura.
 */
#define BURST_DURATION_MS 1000U
#define BURST_PAUSE_MS    1000U

static const char *TAG = "GPIO_LAB";

static void initialize_gpio(void)
{
    const gpio_config_t gpio_configuration = {
        .pin_bit_mask = (1ULL << SIGNAL_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&gpio_configuration));
    ESP_ERROR_CHECK(gpio_set_level(SIGNAL_GPIO, 0));
}

static void hold_static_level(uint32_t level)
{
    ESP_ERROR_CHECK(gpio_set_level(SIGNAL_GPIO, level));

    ESP_LOGI(
        TAG,
        "GPIO %d mantido em nível %s",
        SIGNAL_GPIO,
        level ? "ALTO" : "BAIXO"
    );

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void generate_square_wave_freertos(uint32_t half_period_ms)
{
    const TickType_t half_period_ticks =
        pdMS_TO_TICKS(half_period_ms);

    if (half_period_ticks == 0) {
        ESP_LOGE(
            TAG,
            "Não é possível representar %lu ms com tick de %d Hz",
            (unsigned long)half_period_ms,
            configTICK_RATE_HZ
        );

        ESP_LOGE(
            TAG,
            "A resolução atual é de aproximadamente %d ms por tick",
            portTICK_PERIOD_MS
        );

        ESP_ERROR_CHECK(gpio_set_level(SIGNAL_GPIO, 0));

        while (true) {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    ESP_LOGI(
        TAG,
        "Onda com FreeRTOS: meio período = %lu ms = %lu ticks",
        (unsigned long)half_period_ms,
        (unsigned long)half_period_ticks
    );

    TickType_t last_wake_time = xTaskGetTickCount();

    while (true) {
        gpio_set_level(SIGNAL_GPIO, 1);
        xTaskDelayUntil(&last_wake_time, half_period_ticks);

        gpio_set_level(SIGNAL_GPIO, 0);
        xTaskDelayUntil(&last_wake_time, half_period_ticks);
    }
}

static void generate_square_wave_delay_us(uint32_t half_period_us)
{
    const uint32_t total_burst_time_us =
        BURST_DURATION_MS * 1000U;

    const uint32_t full_period_us =
        half_period_us * 2U;

    const uint32_t cycles_per_burst =
        total_burst_time_us / full_period_us;

    ESP_LOGI(
        TAG,
        "Onda por delay_us: meio período = %lu us",
        (unsigned long)half_period_us
    );

    ESP_LOGI(
        TAG,
        "Cada rajada terá %lu ciclos e aproximadamente %u ms",
        (unsigned long)cycles_per_burst,
        BURST_DURATION_MS
    );

    while (true) {
        /*
         * Não colocamos logs dentro deste laço.
         * Escrever na UART alteraria a temporização do sinal.
         */
        for (uint32_t cycle = 0; cycle < cycles_per_burst; cycle++) {
            gpio_set_level(SIGNAL_GPIO, 1);
            esp_rom_delay_us(half_period_us);

            gpio_set_level(SIGNAL_GPIO, 0);
            esp_rom_delay_us(half_period_us);
        }

        gpio_set_level(SIGNAL_GPIO, 0);

        ESP_LOGI(TAG, "Fim da rajada");

        vTaskDelay(pdMS_TO_TICKS(BURST_PAUSE_MS));
    }
}

void app_main(void)
{
    initialize_gpio();

    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "Laboratório de GPIO e temporização");
    ESP_LOGI(TAG, "GPIO utilizado: %d", SIGNAL_GPIO);
    ESP_LOGI(TAG, "FreeRTOS tick rate: %d Hz", configTICK_RATE_HZ);
    ESP_LOGI(TAG, "Período do tick: %d ms", portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "Modo selecionado: %d", LAB_MODE);
    ESP_LOGI(TAG, "========================================");

    switch (LAB_MODE) {
        case MODE_STATIC_LOW:
            hold_static_level(0);
            break;

        case MODE_STATIC_HIGH:
            hold_static_level(1);
            break;

        case MODE_1HZ_FREERTOS:
            /*
             * 1 Hz:
             * período total = 1000 ms
             * meio período = 500 ms
             */
            generate_square_wave_freertos(500);
            break;

        case MODE_100HZ_DELAY_US:
            /*
             * 100 Hz:
             * período total = 10 ms
             * meio período = 5 ms = 5000 us
             */
            generate_square_wave_delay_us(5000);
            break;

        case MODE_1KHZ_DELAY_US:
            /*
             * 1 kHz:
             * período total = 1 ms
             * meio período = 500 us
             */
            generate_square_wave_delay_us(500);
            break;

        case MODE_100HZ_FREERTOS:
            /*
             * 100 Hz:
             * período total = 10 ms
             * meio período = 5 ms
             *
             * Este modo exige resolução de tick suficiente.
             */
            generate_square_wave_freertos(5);
            break;

        default:
            ESP_LOGE(TAG, "Modo de laboratório inválido");

            while (true) {
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
    }
}
