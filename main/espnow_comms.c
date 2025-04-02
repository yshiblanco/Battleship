#include "espnow_comms.h"
#include "esp_now.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "freertos/freertos.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include <stdio.h>

static const char* TAG = "ESP-NOW";
QueueHandle_t dataQueue;

void initWifi(void) {
    //Initializing WiFi (needed for ESP-NOW)
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    wifi_init_config_t defaultConfig = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&defaultConfig));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void onSend(const uint8_t* macAddress, esp_now_send_status_t status) {
    if (status == ESP_OK) {
        ESP_LOGI(TAG, "Data was succesfully sent\n");
    } else {
        ESP_LOGI(TAG, "Data failed to send\n");
    }
}

void onReceive(const esp_now_recv_info_t* info, const uint8_t* receivedData, int lenData) {
    ESP_LOGI(TAG, "Data receieved: %d\n", *receievedData);

    if (xQueueSend(dataQueue, receievedData, 10) != pdTRUE) {
        ESP_LOGI(TAG, "Data failed to be added to the queue (queue is full)\n");
    }
}

void initESPNOW(void) {
    //Initializing ESP NOW, registering callback functions, and opening two-way communication
    esp_now_init();
    
    esp_now_register_send_cb(onSend);
    esp_now_register_recv_cb(onReceive);

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, peerMac, sizeof(peerInfo.peer_addr)); //copy mac address into the peer address

    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    esp_err_t result = esp_now_add_peer(&peerInfo);
    if (result == ESP_OK) {
        ESP_LOGI(TAG, "Peer added successfully!\n");
    } else {
        ESP_LOGI(TAG, "Failed to add peer: %d\n", result);
    }

    //Creating queue of size 5 for handling data sent over ESP-NOW
    dataQueue = xQueueCreate(5, sizeof(uint8_t*));

}

void sendData(uint8_t* data) {
    esp_now_send(peerMac, data, sizeof(uint8_t));
}

void handleDataTask(void* pvParameters) {

    uint8_t* receivedData;

    while(1) {
        if (xQueueReceive(dataQueue, &receivedData, 10) == pdTRUE) {
            printf("Data received: %d", receivedData);          //test
        }
    }

    vTaskDelete(NULL);
}