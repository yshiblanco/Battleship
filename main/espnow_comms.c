#include "espnow_comms.h"
#include "esp_now.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_system.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_err.h"

void initWifi(void) {
    //Initializing WiFi
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    wifi_init_config_t defaultConfig = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&defaultConfig));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void onSend(const uint8_t* macAddress, esp_now_send_status status) {

}

void onReceive(const esp_now_recv_info_t* info, const uint8_t* receivedData, int lenData) {

}

void initESPNOW(void) {
    //Initializing ESP NOW, registering callback functions, and opening two-way communication
    esp_now_init();
    
    esp_now_register_send_cb(onSend);
    esp_now_register_recv_cb(onReceive);

    esp_now_peer_info_t peerInfo = {};
    memcp(peerInfo.peer_addr, peerMac, sizeof(peerInfo.peer_addr)); //copy mac address into the peer address

    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    esp_err_t result = esp_now_add_peer(&peerInfo);
    if (result == ESP_OK) {
        printf("Peer added successfully!\n");
    } else {
        printf("Failed to add peer: %d\n", result);
    }

}

void sendData((uint8_t*) data) {
    esp_now_send(peerMac, data, sizeof(uint8_t));
}

void handleDataTask