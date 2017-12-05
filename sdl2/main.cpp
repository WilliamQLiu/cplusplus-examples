/*
To compile on Ubuntu
* sudo apt-get install --yes libsdl2-dev
* g++ main.cpp -I /usr/include/SDL2/ -lSDL2 -lGL -lSDL2_image
* Execute with ./a.out

*/

// Using SDL and Standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>  // for sound
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

// Loads individual image (SDL_Surface is software rendering whereas SDL_Texture is hardware rendering)
SDL_Texture* loadTexture( std:: string path);

// The window we'll be rendering to
SDL_Window* gWindow = NULL;  // make sure to have pointers point to NULL if not pointing to anything

// Textures need an SDL_Renderer to render to the screen
SDL_Renderer* gRenderer = NULL;

// The images that correspond to a keypress
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];

// Current displayed texture
SDL_Texture* gTexture = NULL;

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
        // Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf("Warning: Linear texture filtering not enabled" );
        }

        // Create a window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if( gWindow == NULL )
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            // Create renderer for the window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                // Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                // Initialize PNG Loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf("SDL_image could not initialize! SDL_Image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }
    return success;
}

bool loadMedia()
{
    // Loading success flag
    bool success = true;

    // Load PNG texture
    gTexture = loadTexture( "resources/loading_image.png");
    if( gTexture == NULL )
    {
        printf("Failed to load texture image!\n");
        success = false;
    }

    return success;
}

void close()
{
    // Free loaded image
    SDL_DestroyTexture( gTexture );
    gTexture = NULL;

    // Destroy the window and renderer
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    // Shut down SDL_Image
    IMG_Quit();

    // Quit SDL subsystems
    SDL_Quit();

}

SDL_Texture* loadTexture( std::string path)
{
    // The final texture
    SDL_Texture* newTexture = NULL;

    // Load image at the specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf(" Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }
    else
    {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        // Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
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

                // Clear the screen with the color last set from SDL_SetRenderDrawColor
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );  // Set clearing color as White
                SDL_RenderClear( gRenderer );

                // CREATE VIEWPORTS
                // Create top left viewport
                SDL_Rect topLeftViewport;
                topLeftViewport.x = 0;
                topLeftViewport.y = 0;
                topLeftViewport.w = SCREEN_WIDTH / 2;
                topLeftViewport.h = SCREEN_HEIGHT / 2;
                SDL_RenderSetViewport( gRenderer, &topLeftViewport );
                SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );  // Now that screen is cleared, render the texture to screen

                // Create top right viewport
                SDL_Rect topRightViewport;
                topLeftViewport.x = SCREEN_WIDTH / 2;
                topLeftViewport.y = 0;
                topLeftViewport.w = SCREEN_WIDTH / 2;
                topLeftViewport.h = SCREEN_HEIGHT / 2;
                SDL_RenderSetViewport( gRenderer, &topLeftViewport );
                SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );  // Now that screen is cleared, render the texture to screen

                // DRAWING
                // Draw red filled quad, a solid rectangle
                SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4,SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 }; // Area we want filled with x, y, width, height
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderFillRect( gRenderer, &fillRect );  // update texture to screen

                // Draw blue horizontal line
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
                SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

                // Draw sequence of dots
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
                for( int i = 0; i < SCREEN_HEIGHT; i += 4)
                {
                    SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
                }

                // Update screen with our render
                SDL_RenderPresent( gRenderer );

                // Wait two seconds
                //SDL_Delay(2000);

            }
        }
    }

    // Free resources and close SDL
    close();

    return 0;
}