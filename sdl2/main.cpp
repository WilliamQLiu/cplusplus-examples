/*
To compile on Ubuntu
* sudo apt-get install --yes libsdl2-dev
* g++ main.cpp -I /usr/include/SDL2/ -lSDL2 -lGL
* Execute with ./a.out

*/

// Using SDL and Standard IO
#include <SDL.h>
#include <stdio.h>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

int main( int argc, char* args[] )
{
    // The window we'll be rendering to
    SDL_Window* window = NULL;

    // The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    // Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initalize! SDL_Error: %s\n", SDL_GetError() );
    }
}