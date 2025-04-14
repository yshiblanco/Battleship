#include "sdkconfig.h"
#include "espnow_comms.h"
#include "display.h"
#include "matrix.h"
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

//GPIO pins for buttons
#define CONFIRM_BUTTON  38
#define ROTATE_BUTTON   33
#define UP_BUTTON       34
#define RIGHT_BUTTON    35
#define DOWN_BUTTON     36
#define LEFT_BUTTON     37   

void createTasks(void) {
    xTaskCreatePinnedToCore(handleDataTask, "handle_data", 4096, NULL, 0, NULL, 0);
}

void initGPIO(void) {
    gpio_set_direction(CONFIRM_BUTTON, GPIO_MODE_INPUT);
    gpio_pullup_en(CONFIRM_BUTTON);

    gpio_set_direction(ROTATE_BUTTON, GPIO_MODE_INPUT);
    gpio_pullup_en(ROTATE_BUTTON);

    gpio_set_direction(UP_BUTTON, GPIO_MODE_INPUT);
    gpio_pullup_en(UP_BUTTON);

    gpio_set_direction(RIGHT_BUTTON, GPIO_MODE_INPUT);
    gpio_pullup_en(RIGHT_BUTTON);

    gpio_set_direction(DOWN_BUTTON, GPIO_MODE_INPUT);
    gpio_pullup_en(DOWN_BUTTON);

    gpio_set_direction(LEFT_BUTTON, GPIO_MODE_INPUT);
    gpio_pullup_en(LEFT_BUTTON);
}

void app_main(void) {
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    initWifi();
    initESPNOW();
    initSPI();
    initGPIO();
    initDisplays();
    createTasks();

    while(1){
        //transmitData();
        //vTaskDelay(pdMS_TO_TICKS(1000));  // Wait 1 second to avoid watchdog timeout
        if (gpio_get_level(CONFIRM_BUTTON) == 0) {
            printf("Confirm pressed!\n");
        } else if (gpio_get_level(ROTATE_BUTTON) == 0) {
            printf("Rotate pressed!\n");
        } else if (gpio_get_level(UP_BUTTON) == 0) {
            printf("Up pressed!\n");
        } else if (gpio_get_level(RIGHT_BUTTON) == 0) {
            printf("Right pressed!\n");
        } else if (gpio_get_level(DOWN_BUTTON) == 0) {
            printf("Down pressed!\n");
        } else if (gpio_get_level(LEFT_BUTTON) == 0) {
            printf("Left pressed!\n");
        } 
        vTaskDelay(10);
    }


}
