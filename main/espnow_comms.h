#ifndef INC_ESPNOW_COMMS_H
#define INC_ESPNOW_COMMS_H

#include <stdint.h>
#include <stddef.h>
#include "esp_now.h"

extern const uint8_t peerMac[6];

void initWifi(void);
void initESPNOW(void);
void onSend(const uint8_t* data, esp_now_send_status_t status);
void onReceive(const esp_now_recv_info_t* info, const uint8_t* receivedData, int lenData);
void sendData(uint8_t* data, size_t dataSize);
void handleDataTask(void* pvParameters);

#endif