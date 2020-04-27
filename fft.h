
#ifndef FFT_H
#define FFT_H

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>

#include "gallery.h"
#include "game.h"

using namespace std;

void initTTF(SDL_Renderer *renderer);

void rendererTTF(SDL_Renderer* renderer, SDL_Surface* fSurface, SDL_Texture* fTexture, TTF_Font* font, Uint8 r, Uint8 g, Uint8 b, Uint8 a, string text, int left, int top, const SDL_Color YELLOW);

void closeTTF(TTF_Font* font);

#endif



