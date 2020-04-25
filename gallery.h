#ifndef GALLERY_H
#define GALLERY_H

#include <vector>
#include <string>
#include <SDL.h>

enum PictureID {
    PIC_BOMB,
    PIC_RECTANGLE,
    PIC_FLAG,
    PIC_0,
    PIC_1,
    PIC_2,
    PIC_3,
    PIC_4,
    PIC_5,
    PIC_6,
    PIC_7,
    PIC_8,
    PIC_COUNT
};

struct Gallery
{
    SDL_Texture* pictures[PIC_COUNT];
    SDL_Renderer* renderer;
    SDL_Texture* loadTexture(std::string path);

    Gallery(SDL_Renderer* renderer_);
    ~Gallery();

    void loadGamePictures();
    SDL_Texture* getImage(PictureID id) const { return pictures[id]; }
};

#endif // GALLERY_H

