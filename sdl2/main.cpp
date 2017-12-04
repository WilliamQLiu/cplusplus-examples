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
#include <string>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

// Key press surfaces constants
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,  // default count starts at 0
    KEY_PRESS_SURFACE_UP,  // so this is 1
    KEY_PRESS_SURFACE_DOWN,  // this is 2, etc.
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

// Start up SDL and create the window and surface
bool init();

// Load the media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// Loads individual image
SDL_Surface* loadSurface( std:: string path);

// The window we'll be rendering to
SDL_Window* gWindow = NULL;  // make sure to have pointers point to NULL if not pointing to anything

// The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

// The images that correspond to a keypress
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];

// Current displayed image
SDL_Surface* gCurrentSurface = NULL;

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

    // Load default surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface("resources/press.bmp");
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL )
    {
        printf("Failed to load default image!\n");
        success = false;
    }

    // Load up surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = loadSurface("resources/up.bmp");
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL )
    {
        printf("Failed to load default image!\n");
        success = false;
    }

    // Load down surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = loadSurface("resources/down.bmp");
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL )
    {
        printf("Failed to load down image!\n");
        success = false;
    }

    // Load left surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = loadSurface("resources/left.bmp");
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL )
    {
        printf("Failed to load left image!\n");
        success = false;
    }

    // Load left surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = loadSurface("resources/right.bmp");
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL )
    {
        printf("Failed to load right image!\n");
        success = false;
    }

    return success;
}

void close()
{
    // Deallocate surfaces
    for( int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i )
    {
        SDL_FreeSurface( gKeyPressSurfaces[i] );
        gKeyPressSurfaces[i] = NULL;
    }

    // Destroy the window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Shut down SDL_Image
    IMG_Quit();

    // Quit SDL subsystems
    SDL_Quit();

}

SDL_Surface* loadSurface( std::string path)
{
    // Load image at the specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf(" Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }
    return loadedSurface;
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

            // Set default current surface
            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];

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
                    else if( e.type == SDL_KEYDOWN )
                    {
                        // Select surfaces based on key press
                        switch( e.key.keysym.sym )
                        {
                            case SDLK_UP:
                            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
                            break;

                            case SDLK_DOWN:
                            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
                            break;

                            case SDLK_LEFT:
                            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
                            break;

                            case SDLK_RIGHT:
                            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
                            break;

                            default:
                            gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
                            break;


                        }
                    }
                }

                // Apply the image
                SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

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