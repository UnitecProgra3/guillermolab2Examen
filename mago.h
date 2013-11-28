#ifndef MAGO_H
#define MAGO_H
#include "SDL.h"
#include "SDL_image.h"

class mago
{
    public:
         int x,y;
        SDL_Surface*imagen;
        mago(int x,int y,SDL_Surface*personaje);
        void dibujar(SDL_Surface *screen);
        virtual ~mago();
    protected:
    private:
};

#endif // MAGO_H
