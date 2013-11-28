/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//The headers
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
#include <iostream>
#include "Personaje.h"
#include "SDL_mixer.h"
using namespace std;

//Screen attributes
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
const int SCREEN_BPP = 32;

//The surfaces
SDL_Surface *background = NULL;
SDL_Surface *upMessage = NULL;
SDL_Surface *downMessage = NULL;
SDL_Surface *leftMessage = NULL;
SDL_Surface *rightMessage = NULL;
SDL_Surface *message = NULL;
SDL_Surface *screen = NULL;
Mix_Music *music = NULL;
//The event structure
SDL_Event event;

//The font
TTF_Font *font = NULL;

//The color of the font
SDL_Color textColor = { 0, 0, 0 };

SDL_Surface *load_image( std::string filename )
{
    return IMG_Load( filename.c_str() );
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Press an Arrow Key", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the background image
    background = load_image( "background.png" );

    //Open the font
    font = TTF_OpenFont( "lazy.ttf", 72 );

    //If there was a problem in loading the background
    if( background == NULL )
    {
        return false;
    }

    //If there was an error in loading the font
    if( font == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void clean_up()
{
    //Free the surfaces
    SDL_FreeSurface( background );
    SDL_FreeSurface( upMessage );
    SDL_FreeSurface( downMessage );
    SDL_FreeSurface( leftMessage );
    SDL_FreeSurface( rightMessage );

    //Close the font
    TTF_CloseFont( font );

    //Quit SDL_ttf
    TTF_Quit();

    //Quit SDL
    SDL_Quit();
}

bool puedoLLegar(char tablero[8][8],int x_actual, int y_actual,int pasos, int x_final, int y_final)
{
    //Casos base
    if(pasos<0)
        return false;

    if(x_actual>=8
       || y_actual>=8
       || x_actual<0
       || y_actual<0)
       return false;

    if(tablero[y_actual][x_actual]=='#')
       return false;

    if(x_actual==x_final
       && y_actual==y_final)
       return true;

    if(tablero[y_actual][x_actual]=='O')
        pasos--;

    if(tablero[y_actual][x_actual]=='V')
        pasos++;
      if(tablero[y_actual][x_actual]=='V'
           && pasos<=0)
            pasos=1;
    pasos--;



    return puedoLLegar(tablero,x_actual+1,y_actual,pasos,x_final,y_final)
            || puedoLLegar(tablero,x_actual-1,y_actual,pasos,x_final,y_final)
            || puedoLLegar(tablero,x_actual,y_actual+1,pasos,x_final,y_final)
            || puedoLLegar(tablero,x_actual,y_actual-1,pasos,x_final,y_final);
}

void marcar(char tablero[8][8],char tablero_de_pasos[8][8],char ataca[8][8],int x_actual, int y_actual,int pasos)
{
    //Casos base
    if(pasos<0)
        return;

    if(x_actual>=8
       || y_actual>=8
       || x_actual<0
       || y_actual<0)
       return;

    if(tablero[y_actual][x_actual]=='#')
       return;

    tablero_de_pasos[y_actual][x_actual]='P';
    if (pasos>=2&&tablero[y_actual][x_actual]==' ')
        ataca[y_actual][x_actual]='A';

    if(tablero[y_actual][x_actual]=='O')
        pasos--;

    if(tablero[y_actual][x_actual]=='V')
        pasos++;
    if(tablero[y_actual][x_actual]=='V'
           && pasos<=0)
            pasos=1;
    pasos--;



    marcar(tablero,tablero_de_pasos,ataca,x_actual+1,y_actual,pasos);
    marcar(tablero,tablero_de_pasos,ataca,x_actual-1,y_actual,pasos);
    marcar(tablero,tablero_de_pasos,ataca,x_actual,y_actual+1,pasos);
    marcar(tablero,tablero_de_pasos,ataca,x_actual,y_actual-1,pasos);
}




void limpiar(char tablero_de_pasos1[8][8],char ataca[8][8])
{

    for(int x=0;x<8;x++)
        for(int y=0;y<8;y++){
            tablero_de_pasos1[y][x]=' ';
            ataca[y][x]=' ';
        }
}

void limpiarataca(char ataca[8][8])
{

    for(int x=0;x<8;x++)
        for(int y=0;y<8;y++){
            ataca[y][x]=' ';
        }
}
int main( int argc, char* args[] )
{    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return 1;
    }

    music = Mix_LoadMUS( "01 - The Legend of Zelda - Main Theme.mp3" );

    if( music == NULL )
    {
        return 1;
    }

  //  bool first_time = true, first_it = true;
      Mix_PlayMusic(music, -1);

    //Quit flag
    bool quit = false;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }

    //Generate the message surfaces
    upMessage = TTF_RenderText_Solid( font, "Up was pressed.", textColor );
    downMessage = TTF_RenderText_Solid( font, "Down was pressed.", textColor );
    leftMessage = TTF_RenderText_Solid( font, "Left was pressed", textColor );
    rightMessage = TTF_RenderText_Solid( font, "Right was pressed", textColor );



    SDL_Surface* pasillo = load_image("tablero/pasillo.png");
    SDL_Surface* muro = load_image("tablero/muro.png");
    SDL_Surface* cursor = load_image("tablero/cursor.png");
    SDL_Surface* obstaculo = load_image("tablero/obstaculo.png");
    SDL_Surface* ventaja = load_image("tablero/ventaja.png");
    SDL_Surface* ATACAR = load_image("tablero/ATACAR.png");
     SDL_Surface*Vida = load_image("life.png");
     SDL_Surface*Gandor1 = load_image("jugador1.png");
     SDL_Surface*Gandor2  = load_image("jugador2.png");
      SDL_Surface*Gandor3  = load_image("jugador3.png");
       SDL_Surface*Gandor4  = load_image("jugador4.png");
    char tablero[8][8]={{' ',' ',' ','#','#',' ',' ',' '},
                        {' ',' ',' ','#','#',' ',' ',' '},
                        {' ',' ','V','#','#','V',' ',' '},
                       {'#','#',' ',' ',' ',' ','#','#'},
                        {'#','#','V',' ',' ',' ','#','#'},
                        {' ',' ',' ','#','#','V',' ',' '},
                        {' ',' ',' ','#','#',' ',' ',' '},
                        {' ',' ',' ','#','#',' ',' ',' '}};

    char tablero_de_pasos1[8][8]={{' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '}};
     char tablero_de_pasos2[8][8]={{' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '}};
     char tablero_de_pasos3[8][8]={{' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '}};

    char tablero_de_pasos4[8][8]={{' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '}};
     char ataca1[8][8]={{' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '}};
       char ataca2[8][8]={{' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '}};
                char ataca3[8][8]={{' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '}};
                char ataca4[8][8]={{' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '},
                                {' ',' ',' ',' ',' ',' ',' ',' '}};

    SDL_Surface* pasos_surface = load_image("pasos.png");
    SDL_Surface* personaje_surface = load_image("Personaje.png");
    Personaje personaje(0,0,personaje_surface);

    SDL_Surface* personaje_surface2 = load_image("MAGO.png");
    Personaje MAGO(0,7,personaje_surface2);

    SDL_Surface* personaje_surface3 = load_image("angel.png");
    Personaje angel(7,0,personaje_surface3);

    SDL_Surface* personaje_surface4 = load_image("tanque.png");
    Personaje tanque(7,7,personaje_surface4);

    SDL_Surface* turno1 = load_image("turno1.png");
    SDL_Surface* turno2 = load_image("turno2.png");
    SDL_Surface* partida = load_image("partida.png");
    SDL_Surface* ganadas = load_image("punto_partida.png");
    SDL_Surface* opciones_de_juego = load_image("modo_de_juego.png");
    SDL_Surface* cursor_opciones = load_image("cursor_menu.png");
    SDL_Surface* cursor_play1 = load_image("cursor_play1.png");
    SDL_Surface* cursor_play2 = load_image("cursor_play2.png");
    SDL_Surface* cursor_play3 = load_image("cursor_play3.png");
    SDL_Surface* cursor_play4 = load_image("cursor_play4.png");
    int cursor_y=0;
    int cursor_x=0;
    int cursor_opciones_y=280;
    int cursor_opciones_x=0;
    int gandas1=0;
    int gandas2=0;
    int gandas3=0;
    int gandas4=0;
    int turno=0;
    int total=0;
    int vida_jugador1=3;
    int vida_jugador2=3;
    int vida_jugador3=3;
    int vida_jugador4=3;
    bool termino=false;
    bool reset=false;
    bool menu=true;


    //While the user hasn't quit
    while( quit == false )
    {
        //If there's an event to handle
        if( SDL_PollEvent( &event ) )
        {
            //If a key was pressed
            if( event.type == SDL_KEYDOWN )
            {
                //Set the proper message surface

                switch( event.key.keysym.sym )
                {
                    case SDLK_SPACE:
                        if (termino==false){
                        if (turno==0){

                        if (ataca1[MAGO.y][MAGO.x]=='A')
                        vida_jugador2--;
                        else if (ataca1[angel.y][angel.x]=='A')
                        vida_jugador3--;
                        else if (ataca1[tanque.y][tanque.x]=='A')
                        vida_jugador4--;

                        turno++;
                        }else if (turno==1){

                        if (ataca2[personaje.y][personaje.x]=='A')
                        vida_jugador1--;
                        else if (ataca2[angel.y][angel.x]=='A')
                        vida_jugador3--;
                        else if (ataca2[tanque.y][tanque.x]=='A')
                        vida_jugador4--;
                        turno++;
                        }else if (turno==2){

                        if (ataca3[personaje.y][personaje.x]=='A')
                        vida_jugador1--;
                        else if (ataca3[MAGO.y][MAGO.x]=='A')
                        vida_jugador2--;
                        else if (ataca3[tanque.y][tanque.x]=='A')
                        vida_jugador4--;
                        turno++;

                        }else if (turno==3){

                         if (ataca4[personaje.y][personaje.x]=='A')
                        vida_jugador1--;
                        else if (ataca4[MAGO.y][MAGO.x]=='A')
                        vida_jugador2--;
                        else if (ataca4[angel.y][angel.x]=='A')
                        vida_jugador3--;
                        turno++;

                        }


                        }
                    break;
                    case SDLK_UP:
                        cursor_y--;
                    break;
                    case SDLK_DOWN:
                        cursor_y++;
                    break;
                    case SDLK_LEFT:
                        if (menu==true)
                        cursor_opciones_x--;
                        if (menu==false)
                        cursor_x--;
                    break;
                    case SDLK_RIGHT:
                        if (menu==true)
                        cursor_opciones_x++;
                        if (menu==false)
                        cursor_x++;
                    break;
                    case SDLK_RETURN:

                        if(menu==false){
                        if(puedoLLegar(tablero,personaje.x,personaje.y,3,cursor_x,cursor_y)&&turno==0)
                        {   personaje.x=cursor_x;
                            personaje.y=cursor_y;
                            limpiar(tablero_de_pasos1,ataca1);
                            marcar(tablero,tablero_de_pasos1,ataca1,personaje.x,personaje.y,3);
                            turno++;

                        }else if(puedoLLegar(tablero,MAGO.x,MAGO.y,3,cursor_x,cursor_y)&&turno==1)
                        {   MAGO.x=cursor_x;
                            MAGO.y=cursor_y;
                            limpiar(tablero_de_pasos2,ataca2);
                            marcar(tablero,tablero_de_pasos2,ataca2,MAGO.x,MAGO.y,3);
                            turno++;

                        }else if(puedoLLegar(tablero,angel.x,angel.y,3,cursor_x,cursor_y)&&turno==2)
                        {   angel.x=cursor_x;
                            angel.y=cursor_y;
                            limpiar(tablero_de_pasos3,ataca3);
                            marcar(tablero,tablero_de_pasos3,ataca3,angel.x,angel.y,3);
                            turno++;

                        }else if(puedoLLegar(tablero,tanque.x,tanque.y,3,cursor_x,cursor_y)&&turno==3)
                        {   tanque.x=cursor_x;
                            tanque.y=cursor_y;
                            limpiar(tablero_de_pasos4,ataca4);
                            marcar(tablero,tablero_de_pasos4,ataca4,tanque.x,tanque.y,3);
                            turno++;

                        }

                        }
                        if (menu==true){
                        menu=false;
                       }
                    break;

                        }
            }

            //If the user has Xed out the window
            else if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }



        //Apply the background

        //If a message needs to be displayed
        if( message != NULL )
        {
            //Apply the background to the screen
            apply_surface( 0, 0, background, screen );

            //Apply the message centered on the screen
            apply_surface( ( SCREEN_WIDTH - message->w ) / 2, ( SCREEN_HEIGHT - message->h ) / 2, message, screen );

            //Null the surface pointer
            message = NULL;
        }
        if (vida_jugador1<=0){

        limpiar(tablero_de_pasos1,ataca1);
        personaje.x=8;
        }else if (vida_jugador2<=0){

        limpiar(tablero_de_pasos2,ataca2);
        MAGO.x=8;
        }else if (vida_jugador3<=0){

         limpiar(tablero_de_pasos3,ataca3);
        angel.x=8;
        } else if (vida_jugador4<=0){

        limpiar(tablero_de_pasos4,ataca4);
         angel.x=8;
        }
          if (gandas1==3){
        termino=true;
        apply_surface(0,0,Gandor1,screen);
        }else if (gandas2==3){
           termino=true;
        apply_surface(0,0,Gandor2,screen);
        }else if (gandas3==3){
            termino=true;
        apply_surface(0,0,Gandor3,screen);
        }else if (gandas4==3){
            termino=true;
        apply_surface(0,0,Gandor4,screen);
        }

        if (menu==true&&termino==false){
        apply_surface(0,0,opciones_de_juego,screen);
        apply_surface((cursor_opciones_x*219),cursor_opciones_y,cursor_opciones,screen);
        }
        if (menu==false&&termino==false){
        for(int x=0;x<10;x++)
            for(int y=0;y<10;y++)
            {
                if(tablero[y][x]==' ')
                    apply_surface(x*75,y*75,pasillo,screen);
                if(tablero[y][x]=='#')
                    apply_surface(x*75,y*75,muro,screen);
                if(tablero[y][x]=='O')
                    apply_surface(x*75,y*75,obstaculo,screen);
                if(tablero[y][x]=='V')
                    apply_surface(x*75,y*75,ventaja,screen);

            }

        apply_surface(250,0,partida,screen);

        if (cursor_opciones_x==0){
        for(int x=0;x<8;x++){
            for(int y=0;y<8;y++){
            if(tablero_de_pasos1[y][x]=='P'){
            apply_surface(x*75,y*75,pasos_surface,screen);
            }
             if(ataca1[y][x]=='A'){
            apply_surface(x*75,y*75,ATACAR,screen);
             }
            }
         }
          for(int x=0;x<8;x++){
            for(int y=0;y<8;y++){
            if(tablero_de_pasos2[y][x]=='P'){
            apply_surface(x*75,y*75,pasos_surface,screen);
            }
             if(ataca2[y][x]=='A'){
            apply_surface(x*75,y*75,ATACAR,screen);
             }
            }
         }
        personaje.dibujar(screen);
        MAGO.dibujar(screen);
         for(int x=0;x<vida_jugador1;x++){
            apply_surface(x*20,0,Vida,screen);
         }
         for(int x=0;x<vida_jugador2;x++){
            apply_surface((x*20)+520,0,Vida,screen);
         }


        for(int x=0;x<gandas1;x++){
            apply_surface((x*35)+154,0,ganadas,screen);
         }
         for(int x=0;x<gandas2;x++){
            apply_surface((x*35)+350,0,ganadas,screen);
         }
         if (turno>1)
            turno=0;
        if (vida_jugador1==0&&gandas1<3&&gandas2<3){
            gandas2++;
            reset=true;

        }else if (vida_jugador2==0&&gandas1<3&&gandas2<3){
            gandas1++;
            reset=true;

        }
        if (turno==0)
        apply_surface(cursor_x*75,cursor_y*75,cursor_play1,screen);
        else if (turno==1)
        apply_surface(cursor_x*75,cursor_y*75,cursor_play2,screen);


        }
        if (cursor_opciones_x==1){
         for(int x=0;x<8;x++){
            for(int y=0;y<8;y++){
            if(tablero_de_pasos1[y][x]=='P'){
            apply_surface(x*75,y*75,pasos_surface,screen);
            }
             if(ataca1[y][x]=='A'){
            apply_surface(x*75,y*75,ATACAR,screen);
             }
            }
         }
          for(int x=0;x<8;x++){
            for(int y=0;y<8;y++){
            if(tablero_de_pasos2[y][x]=='P'){
            apply_surface(x*75,y*75,pasos_surface,screen);
            }
             if(ataca2[y][x]=='A'){
            apply_surface(x*75,y*75,ATACAR,screen);
             }
            }
         }
          for(int x=0;x<8;x++){
            for(int y=0;y<8;y++){
            if(tablero_de_pasos3[y][x]=='P'){
            apply_surface(x*75,y*75,pasos_surface,screen);
            }
             if(ataca3[y][x]=='A'){
            apply_surface(x*75,y*75,ATACAR,screen);
             }
            }
         }
        personaje.dibujar(screen);
        MAGO.dibujar(screen);
        angel.dibujar(screen);
         for(int x=0;x<vida_jugador1;x++){
            apply_surface(x*20,0,Vida,screen);
         }
         for(int x=0;x<vida_jugador2;x++){
            apply_surface((x*20)+520,0,Vida,screen);
         }
          for(int x=0;x<vida_jugador3;x++){
            apply_surface((x*20),35,Vida,screen);
         }

         for(int x=0;x<gandas1;x++){
            apply_surface((x*35)+154,0,ganadas,screen);
         }
         for(int x=0;x<gandas2;x++){
            apply_surface((x*35)+350,0,ganadas,screen);
         }
         for(int x=0;x<gandas3;x++){
            apply_surface((x*35)+154,35,ganadas,screen);
         }


         if (vida_jugador1==0&&vida_jugador2==0&&gandas1<3&&gandas2<3&&gandas3<3){
            gandas3++;
            reset=true;

        }else if (vida_jugador1==0&&vida_jugador3==0&&gandas1<3&&gandas2<3&&gandas3<3){
            gandas2++;
            reset=true;

        }else if (vida_jugador2==0&&vida_jugador3==0&&gandas1<3&&gandas2<3&&gandas3<3){
            gandas1++;
            reset=true;

        }
         if (turno>2)
            turno=0;
         if (turno==0)
        apply_surface(cursor_x*75,cursor_y*75,cursor_play1,screen);
        else if (turno==1)
        apply_surface(cursor_x*75,cursor_y*75,cursor_play2,screen);
        else if (turno==2)
        apply_surface(cursor_x*75,cursor_y*75,cursor_play3,screen);

        }
        if (cursor_opciones_x==2){
             for(int x=0;x<8;x++){
            for(int y=0;y<8;y++){
            if(tablero_de_pasos1[y][x]=='P'){
            apply_surface(x*75,y*75,pasos_surface,screen);
            }
             if(ataca1[y][x]=='A'){
            apply_surface(x*75,y*75,ATACAR,screen);
             }
            }
         }
          for(int x=0;x<8;x++){
            for(int y=0;y<8;y++){
            if(tablero_de_pasos2[y][x]=='P'){
            apply_surface(x*75,y*75,pasos_surface,screen);
            }
             if(ataca2[y][x]=='A'){
            apply_surface(x*75,y*75,ATACAR,screen);
             }
            }
         }
          for(int x=0;x<8;x++){
            for(int y=0;y<8;y++){
            if(tablero_de_pasos3[y][x]=='P'){
            apply_surface(x*75,y*75,pasos_surface,screen);
            }
             if(ataca3[y][x]=='A'){
            apply_surface(x*75,y*75,ATACAR,screen);
             }
            }
         }
        for(int x=0;x<8;x++){
            for(int y=0;y<8;y++){
            if(tablero_de_pasos4[y][x]=='P'){
            apply_surface(x*75,y*75,pasos_surface,screen);
            }
             if(ataca4[y][x]=='A'){
            apply_surface(x*75,y*75,ATACAR,screen);
             }
            }
         }
         for(int x=0;x<vida_jugador1;x++){
            apply_surface(x*20,0,Vida,screen);
         }
         for(int x=0;x<vida_jugador2;x++){
            apply_surface((x*20)+520,0,Vida,screen);
         }
          for(int x=0;x<vida_jugador3;x++){
            apply_surface((x*20),35,Vida,screen);
         }
          for(int x=0;x<vida_jugador4;x++){
            apply_surface((x*20)+520,35,Vida,screen);
         }
        for(int x=0;x<gandas1;x++){
            apply_surface((x*35)+154,0,ganadas,screen);
         }
         for(int x=0;x<gandas2;x++){
            apply_surface((x*35)+350,0,ganadas,screen);
         }
         for(int x=0;x<gandas3;x++){
            apply_surface((x*35)+154,35,ganadas,screen);
         }
         for(int x=0;x<gandas4;x++){
            apply_surface((x*35)+350,35,ganadas,screen);
         }

        personaje.dibujar(screen);
        MAGO.dibujar(screen);
        angel.dibujar(screen);
        tanque.dibujar(screen);
        if (vida_jugador1==0&&vida_jugador2==0&&vida_jugador3==0&&gandas1<3&&gandas2<3&&gandas3<3&&gandas4<3){
            reset=true;
            gandas4++;

        }else if (vida_jugador1==0&&vida_jugador2==0&&vida_jugador4==0&&gandas1<3&&gandas2<3&&gandas3<3&&gandas4<3){
            gandas3++;
            reset=true;

        }else if (vida_jugador1==0&&vida_jugador3==0&&vida_jugador4==0&&gandas1<3&&gandas2<3&&gandas3<3&&gandas4<3){
            gandas2++;
            reset=true;

        }else if (vida_jugador2==0&&vida_jugador3==0&&vida_jugador4==0&&gandas1<3&&gandas2<3&&gandas3<3&&gandas4<3){
            gandas1++;
            reset=true;

        }
        if (turno>3)
            turno=0;
        }
        if (turno==0)
        apply_surface(cursor_x*75,cursor_y*75,cursor_play1,screen);
        else if (turno==1)
        apply_surface(cursor_x*75,cursor_y*75,cursor_play2,screen);
        else if (turno==2)
        apply_surface(cursor_x*75,cursor_y*75,cursor_play3,screen);
        else if (turno==3)
        apply_surface(cursor_x*75,cursor_y*75,cursor_play4,screen);

        }

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
    if (cursor_x>7)
        cursor_x=7;
     if (cursor_y>7)
        cursor_y=7;
    if (cursor_x<0)
        cursor_x=0;
     if (cursor_y<0)
        cursor_y=0;


        if (reset==true){
            if (cursor_opciones_x==1){
            limpiar(tablero_de_pasos1,ataca1);
            limpiar(tablero_de_pasos2,ataca2);
            limpiar(tablero_de_pasos3,ataca3);
             reset=false;
             personaje.x=0;
             personaje.y=0;
             MAGO.x=0;
             MAGO.y=7;
             angel.x=7;
             angel.y=0;
             vida_jugador1=3;
            vida_jugador2=3;
            vida_jugador3=3;
             termino=false;
            }else if (cursor_opciones_x==2){
            limpiar(tablero_de_pasos1,ataca1);
            limpiar(tablero_de_pasos2,ataca2);
            limpiar(tablero_de_pasos3,ataca3);
            limpiar(tablero_de_pasos4,ataca4);
             reset=false;
             personaje.x=0;
             personaje.y=0;
             MAGO.x=0;
             MAGO.y=7;
             tanque.x=7;
             tanque.y=7;
             angel.x=7;
             angel.y=0;
             vida_jugador1=3;
            vida_jugador2=3;
            vida_jugador3=3;
            vida_jugador4=3;
             termino=false;
            }else if (cursor_opciones_x==0){
            limpiar(tablero_de_pasos1,ataca1);
            limpiar(tablero_de_pasos2,ataca2);
             reset=false;
             personaje.x=0;
             personaje.y=0;
             MAGO.x=0;
             MAGO.y=7;
             vida_jugador1=3;
            vida_jugador2=3;
             termino=false;
        }
        }


        if (cursor_opciones_x>2)
            cursor_opciones_x=0;
        if (cursor_opciones_x==-1)
            cursor_opciones_x=0;

    }

    //Clean up
    clean_up();

    return 0;
}
