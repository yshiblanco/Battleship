#include "display.h"
#include "matrix.h"
#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

//GPIO pins for buttons
#define CONFIRM_BUTTON  38
#define ROTATE_BUTTON   33
#define UP_BUTTON       34
#define RIGHT_BUTTON    35
#define DOWN_BUTTON     36
#define LEFT_BUTTON     37       

//SPI defines for grids
#define SPI2_MOSI_PIN    11
#define SPI2_CS0_PIN     10             //player grid
#define SPI2_CS1_PIN     9              //attack grid
#define SPI2_CLK_PIN     12

//Configuring LED matrices as devices
static spi_device_interface_config_t playerMatrix = {
    .address_bits = 8,
    .clock_speed_hz = 1000000,                  //1MHz clock speed; matrix has max rate of 10MHz
    .mode = 0,
    .spics_io_num = SPI2_CS0_PIN,
    .queue_size = 2,
};

static spi_device_interface_config_t attackMatrix = {
    .address_bits = 8,
    .clock_speed_hz = 1000000,                  
    .mode = 0,
    .spics_io_num = SPI2_CS1_PIN,
    .queue_size = 2,
};

static spi_device_handle_t playerHandle;
static spi_device_handle_t attackHandle;

void initSPI(void) {
    //Initialize configuration for SPI buses
    spi_bus_config_t bus2Config = {
        .miso_io_num = -1,
        .mosi_io_num = SPI2_MOSI_PIN,
        .sclk_io_num = SPI2_CLK_PIN,
        .max_transfer_sz = 32,                 
    };

    //Initialize SPI bus
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus2Config, 0));

    //Adding LED matrices to SPI buses
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &playerMatrix, &playerHandle));
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &attackMatrix, &attackHandle));
}

void sendCmd(spi_device_handle_t devHandle, uint8_t addr, uint8_t data) {
    spi_transaction_t trans = {0};
    trans.flags = SPI_TRANS_USE_TXDATA;
    trans.length = 16;
    trans.tx_data[0] = addr;
    trans.tx_data[1] = data;
    ESP_ERROR_CHECK(spi_device_polling_transmit(devHandle, &trans));
}

void initDisplays(void) {
    //Initializing Player Grid
    sendCmd(playerHandle, 0x0F, 0x00); //Disable display test
    sendCmd(playerHandle, 0x0C, 0x01); //Exit shutdown mode (turn on)
    sendCmd(playerHandle, 0x0B, 0x07); //Scan all 8 digits
    sendCmd(playerHandle, 0x0A, 0x01); //Set intensity/brightness
    sendCmd(playerHandle, 0x09, 0x00); //No decode mode

    //Initializing Attack Grid
    sendCmd(attackHandle, 0x0F, 0x00); 
    sendCmd(attackHandle, 0x0C, 0x01); 
    sendCmd(attackHandle, 0x0B, 0x07); 
    sendCmd(attackHandle, 0x0A, 0x01); 
    sendCmd(attackHandle, 0x09, 0x00); 
}
void updateDisplay(GridMatrix* grid, spi_device_handle_t devHandle) {
    uint8_t addr, data;

    for (addr = 1; addr <= 8; addr++) {
        data = getRow(grid, addr - 1);
        sendCmd(devHandle, addr, data);
    }
}

void placeShips(int shipLength) {
    GridMatrix displayedGrid = {0};
    GridMatrix tempGrid = {0};

    int cursorX = 7;
    int cursorY = 7;
    bool vertical = true;

    while(1) {

        //User cursor moves up
        if (gpio_get_level(UP_BUTTON) == 0) {
            if ((vertical && (cursorY - shipLength < 0)) || (!vertical && (cursorY - 1 < 0))) {
                //Do nothing, goes past bounds
            } else {
                cursorY--;
            }
        }

        //User cursor moves right
        if (gpio_get_level(RIGHT_BUTTON) == 0) {
            if ((!vertical && (cursorX + shipLength > 7)) || (vertical && (cursorX + 1 > 7))) {
                //Do nothing, goes past bounds
            } else {
                cursorX++;
            }
        }

        //User cursor moves down
        if (gpio_get_level(DOWN_BUTTON) == 0) {
            if (cursorY + 1 > 7) {
                //Do nothing, goes past bounds
            } else {
                cursorY++;
            }
        }

        //User cursor moves left
        if (gpio_get_level(LEFT_BUTTON) == 0) {
            if (cursorX - 1 < 0) {
                //Do nothing, goes past bounds
            } else {
                cursorX--;
            }
        }

        if (gpio_get_level(ROTATE_BUTTON) == 0)  {
            if (!((vertical && (cursorX > 7 - shipLength + 1)) || (!vertical && (cursorY < shipLength - 1)))) {
                vertical = !vertical;
            }
        }

        //holds information only about the ship currently being placed
        resetMatrix(&tempGrid);
        updatePlayerMatrix(&tempGrid, shipLength, cursorX, cursorY, vertical);        

        if (gpio_get_level(CONFIRM_BUTTON) == 0 && !(checkOverlap(&playerGrid, &tempGrid))) {
            break;
        }

        //displays the ship currently being placed and the previous ships placed
        resetMatrix(&displayedGrid);
        mergeMatrix(&displayedGrid, &playerGrid);
        updatePlayerMatrix(&displayedGrid, shipLength, cursorX, cursorY, vertical);
        updateDisplay(&displayedGrid, playerHandle);

        vTaskDelay(pdMS_TO_TICKS(200));
    }

    mergeMatrix(&playerGrid, &tempGrid);

}







