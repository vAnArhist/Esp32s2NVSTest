#include <stdio.h>
#include "driver/gpio.h"
#include "sdkconfig.h"
// Include FreeRTOS for delay
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"
#include "esp_system.h"
#include "main.h"

static const char* LogTag = "Main";

#define DELAY_MS(ms)                vTaskDelay(ms / portTICK_PERIOD_MS)
static Main myMain;

bool Main::setup()
{
    gpio_reset_pin(GPIO_NUM_15);
    gpio_set_direction(GPIO_NUM_15, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_level(GPIO_NUM_15, 0);
    DELAY_MS(1000);
    smpConfig = Core::Config::getInstance();
    ESP_LOGI(LogTag, "smpConfig start");
    devId_t deviceWrite = { };
    devId_t deviceRead = { };
    deviceWrite.id = 1;
    uint8_t mac[6] = { 0xff, 0xf1, 0xfa, 0xfc, 0x00, 0x01 };
    memcpy(deviceWrite.macAddr, mac, sizeof(deviceWrite.macAddr));

    smpConfig->writeId(1, deviceWrite);
    bool stat = smpConfig->readId(1, deviceRead);

    ESP_LOGI(LogTag, "Id: %d\n Mac: ", deviceRead.id);

    for (int i = 0; i < 6; i++) {
        ESP_LOGI(LogTag, "%02x", deviceRead.macAddr[i]);
    }

    return true;
}

extern "C" void app_main(void)
{
    ESP_LOGI(LogTag, "Chip started!");

    if (myMain.setup()) {
        while(1) {
            myMain.run();
        }
    }

    while (1) {

    }
}

void Main::run()
{
    gpio_set_level(GPIO_NUM_15, 1);
    DELAY_MS(500);
    gpio_set_level(GPIO_NUM_15, 0);
    DELAY_MS(500);
}
