#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>


#include "SDL_utils.h"
//#include "Game.h"
#include "Gallery.h"

using namespace std;

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 600;
const char WINDOW_TITLE[] = "Minesweeper Game";

Gallery* gallery = nullptr; // global picture manager

void renderSplashScreen();

int main(int argc, char* argv[])
{
    srand(time(0));
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    gallery = new Gallery(renderer);
//    Game game(BOARD_WIDTH, BOARD_HEIGHT);
//    SDL_Event e;

    SDL_Delay(2000);
    delete gallery;
    quitSDL(window, renderer);
    return 0;
}

void renderSplashScreen()
{
    cout << "Press any key to start game" << endl;
    waitUntilKeyPressed();
}

