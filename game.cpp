#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>

#include "game.h"
#include "gallery.h"

extern Gallery* gallery;

void drawPic(SDL_Renderer* renderer, int top, int left, SDL_Texture* texture, const int CELL_SIZE)
{
    SDL_Rect cell;
    cell.x = left * CELL_SIZE;
    cell.y = top * CELL_SIZE;
    cell.w = CELL_SIZE;
    cell.h = CELL_SIZE;
    SDL_RenderCopy(renderer, texture, NULL, &cell);
}

void drawBlocks(SDL_Renderer* renderer, level** rectangle, const int ROW_SIZE, const int COLUMN_SIZE, const int CELL_SIZE)
{
    for (int i = 0; i < ROW_SIZE; i++)
    {
        for (int j = 0; j < COLUMN_SIZE; j++)
        {
            drawPic(renderer, i, j, gallery->getImage(PIC_RECTANGLE), CELL_SIZE);
        }
    }
}

void setRandBoom(level **rectangle, const int ROW_SIZE, const int COLUMN_SIZE, const int numberOfBoom)
{
    int x, y;
    int count = 0;
    int t = ROW_SIZE * COLUMN_SIZE;
    do
    {
        int randRect = rand() % t;
        x = randRect / COLUMN_SIZE;
        y = randRect % COLUMN_SIZE;
        if (rectangle[x][y].isBoom == 0)
        {
            rectangle[x][y].isBoom = 1;
            count++;
        } else continue;
    }
    while(count < numberOfBoom);
}

int countOne(level** rectangle, int y, int x, const int ROW_SIZE, const int COLUMN_SIZE)
{
    int sum = 0;
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            if ((y + i) >= 0 && (y + i) < ROW_SIZE && (x + j) >= 0 && (x + j) < COLUMN_SIZE)
            {
                if (rectangle[y + i][x + j].isBoom == 1)
                {
                    sum += 1;
                }
            }
        }
    }
    return sum;
}

void print(SDL_Renderer* renderer, level** rectangle, int y, int x, const int ROW_SIZE, const int COLUMN_SIZE, const int CELL_SIZE)
{
        rectangle[y][x].state = 1;

        int sum = countOne(rectangle, y, x, ROW_SIZE, COLUMN_SIZE);

        if (1 == sum) drawPic(renderer, y, x, gallery->getImage(PIC_1), CELL_SIZE);

        if (2 == sum) drawPic(renderer, y, x, gallery->getImage(PIC_2), CELL_SIZE);

        if (3 == sum) drawPic(renderer, y, x, gallery->getImage(PIC_3), CELL_SIZE);

        if (4 == sum) drawPic(renderer, y, x, gallery->getImage(PIC_4), CELL_SIZE);

        if (5 == sum) drawPic(renderer, y, x, gallery->getImage(PIC_5), CELL_SIZE);

        if (6 == sum) drawPic(renderer, y, x, gallery->getImage(PIC_6), CELL_SIZE);

        if (7 == sum) drawPic(renderer, y, x, gallery->getImage(PIC_7), CELL_SIZE);

        if (8 == sum) drawPic(renderer, y, x, gallery->getImage(PIC_8), CELL_SIZE);
}

void openEmpty(SDL_Renderer* renderer, level** rectangle, int y, int x, const int ROW_SIZE, const int COLUMN_SIZE, const int CELL_SIZE)
{
    rectangle[y][x].state = 1;

    int sum = countOne(rectangle, y, x, ROW_SIZE, COLUMN_SIZE);

    if (sum == 0 && rectangle[y][x].state == 1)
    {
        drawPic(renderer, y, x, gallery->getImage(PIC_0), CELL_SIZE);

        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                if (i != 0 || j != 0)
                {
                    if (y + i >= 0 && y + i < ROW_SIZE && x + j >= 0 && x + j < COLUMN_SIZE)
                    {
                        if ((rectangle[y + i][x + j].state == 0 || rectangle[y + i][x + j].state == 2) && rectangle[y + i][x + j].isBoom == 0)
                        {
                            rectangle[y + i][x + j].state = 1;

                            openEmpty(renderer, rectangle, y + i, x + j, ROW_SIZE, COLUMN_SIZE, CELL_SIZE);

                            print(renderer, rectangle, y + i, x + j, ROW_SIZE, COLUMN_SIZE, CELL_SIZE);
                        }
                    }
                }
            }
        }
    }
}

void printBoom(SDL_Renderer* renderer, level** rectangle, int y, int x, const int ROW_SIZE, const int COLUMN_SIZE, const int CELL_SIZE)
{
    if (rectangle[y][x].state == 1)
    {
        for (int i = 0; i < ROW_SIZE; i++)
        {
            for (int j = 0; j < COLUMN_SIZE; j++)
            {
                if (rectangle[i][j].isBoom == 1)
                {
                    rectangle[i][j].state = 1;

                    drawPic(renderer, i, j, gallery->getImage(PIC_BOOM), CELL_SIZE);
                }
            }
        }
    }
}




