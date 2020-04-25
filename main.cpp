#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <SDL.h>
#include <SDL_mixer.h>

#include "SDL_utils.h"
#include "gallery.h"
#include "game.h"

using namespace std;

const int SCREEN_WIDTH = 360; 
const int SCREEN_HEIGHT = 360;
const char WINDOW_TITLE[] = "MINESWEEPER GAME";

const int CELL_SIZE = 30;
const int ROW_SIZE = 12;
const int COLUMN_SIZE = 12;

Gallery* gallery = nullptr; // global picture manager

int main(int argc, char* argv[])
{
    srand(time(0));
    SDL_Window* window;
    SDL_Renderer* renderer;

    Mix_Chunk *boom;
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError();
        return -1;
    }
    boom = Mix_LoadWAV("boom.wav");

    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    bool isRunning = true;

    gallery = new Gallery(renderer);

    level **rectangle = new level *[ROW_SIZE];

    for (int i = 0; i < ROW_SIZE; i++)
    {
        rectangle[i] = new level [COLUMN_SIZE];
    }
    for (int i = 0; i < ROW_SIZE; i++)
    {
        for (int j = 0; j < COLUMN_SIZE; j++)
        {
            rectangle[i][j].isBomb = 0;
            rectangle[i][j].state = 0;
        }
    }
    drawBlocks(renderer, rectangle, ROW_SIZE, COLUMN_SIZE, CELL_SIZE);

    int numberOfBomb = 40;

    setRandBomb(rectangle, ROW_SIZE, COLUMN_SIZE, numberOfBomb);

    for (int i = 0; i < ROW_SIZE; i++)
    {
        for (int j = 0; j < COLUMN_SIZE; j++)
        {
            if (rectangle[i][j].isBomb == 0)
            {
                cout << countOne(rectangle, i, j, ROW_SIZE, COLUMN_SIZE) << " ";
            }
            if (rectangle[i][j].isBomb == 1)
            {
                cout << "*" << " ";
            }
        }
        cout << endl;
    }

    SDL_Event mouse;

    while (isRunning)
    {
        SDL_RenderPresent(renderer);

        int open = 0;

        for (int i = 0; i < ROW_SIZE; i++)
        {
            for (int j = 0; j < COLUMN_SIZE; j++)
            {
                if (rectangle[i][j].state == 1)
                {
                    open += 1;
                }
            }
        }
        while (SDL_PollEvent(&mouse))
        {
            if (mouse.type == SDL_QUIT || (mouse.type == SDL_KEYDOWN && mouse.key.keysym.sym == SDLK_ESCAPE))
            {
                isRunning = false;
                quitSDL(window, renderer);
                isRunning = false;
            }

            if (mouse.type == SDL_MOUSEBUTTONDOWN)
            {
                int X;
                int Y;
                SDL_GetMouseState(&X, &Y);
                if (X < 0 || X > SCREEN_WIDTH || Y < 0 || Y > SCREEN_HEIGHT) continue;
                    int i;
                    int j;
                    i = Y / CELL_SIZE;
                    j = X / CELL_SIZE;
                if (mouse.button.button == SDL_BUTTON_RIGHT)
                {
                    if (rectangle[i][j].state == 1) break;
                    if (rectangle[i][j].state == 2)
                    {
                        rectangle[i][j].state = 0;
                        drawPic(renderer, i, j, gallery->getImage(PIC_RECTANGLE), CELL_SIZE);
                    }
                    else
                    {
                        rectangle[i][j].state = 2;
                        drawPic(renderer, i, j, gallery->getImage(PIC_FLAG), CELL_SIZE);
                    }
                }


                if (mouse.button.button == SDL_BUTTON_LEFT)
                {
                    if (rectangle[i][j].state != 2)
                    {
                        rectangle[i][j].state = 1;
                        if (rectangle[i][j].isBomb == 1)
                        {
                            printBomb(renderer, rectangle, i, j, ROW_SIZE, COLUMN_SIZE, CELL_SIZE);
                            Mix_PlayChannel(-1, boom, 0);

                            isRunning = false;
                        }
                        if (rectangle[i][j].isBomb == 0 && countOne(rectangle, i, j, ROW_SIZE, COLUMN_SIZE) != 0)
                        {
                            print(renderer, rectangle, i, j, ROW_SIZE, COLUMN_SIZE, CELL_SIZE);
                        }
                        if (rectangle[i][j].isBomb == 0 && countOne(rectangle, i, j, ROW_SIZE, COLUMN_SIZE) == 0)
                        {
                            openEmpty(renderer, rectangle, i, j, ROW_SIZE, COLUMN_SIZE, CELL_SIZE);
                        }
                    }
                }
            }
                SDL_RenderPresent(renderer);
        }
    }

    waitUntilKeyPressed();
    for (int i = 0; i < ROW_SIZE; i++){
        delete[] rectangle[i];
    }
    delete[] rectangle;

//    SDL_Delay(2000000);

    delete gallery;
    quitSDL(window, renderer);
    return 0;
}


