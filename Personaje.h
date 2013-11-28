#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "SDL.h"
#include "SDL_image.h"

class Personaje
{
    public:
        int x,y;
        SDL_Surface*imagen;
        Personaje(int x,int y,SDL_Surface*personaje);
        void dibujar(SDL_Surface *screen);
        virtual ~Personaje();
    protected:
    private:
};

#endif // PERSONAJE_H
