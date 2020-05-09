#include <SDL.h>
#include <SDL_image.h>

#include "gallery.h"
#include "SDL_utils.h"


Gallery::Gallery(SDL_Renderer* renderer_)
    : renderer(renderer_)
{
    loadGamePictures();
}

Gallery::~Gallery()
{
    for (SDL_Texture* texture : pictures)
        SDL_DestroyTexture(texture);
}

SDL_Texture* Gallery::loadTexture(std::string path )
{
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if ( loadedSurface == NULL )
        logSDLError(std::cout, "Unable to load image " + path + " SDL_image Error: " + IMG_GetError());
    else {
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == NULL )
            logSDLError(std::cout, "Unable to create texture from " + path + " SDL Error: " + SDL_GetError());
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}

void Gallery::loadGamePictures()
{
    pictures[PIC_BOOM] = loadTexture("IMAGE\\boom.png");

    pictures[PIC_RECTANGLE] = loadTexture("IMAGE\\rectangle.png");

    pictures[PIC_FLAG] = loadTexture("IMAGE\\flag.png");

    pictures[PIC_0] = loadTexture("IMAGE\\0.png");

    pictures[PIC_1] = loadTexture("IMAGE\\1.png");

    pictures[PIC_2] = loadTexture("IMAGE\\2.png");

    pictures[PIC_3] = loadTexture("IMAGE\\3.png");

    pictures[PIC_4] = loadTexture("IMAGE\\4.png");

    pictures[PIC_5] = loadTexture("IMAGE\\5.png");

    pictures[PIC_6] = loadTexture("IMAGE\\6.png");

    pictures[PIC_7] = loadTexture("IMAGE\\7.png");

    pictures[PIC_8] = loadTexture("IMAGE\\8.png");

    pictures[PIC_NO] = loadTexture("IMAGE\\no.png");

    pictures[PIC_SAD] = loadTexture("IMAGE\\sad.png");

    pictures[PIC_FUNNY] = loadTexture("IMAGE\\funny.png");
}


