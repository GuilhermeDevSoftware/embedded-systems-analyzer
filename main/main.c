#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_system.h"

static const char *TAG = "AMBIENTE";

void app_main(void)
{
    ESP_LOGI(TAG, "Projeto ambiente_analisador iniciado");
    ESP_LOGI(TAG, "Versao do ESP-IDF: %s", esp_get_idf_version());

    ESP_LOGI(
        TAG,
        "Heap livre inicial: %lu bytes",
        (unsigned long)esp_get_free_heap_size()
    );

    while (1)
    {
        ESP_LOGI(TAG, "Firmware ativo");

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
