#ifndef GAME_H
#define GAME_H

#include "gallery.h"
#include "draw.h"

struct level
{
    int isBomb; // 1 - boom, 0-0
    int state; // 0-chua mo; 1-mo, 2-flag
};

void drawPic(SDL_Renderer* renderer, int top, int left, SDL_Texture* texture, const int CELL_SIZE);

void drawBlocks(SDL_Renderer* renderer, level** rectangle, const int ROW_SIZE, const int COLUMN_SIZE, const int CELL_SIZE);

void setRandBomb(level **rectangle, const int ROW_SIZE, const int COLUMN_SIZE, int numberOfBomb);

int countOne(level** rectangle, int i, int j, const int ROW_SIZE, const int COLUMN_SIZE);

void print(SDL_Renderer* renderer, level** rectangle, int y, int x, const int ROW_SIZE, const int COLUMN_SIZE, const int CELL_SIZE);

void openEmpty(SDL_Renderer * renderer, level** rectangle, int row, int col, const int ROW_SIZE, const int COLUMN_SIZE, const int CELL_SIZE);

void printBomb(SDL_Renderer* renderer, level ** rectangle, int y, int x, const int ROW_SIZE, const int COLUMN_SIZE, const int CELL_SIZE);


#endif // GAME_H
