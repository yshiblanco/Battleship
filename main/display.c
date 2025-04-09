#include "display.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

//SPI defines for player grid
#define SPI2_MOSI_PIN    11
#define SPI2_CS0_PIN     10
#define SPI2_CS1_PIN     9
#define SPI2_CLK_PIN     12

//SPI defines for attack grid
#define SPI3_MOSI_PIN    3
#define SPI3_CS0_PIN     5
#define SPI3_CS1_PIN     6
#define SPI3_CLK_PIN     4

void SPI_init(void) {
    esp_err_t ret;

    //Initialize configuration for SPI buses
    spi_bus_config_t bus2Config = {
        .miso_io_num = -1,
        .mosi_io_num = SPI2_MOSI_PIN,
        .sclk_io_num = SPI2_CLK_PIN,
        .max_transfer_sz = 32,                 
    };

    spi_bus_config_t bus3Config = {
        .miso_io_num = -1,
        .mosi_io_num = SPI3_MOSI_PIN,
        .sclk_io_num = SPI3_CLK_PIN,
        .max_transfer_sz = 32,
    };

    //Initialize SPI buses
    ret = spi_bus_initialize(SPI2_HOST, &bus2Config, 0);
    ESP_ERROR_CHECK(ret);

    ret = spi_bus_initialize(SPI3_HOST, &bus3Config, 0);
    ESP_ERROR_CHECK(ret);

    //Configuring LED matrices
    spi_device_interface_config_t playerRow0 = {
        .clock_speed_hz = 1000000,                  //1MHz clock speed; matrix has max rate of 10MHz
        .mode = 0,
        .spics_io_num = SPI2_CS0_PIN,
        .queue_size = 8,
    };

    spi_device_interface_config_t playerRow1 = {
        .clock_speed_hz = 1000000,                  
        .mode = 0,
        .spics_io_num = SPI2_CS1_PIN,
        .queue_size = 8,
    };

    spi_device_interface_config_t attackRow0 = {
        .clock_speed_hz = 1000000,                  
        .mode = 0,
        .spics_io_num = SPI3_CS0_PIN,
        .queue_size = 8,
    };

    spi_device_interface_config_t attackRow1 = {
        .clock_speed_hz = 1000000,                  
        .mode = 0,
        .spics_io_num = SPI3_CS1_PIN,
        .queue_size = 8,
    };

    //Adding LED matrices to SPI buses
    spi_device_handle_t playerRow0Handle;
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &playerRow0, &playerRow0Handle));

    spi_device_handle_t playerRow1Handle;
    ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &playerRow1, &playerRow1Handle));

    spi_device_handle_t attackRow0Handle;
    ESP_ERROR_CHECK(spi_bus_add_device(SPI3_HOST, &attackRow0, &attackRow0Handle));

    spi_device_handle_t attackRow1Handle;
    ESP_ERROR_CHECK(spi_bus_add_device(SPI3_HOST, &attackRow1, &attackRow1Handle));



}