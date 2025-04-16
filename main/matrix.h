#ifndef INC_MATRIX_H
#define INC_MATRIX_H

#include <stdint.h>
#include <stdbool.h>

typedef int GridMatrix[8][8];

extern GridMatrix playerGrid;
extern GridMatrix attackGrid;

uint8_t getRow(GridMatrix* matrix, int row);
void resetMatrix(GridMatrix* matrix);
void updateMatrix(GridMatrix* matrix, int shipLength, int cursorX, int cursorY, bool vertical);

#endif