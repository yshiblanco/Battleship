#include "espnow_comms.h"
#include "display.h"
#include "attack_logic.h"
#include "game_logic.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include <string.h>
#include <stdbool.h>


static const char* TAG = "ESP-NOW";
QueueHandle_t dataQueue;
const uint8_t peerMac[6] = {0x70, 0x04, 0x1D, 0xF7, 0xB3, 0xEE};

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
    if (status == ESP_NOW_SEND_SUCCESS) {
        ESP_LOGI(TAG, "Data was succesfully sent\n");
    } else {
        ESP_LOGI(TAG, "Data failed to send\n");
    }
}

void onReceive(const esp_now_recv_info_t* info, const uint8_t* receivedData, int lenData) {
    ESP_LOGI(TAG, "Data received: %d\n", *receivedData);

    if (xQueueSend(dataQueue, receivedData, portMAX_DELAY) != pdTRUE) {
        ESP_LOGI(TAG, "Data failed to be added to the queue (queue is full)\n");
    }
}

void initESPNOW(void) {
    //Initializing ESP NOW
    esp_now_init();
    
    //Registering callback functions
    esp_now_register_send_cb(onSend);
    esp_now_register_recv_cb(onReceive);

    //Defining peer info
    esp_now_peer_info_t *peerInfo = malloc(sizeof(esp_now_peer_info_t));    //allocating memory with size specific to the struct
    memset(peerInfo, 0, sizeof(esp_now_peer_info_t));                       //sets all the bytes of the allocated memory to 0 (ensuring no issues from prev data stored there)
    peerInfo->channel = 1;
    peerInfo->ifidx = ESP_IF_WIFI_STA;
    peerInfo->encrypt = false;
    memcpy(peerInfo->peer_addr, peerMac, ESP_NOW_ETH_ALEN);         //copies mac addr into the corresponding place in the peerInfo struct

    //Adding peer and logging outcome
    esp_err_t result = esp_now_add_peer(peerInfo);
    if (result == ESP_OK) {
        ESP_LOGI(TAG, "Peer added successfully!\n");
    } else {
        ESP_LOGI(TAG, "Failed to add peer: %d\n", result);
    }

    free(peerInfo);                                                 //freeing up the memory we allocated earlier (must be done when dynamically allocating memory)

    //Creating queue of size 4 for handling data sent over ESP-NOW
    dataQueue = xQueueCreate(4, sizeof(GameData));

}

void sendReadyStatusData(bool isReady, uint32_t randomNumber) {
    GameData data = {
        .msg = MSG_READY,
        .message.readyStatusData = {
            .ready = isReady,
            .diceRoll = randomNumber
        }
    };

    esp_now_send(peerMac, (uint8_t*)&data, sizeof(GameData));
}

void sendAttackData(int x, int y) {
    GameData data = {
        .msg = MSG_ATTACK,
        .message.attackData = {
            .x = x,
            .y = y
        }
    };

    esp_now_send(peerMac, (uint8_t*)&data, sizeof(GameData));
}

void sendHitData(bool wasHit) {
    GameData data = {
        .msg = MSG_HIT,
        .message.hitData = {
            .hit = wasHit
        }
    };

    esp_now_send(peerMac, (uint8_t*)&data, sizeof(GameData));
}

void sendWinConditionData(bool status) {
    GameData data = {
        .msg = MSG_WIN_CONDITION,
        .message.winConditionData = {
            .allSunk = status
        }
    };

    esp_now_send(peerMac, (uint8_t*)&data, sizeof(GameData));
}

void handleDataTask(void* pvParameters) {

    GameData receivedData;

    while(1) {
        
    }

    vTaskDelete(NULL);
}