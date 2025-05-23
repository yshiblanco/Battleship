#ifndef INC_DISPLAY_H
#define INC_DISPLAY_H

#include <stdint.h>
#include "matrix.h"
#include "driver/spi_common.h"
#include "driver/spi_master.h"

extern spi_device_handle_t playerHandle;
extern spi_device_handle_t attackHandle;

void initSPI(void);
void initDisplays(void);
void updateDisplay(GridMatrix* grid, spi_device_handle_t devHandle);
void displayShips(void);
void displayAllHits(void);
void displayWaitingScreen(void);
void displayWin(void);
void displayLose(void);

#endif