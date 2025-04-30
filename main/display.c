#include "display.h"
#include "matrix.h"
#include <stdint.h>
#include <string.h>
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

spi_device_handle_t playerHandle;
spi_device_handle_t attackHandle;

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

static void sendCmd(spi_device_handle_t devHandle, uint8_t addr, uint8_t data) {
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

void displayShips(void) {
    updateDisplay(&shipGrid, playerHandle);
}

void displayAllHits(void) {
    updateDisplay(&hitsGrid, playerHandle);
}

void displayWaitingScreen(void) {
    sendCmd(attackHandle, 4, 0xC0);
    sendCmd(attackHandle, 5, 0xC0);
    vTaskDelay(pdMS_TO_TICKS(300));

    sendCmd(attackHandle, 4, 0xD8);
    sendCmd(attackHandle, 5, 0xD8);
    vTaskDelay(pdMS_TO_TICKS(300));

    sendCmd(attackHandle, 4, 0xDB);
    sendCmd(attackHandle, 5, 0xDB);
    vTaskDelay(pdMS_TO_TICKS(300));

    sendCmd(attackHandle, 4, 0x00);
    sendCmd(attackHandle, 5, 0x00);
    vTaskDelay(pdMS_TO_TICKS(300));
}

void displayWin(void) {
    //Clearing matrices
    resetMatrix(&playerGrid);
    resetMatrix(&attackGrid);
    updateDisplay(&playerGrid, playerHandle);
    updateDisplay(&attackGrid, attackHandle);

    //Display W on screen that previously showed player's ships
    sendCmd(playerHandle, 2, 0x63);
    sendCmd(playerHandle, 3, 0x63);
    sendCmd(playerHandle, 4, 0x63);
    sendCmd(playerHandle, 5, 0x6B);
    sendCmd(playerHandle, 6, 0x7F);
    sendCmd(playerHandle, 7, 0x77);
    sendCmd(playerHandle, 8, 0x63);

    //Display happy face on screen that previously showed player's attack grid
    sendCmd(attackHandle, 2, 0x66);
    sendCmd(attackHandle, 3, 0x66);
    sendCmd(attackHandle, 5, 0x81);
    sendCmd(attackHandle, 6, 0x42);
    sendCmd(attackHandle, 7, 0x3C);
}

void displayLose(void) {
    //Clearing matrices
    resetMatrix(&playerGrid);
    resetMatrix(&attackGrid);
    updateDisplay(&playerGrid, playerHandle);
    updateDisplay(&attackGrid, attackHandle);

    //Display L on screen that previously showed the player's ships
    sendCmd(playerHandle, 2, 0x60);
    sendCmd(playerHandle, 3, 0x60);
    sendCmd(playerHandle, 4, 0x60);
    sendCmd(playerHandle, 5, 0x60);
    sendCmd(playerHandle, 6, 0x60);
    sendCmd(playerHandle, 7, 0x60);
    sendCmd(playerHandle, 8, 0x7E);

    //Display sad face on screen that previously showed player's attack grid
    sendCmd(attackHandle, 2, 0x66);
    sendCmd(attackHandle, 3, 0x66);
    sendCmd(attackHandle, 5, 0x3C);  
    sendCmd(attackHandle, 6, 0x42);      
    sendCmd(attackHandle, 7, 0x81);

}







