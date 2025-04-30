#ifndef INC_MATRIX_H
#define INC_MATRIX_H

#include <stdint.h>
#include <stdbool.h>

typedef int GridMatrix[8][8];

extern GridMatrix playerGrid;
extern GridMatrix shipGrid;
extern GridMatrix attackGrid;
extern GridMatrix hitGrid;

uint8_t getRow(GridMatrix* matrix, int row);
void resetMatrix(GridMatrix* matrix);
void updateShipMatrix(GridMatrix* matrix, int shipLength, int cursorX, int cursorY, bool vertical);
int updateMatrix(GridMatrix* matrix, int x, int y);
void mergeMatrix(GridMatrix* baseGrid, GridMatrix* otherGrid);
bool checkOverlap(GridMatrix* baseGrid, GridMatrix* otherGrid);

#endif