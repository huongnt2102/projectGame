#include <SDL.h>
#include <SDL_ttf.h>

#include "fft.h"

void initTTF(SDL_Renderer *renderer)
{
    if (TTF_Init() < 0)
	{
		SDL_Log("%s", TTF_GetError());
		return;
	}
}


void rendererTTF(SDL_Renderer* renderer, SDL_Surface* fSurface, SDL_Texture* fTexture,TTF_Font* font, string text, int left, int top, const SDL_Color RED)
{
    fSurface = TTF_RenderText_Solid(font, text.c_str(), RED);

    fTexture = SDL_CreateTextureFromSurface(renderer, fSurface);

    SDL_Rect srcRest;
    SDL_Rect desRect;
    TTF_SizeText(font, text.c_str(), &srcRest.w, &srcRest.h);
    srcRest.x = 0;
	srcRest.y = 0;

	desRect.x = left;
	desRect.y = top;

	desRect.w = srcRest.w;
	desRect.h = srcRest.h;

	SDL_RenderCopy(renderer, fTexture, &srcRest, &desRect);
}

void closeTTF(TTF_Font* font)
{
    TTF_CloseFont(font);
    font = NULL;
    TTF_Quit();
}

