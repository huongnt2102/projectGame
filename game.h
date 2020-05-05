#ifndef GAME_H
#define GAME_H

#include "gallery.h"

struct level
{
    int isBoom; // 1 - boom, 0-0
    int state; // 0-chua mo; 1-mo, 2-flag
};

void drawPic(SDL_Renderer* renderer, int top, int left, SDL_Texture* texture, const int CELL_SIZE);

void drawBlocks(SDL_Renderer* renderer, level** rectangle, const int ROW_SIZE, const int COLUMN_SIZE, const int CELL_SIZE);

void setRandBoom(level **rectangle, const int ROW_SIZE, const int COLUMN_SIZE, const int numberOfBoom);

int countOne(level** rectangle, int y, int x, const int ROW_SIZE, const int COLUMN_SIZE);

void print(SDL_Renderer* renderer, level** rectangle, int y, int x, const int ROW_SIZE, const int COLUMN_SIZE, const int CELL_SIZE);

void openEmpty(SDL_Renderer * renderer, level** rectangle, int y, int x, const int ROW_SIZE, const int COLUMN_SIZE, const int CELL_SIZE);

void printBoom(SDL_Renderer* renderer, level ** rectangle, int y, int x, const int ROW_SIZE, const int COLUMN_SIZE, const int CELL_SIZE);


#endif // GAME_H

