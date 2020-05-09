#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "SDL_utils.h"
#include "gallery.h"
#include "game.h"
#include "fft.h"

using namespace std;

const int SCREEN_WIDTH = 450;
const int SCREEN_HEIGHT = 600;
const char WINDOW_TITLE[] = "MINESWEEPER GAME";

const int CELL_SIZE = 30;
const int ROW_SIZE = 15;
const int COLUMN_SIZE = 15;
const int numberOfBoom = 70;
const int backGround = 75;

const SDL_Color RED = {255, 0, 0};

Gallery* gallery = nullptr;

void playGame(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, Mix_Chunk *boom);

void quitGame(level **rectangle, const int ROW_SIZE, const int COLUMN_SIZE);

bool press(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, Mix_Chunk *boom);

int main(int argc, char* argv[])
{
    srand(time(0));
    SDL_Window* window;
    SDL_Renderer* renderer;

    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);


    Mix_Chunk *boom;
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError();
        return -1;
    }
    boom = Mix_LoadWAV("boom.wav");

    TTF_Font* font = nullptr;

    SDL_SetRenderDrawColor(renderer, 224, 224, 224, 0);

    SDL_RenderClear(renderer);

    initTTF(renderer);
    font = TTF_OpenFont("VeraMoBd.ttf", 50);
    rendererTTF(renderer, font, "  70 BOOM", 60, 200, RED);

    font = TTF_OpenFont("VeraMoBd.ttf", 37);
    rendererTTF(renderer, font, "PRESS TAB TO START", 20, 350, RED);

    SDL_RenderPresent(renderer);

    gallery = new Gallery(renderer);

    while (press(window, renderer, font, boom))
    {
        playGame(renderer, window, font, boom);
    }

    waitUntilKeyPressed();

    closeTTF(font);

    delete gallery;

    quitSDL(window, renderer);

    return 0;
}

void playGame(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, Mix_Chunk *boom)
{
    bool win = false;

    bool isRunning = true;

    level** rectangle = new level* [ROW_SIZE];

    for (int i = 0; i < ROW_SIZE; i++)
    {
        rectangle[i] = new level [COLUMN_SIZE];
    }
    for (int i = 0; i < ROW_SIZE; i++)
    {
        for (int j = 0; j < COLUMN_SIZE; j++)
        {
            rectangle[i][j].isBoom = 0;

            rectangle[i][j].state = 0;
        }
    }

    drawBlocks(renderer, rectangle, ROW_SIZE, COLUMN_SIZE, CELL_SIZE, backGround);

    setRandBoom(rectangle, ROW_SIZE, COLUMN_SIZE, numberOfBoom);

    for (int i = 0; i < ROW_SIZE; i++)
    {
        for (int j = 0; j < COLUMN_SIZE; j++)
        {
            if (rectangle[i][j].isBoom == 0)
            {
                cout << countOne(rectangle, i, j, ROW_SIZE, COLUMN_SIZE) << " ";
            }
            if (rectangle[i][j].isBoom == 1)
            {
                cout << "*" << " ";
            }
        }
        cout << endl;
    }
    cout << endl;

    SDL_Event mouse;

    while (isRunning)
    {
        SDL_RenderPresent(renderer);

        drawPic(renderer, -1, 6, gallery->getImage(PIC_NO), CELL_SIZE, backGround);

        int openNumber = 0;

        int openFlag = 0;

        for (int i = 0; i < ROW_SIZE; i++)
        {
            for (int j = 0; j < COLUMN_SIZE; j++)
            {
                if (rectangle[i][j].state == 1)
                {
                    openNumber += 1;
                }
                if (rectangle[i][j].state == 2)
                {
                    openFlag += 1;
                }
            }
        }
        int boomLeft = numberOfBoom - openFlag;

        SDL_RenderClear(renderer);

        string write = to_string(boomLeft);

        drawPic(renderer, -1, 6, gallery->getImage(PIC_NO), CELL_SIZE, backGround);

        font = TTF_OpenFont("VeraMoBd.ttf", 40);
        rendererTTF(renderer, font, write, 10, 10, RED);

        for (int i = 0; i < ROW_SIZE; i++)
        {
            for (int j = 0; j < COLUMN_SIZE; j++)
            {
                if (rectangle[i][j].state == 0)
                {
                    drawPic(renderer, i, j, gallery->getImage(PIC_RECTANGLE), CELL_SIZE, backGround);
                }
                else if (rectangle[i][j].state == 1)
                {
                    if (countOne(rectangle, i, j, ROW_SIZE, COLUMN_SIZE) != 0)
                    {
                        printNumber(renderer, rectangle, i, j, ROW_SIZE, COLUMN_SIZE, CELL_SIZE, backGround);
                    }
                    if (countOne(rectangle, i, j, ROW_SIZE, COLUMN_SIZE) == 0)
                    {
                        openEmpty(renderer, rectangle, i, j, ROW_SIZE, COLUMN_SIZE, CELL_SIZE, backGround);
                    }
                }
                else
                {
                    drawPic(renderer, i, j, gallery->getImage(PIC_FLAG), CELL_SIZE, backGround);
                }
            }
        }

        if (openNumber == ROW_SIZE * COLUMN_SIZE - numberOfBoom)
        {
            win = true;

            isRunning = false;
        }
        while (SDL_PollEvent(&mouse))
        {
            if (mouse.type == SDL_QUIT || (mouse.type == SDL_KEYDOWN && mouse.key.keysym.sym == SDLK_ESCAPE))
            {
                quitGame(rectangle, ROW_SIZE, COLUMN_SIZE);
                isRunning = false;
                quitSDL(window, renderer);
            }

            if (mouse.type == SDL_MOUSEBUTTONDOWN)
            {
                int X;
                int Y;
                SDL_GetMouseState(&X, &Y);
                if (X < 0 || X > SCREEN_WIDTH || Y < 0 || Y > SCREEN_HEIGHT) continue;
                    int i;
                    int j;
                    i = (Y - backGround) / CELL_SIZE;
                    j = X / CELL_SIZE;
                if (mouse.button.button == SDL_BUTTON_RIGHT)
                {
                    if (rectangle[i][j].state == 1) break;

                    if (rectangle[i][j].state == 2)
                    {
                        rectangle[i][j].state = 0;
                        drawPic(renderer, i, j, gallery->getImage(PIC_RECTANGLE), CELL_SIZE, backGround);
                    }
                    else
                    {
                        rectangle[i][j].state = 2;
                        drawPic(renderer, i, j, gallery->getImage(PIC_FLAG), CELL_SIZE, backGround);
                    }
                }

                if (mouse.button.button == SDL_BUTTON_LEFT)
                {
                    if (rectangle[i][j].state != 2)
                    {
                        rectangle[i][j].state = 1;

                        if (rectangle[i][j].isBoom == 1)
                        {
                            drawPic(renderer, -1, 6, gallery->getImage(PIC_SAD), CELL_SIZE, backGround);

                            printBoom(renderer, rectangle, i, j, ROW_SIZE, COLUMN_SIZE, CELL_SIZE, backGround);

                            Mix_PlayChannel(-1, boom, 0);

                            win = false;

                            isRunning = false;

                            break;

                        }
                        if (rectangle[i][j].isBoom == 0 && countOne(rectangle, i, j, ROW_SIZE, COLUMN_SIZE) != 0)
                        {
                            drawPic(renderer, -1, 6, gallery->getImage(PIC_FUNNY), CELL_SIZE, backGround);

                            printNumber(renderer, rectangle, i, j, ROW_SIZE, COLUMN_SIZE, CELL_SIZE, backGround);
                        }
                        if (rectangle[i][j].isBoom == 0 && countOne(rectangle, i, j, ROW_SIZE, COLUMN_SIZE) == 0)
                        {
                            drawPic(renderer, -1, 6, gallery->getImage(PIC_FUNNY), CELL_SIZE, backGround);

                            openEmpty(renderer, rectangle, i, j, ROW_SIZE, COLUMN_SIZE, CELL_SIZE, backGround);
                        }
                    }
                }
            }
            SDL_RenderPresent(renderer);
        }
    }
    if (win == true)
    {
        font = TTF_OpenFont("VeraMoBd.ttf", 55);
        rendererTTF(renderer, font, "  YOU WIN", 20, 0, RED);

        font = TTF_OpenFont("VeraMoBd.ttf", 30);
        rendererTTF(renderer, font, " press TAB to play", 10, 550, RED);
        SDL_RenderPresent(renderer);
    }

    else
    {
        font = TTF_OpenFont("VeraMoBd.ttf", 55);
        rendererTTF(renderer, font, "  YOU LOSE", 20, 0, RED);

        font = TTF_OpenFont("VeraMoBd.ttf", 30);
        rendererTTF(renderer, font, "press TAB to play again", 10, 550, RED),
        SDL_RenderPresent(renderer);
    }
}

void quitGame(level **rectangle, const int ROW_SIZE, const int COLUMN_SIZE)
{
    for (int i = 0; i < ROW_SIZE; i++)
    {
        delete[] rectangle[i];
    }
    delete[] rectangle;
}

bool press(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font, Mix_Chunk *boom)
{
        bool menu = false;

        SDL_Event keyboard;

        while(menu == false)
        {
            while(SDL_PollEvent(&keyboard))
            {
                if (keyboard.type == SDL_KEYDOWN)
                {
                    if (keyboard.key.keysym.sym == SDLK_TAB)
                    {
                        menu = true;
                    }
                    if (keyboard.type == SDL_QUIT || (keyboard.type == SDL_KEYDOWN && keyboard.key.keysym.sym == SDLK_ESCAPE))
                    {
                        quitSDL(window, renderer);
                        break;
                    }
                }
            }
        }
    return menu;
}


