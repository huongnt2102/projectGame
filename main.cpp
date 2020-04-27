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
const int SCREEN_HEIGHT = 450;
const char WINDOW_TITLE[] = "MINESWEEPER GAME";

const int CELL_SIZE = 30;
const int ROW_SIZE = 15;
const int COLUMN_SIZE = 15;

const SDL_Color YELLOW = {255, 255, 0};

Gallery* gallery = nullptr; // global picture manager

void playGame(SDL_Renderer* renderer, SDL_Window* window, SDL_Surface* fSurface, SDL_Texture* fTexture, TTF_Font* font, Mix_Chunk *boom);

bool press(SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* fSurface, SDL_Texture* fTexture, TTF_Font* font, Mix_Chunk *boom);

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
    SDL_Surface* fSurface = nullptr;
    SDL_Texture* fTexture = nullptr;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_RenderClear(renderer);

    initTTF(renderer);
    font = TTF_OpenFont("lazy.ttf", 50);
    rendererTTF(renderer, fSurface, fTexture, font, 255, 0, 0, 255, "  70 BOOM", 60, 100, YELLOW);

    font = TTF_OpenFont("VeraMoBd.ttf", 37);
    rendererTTF(renderer, fSurface, fTexture, font, 255, 0, 0, 255, " PRESS TAB TO START", 10, 250, YELLOW);
    SDL_RenderPresent(renderer);

    gallery = new Gallery(renderer);

    while (press(window, renderer, fSurface, fTexture, font, boom))
    {
        playGame(renderer, window, fSurface, fTexture, font, boom);
    }

    waitUntilKeyPressed();

    closeTTF(font);

    delete gallery;

    quitSDL(window, renderer);

    return 0;
}

void playGame(SDL_Renderer* renderer, SDL_Window* window, SDL_Surface* fSurface, SDL_Texture* fTexture, TTF_Font* font, Mix_Chunk *boom)
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
            rectangle[i][j].isBomb = 0;

            rectangle[i][j].state = 0;
        }
    }

    drawBlocks(renderer, rectangle, ROW_SIZE, COLUMN_SIZE, CELL_SIZE);

    int numberOfBomb = 70;

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
        if (open == ROW_SIZE * COLUMN_SIZE - numberOfBomb)
        {
            win = true;

            isRunning = false;
        }
        while (SDL_PollEvent(&mouse))
        {
            if (mouse.type == SDL_QUIT || (mouse.type == SDL_KEYDOWN && mouse.key.keysym.sym == SDLK_ESCAPE))
            {
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

                            win = false;

                            isRunning = false;

                            break;

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
    if (win == true)
    {
        font = TTF_OpenFont("VeraMoBd.ttf", 55);
        rendererTTF(renderer, fSurface, fTexture, font, 255, 0, 0, 255, "  YOU WIN", 20, 120, YELLOW);

        font = TTF_OpenFont("VeraMoBd.ttf", 31);
        rendererTTF(renderer, fSurface, fTexture, font, 255, 0, 0, 255, " PRESS TAB TO PLAY AGAIN", 1, 300, YELLOW);
        SDL_RenderPresent(renderer);
    }

    else
    {
        font = TTF_OpenFont("VeraMoBd.ttf", 55);
        rendererTTF(renderer, fSurface, fTexture, font, 255, 0, 0, 255, "  YOU LOSE", 20, 120, YELLOW);

        font = TTF_OpenFont("VeraMoBd.ttf", 31);
        rendererTTF(renderer, fSurface, fTexture, font, 255, 0, 0, 255, "press TAB to play again", 1, 300, YELLOW),
        SDL_RenderPresent(renderer);
    }
}

bool press(SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* fSurface, SDL_Texture* fTexture, TTF_Font* font, Mix_Chunk *boom)
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


