#ifndef INC_ESPNOW_COMMS_H
#define INC_ESPNOW_COMMS_H

#include <stdint.h>
#include "freertos/queue.h"

const uint8_t peerMac[] = {0x70, 0x04, 0x1D, 0xF7, 0xB3, 0xEE}       //MAC address of the other ESP
QueueHandle_t dataQueue;

void initWifi(void);
void initESPNOW(void);
void onSend(const uint8_t* data, esp_now_send_status status);
void onReceive(const esp_now_recv_info_t* info, const uint8_t* receivedData, int lenData);

#endif