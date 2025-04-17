#include "matrix.h"
#include <stdint.h>
#include <stdbool.h>

GridMatrix playerGrid = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
};

GridMatrix attackGrid = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
};

uint8_t getRow(GridMatrix* matrix, int row) {
    int col;
    uint8_t data = 0;

    for (col = 0; col <= 7; col++) {
        if ((*matrix)[row][col] == 1) {
            data |= (1 << (7 - col));
        }
    }

    return data;
}

void resetMatrix(GridMatrix* matrix) {
    int i, j;
    for (i = 0; i <= 7; i++) {
        for (j = 0; j <=7; j++) {
            (*matrix)[i][j] = 0;
        }
    }
}

void updateMatrix(GridMatrix* matrix, int shipLength, int cursorX, int cursorY, bool vertical) {
    int i;

    if (vertical) {
        for (i = cursorY; i > cursorY - shipLength; i--) {
            (*matrix)[i][cursorX] = 1;
        }
    } else {
        for (i = cursorX; i < cursorX + shipLength; i++) {
            (*matrix)[cursorY][i] = 1;
        }
    }
}

void mergeMatrix(GridMatrix* baseGrid, GridMatrix* otherGrid) {
    int i, j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            (*baseGrid)[i][j] |= (*otherGrid)[i][j];
        }
    }
}

bool checkOverlap(GridMatrix* baseGrid, GridMatrix* otherGrid) {
    int i, j;

    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            if (((*baseGrid)[i][j] == 1) && ((*otherGrid)[i][j] == 1)) {
                return true;
            }
        }
    }

    return false;
}