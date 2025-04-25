#ifndef INC_MATRIX_H
#define INC_MATRIX_H

#include <stdint.h>
#include <stdbool.h>

typedef int GridMatrix[8][8];

extern GridMatrix playerGrid;
extern GridMatrix attackGrid;

uint8_t getRow(GridMatrix* matrix, int row);
void resetMatrix(GridMatrix* matrix);
void updatePlayerMatrix(int shipLength, int cursorX, int cursorY, bool vertical);
void updateAttackMatrix(GameData* data);
void mergeMatrix(GridMatrix* baseGrid, GridMatrix* otherGrid);
bool checkOverlap(GridMatrix* baseGrid, GridMatrix* otherGrid);

#endif