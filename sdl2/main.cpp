/*
To compile on Ubuntu
* sudo apt-get install --yes libsdl2-dev
* g++ main.cpp -I /usr/include/SDL2/ -lSDL2 -lGL -lSDL2_image
* Execute with ./a.out

*/

// Using SDL and Standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

// Start up SDL and create the window and surface
bool init();

// Load the media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// The window we'll be rendering to
SDL_Window* gWindow = NULL;  // make sure to have pointers point to NULL if not pointing to anything

// The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gXOut = NULL;


bool init()
{
    // Initialization Flag
    bool success = true;

    // Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initalize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        // Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if( gWindow == NULL )
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            // Get window surface
            gScreenSurface = SDL_GetWindowSurface(gWindow);
        }
    }
    return success;
}

bool loadMedia()
{
    // Loading success flag
    bool success = true;

    IMG_Init(IMG_INIT_PNG);

    // Load a splash image
    gXOut = IMG_Load("loading_image.png");
    if(gXOut == NULL)
    {
        printf("Unable to load image %s! SDL Error: %s\n", "loading_image.bmp", SDL_GetError());
        success = false;
    }
    return success;
}

void close()
{
    // Deallocate surface
    SDL_FreeSurface(gXOut);
    gXOut = NULL;

    // Destroy the window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Shut down SDL_Image
    IMG_Quit();

    // Quit SDL subsystems
    SDL_Quit();

}

int main(int argc, char* args[])
{
    // Start up SDL and create the window
    if( !init() )
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        // Load the media
        if( !loadMedia() )
        {
            printf("Failed to load media!\n");
        }
        else
        {
            bool quit = false;  // Main Loop Flag
            SDL_Event e;  // Event Handler, an event is a key press, mouse motion, joy button press

            // while application is running
            while( !quit )
            {
                // Handle events on queue by polling them for the most recent event
                while( SDL_PollEvent( &e ) != 0 )  // When SDL_PollEvent is empty, returns 0
                {
                    // User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                }

                // Apply the image
                SDL_BlitSurface(gXOut, NULL, gScreenSurface, NULL);

                // Update the surface
                SDL_UpdateWindowSurface(gWindow);

                // Wait two seconds
                //SDL_Delay(2000);

            }
        }
    }

    // Free resources and close SDL
    close();

    return 0;
}