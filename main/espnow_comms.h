#ifndef INC_ESPNOW_COMMS_H
#define INC_ESPNOW_COMMS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "esp_now.h"

typedef enum {
    MSG_READY,
    MSG_ATTACK,
    MSG_HIT,
    MSG_WIN_CONDITION
} MessageType;

typedef struct {
    MessageType msg;
    union {
        struct {
            bool ready;
            int diceRoll;
        } readyStatusData;

        struct {
            int x;
            int y;
        } attackData;

        struct {
            bool hit;
        } hitData;

        struct {
            bool allSunk;
        } winConditionData;
    } message;
} GameData;

extern const uint8_t peerMac[6];

void initWifi(void);
void initESPNOW(void);
void onSend(const uint8_t* data, esp_now_send_status_t status);
void onReceive(const esp_now_recv_info_t* info, const uint8_t* receivedData, int lenData);
void sendReadyStatusData(bool isReady, uint32_t randomNumber);
void sendAttackData(int x, int y);
void sendHitData(bool wasHit);
void sendWinConditionData(bool status);
void handleDataTask(void* pvParameters);

#endif