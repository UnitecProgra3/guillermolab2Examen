#include "mago.h"

mago::mago(int x,int y,SDL_Surface*imagen)
{
    this->x=x;
    this->y=y;
    this->imagen=imagen;
}
void mago::dibujar(SDL_Surface*screen)
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x*75;
    offset.y = y*75;

    //Blit
    SDL_BlitSurface( imagen, NULL, screen, &offset );
}


mago::~mago()
{
    //dtor
}
