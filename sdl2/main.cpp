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

// Texture wrapper class
class LTexture
{
    public:
        // Initialize variables
        LTexture();

        // Deallocates memory
        ~LTexture();

        // Load image at a specified path
        bool loadFromFile( std::string path );

        // Deallocates texture
        void free();

        // Renders texture at given point
        void render( int x, int y , SDL_Rect* clip = NULL );  // accepts a rect for which part to render / clip

        // Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        // The actual hardware texture
        SDL_Texture* mTexture;

        // Image dimensions
        int mWidth;
        int mHeight;
};

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

// Scene Textures using LTexture
LTexture gFooTexture;
LTexture gBackgroundTexture;

// Create some spirtes
SDL_Rect gSpriteClips[ 4 ];
LTexture gSpriteSheetTexture;

LTexture::LTexture()
{
    // This Constructor initializes variables
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    // This Destructor deallocates
    free();
}

bool LTexture::loadFromFile( std::string path )
{
    // Get rid of preexisting texture if it exists
    free();

    // Final Texture
    SDL_Texture* newTexture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf(" Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        // Color key image (i.e. pick a color to make transparent )
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ));  // surface, enable color keying, and the pixel we want to color key with (using cyan here)

        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            // Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    // Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::free()
{
    // Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render( int x, int y, SDL_Rect* clip )
{
    // Render a texture, with optional clip param to see what parts to clip

    // Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    // Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopy( gRenderer, mTexture, clip, &renderQuad );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

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

    // Load Foo texture
    //if( !gFooTexture.loadFromFile( "resources/foo.png" ))
    //{
    //    printf("Failed to load Foo texture image!\n" );
    //    success = false;
    //}

    //// Load background texture
    //if( !gBackgroundTexture.loadFromFile("resources/background.png"))
    //{
    //    printf("Failed to load background texture image!\n" );
    //    success = false;
    //}

    // Load PNG texture
    //gTexture = loadTexture( "resources/loading_image.png");
    //if( gTexture == NULL )
    //{
    //    printf("Failed to load texture image!\n");
    //    success = false;
    //}

    // Load Sprite Sheet Texture
    if( !gSpriteSheetTexture.loadFromFile( "resources/dots.png" ) )
    {
        printf("Failed to load sprite sheet texture!\n" );
        success = false;
    }
    else
    {
        // Set top left sprite
        gSpriteClips[ 0 ].x = 0;
        gSpriteClips[ 0 ].y = 0;
        gSpriteClips[ 0 ].w = 100;
        gSpriteClips[ 0 ].h = 100;

        // Set top right sprite
        gSpriteClips[ 1 ].x = 100;
        gSpriteClips[ 1 ].y = 0;
        gSpriteClips[ 1 ].w = 100;
        gSpriteClips[ 1 ].h = 100;

        // Set bottom left sprite
        gSpriteClips[ 2 ].x = 0;
        gSpriteClips[ 2 ].y = 100;
        gSpriteClips[ 2 ].w = 100;
        gSpriteClips[ 2 ].h = 100;

        // Set bottom right sprite
        gSpriteClips[ 3 ].x = 100;
        gSpriteClips[ 3 ].y = 100;
        gSpriteClips[ 3 ].w = 100;
        gSpriteClips[ 3 ].h = 100;
    }

    return success;
}

void close()
{
    // Free loaded images
    gFooTexture.free();
    gBackgroundTexture.free();

    // Free loaded image
    //SDL_DestroyTexture( gTexture );
    //gTexture = NULL;

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

void createViewPorts()
{
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
}

void createDrawings()
{
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

    SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );  // Render the texture to screen
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
                //createViewPorts();

                // CREATE DRAWINGS
                //createDrawings();

                // Render background texture to screen
                //gBackgroundTexture.render( 0, 0 );

                // Render Foo to the screen
                //gFooTexture.render( 240, 190 );

                // Render sprites
                gSpriteSheetTexture.render( 0, 0, &gSpriteClips[ 0 ] );  // top left sprite
                gSpriteSheetTexture.render( SCREEN_WIDTH - gSpriteClips[ 1 ].w, 0, &gSpriteClips[ 1 ] );  // top right sprite
                gSpriteSheetTexture.render( 0, SCREEN_HEIGHT - gSpriteClips[ 2 ].h, &gSpriteClips[ 2 ] );  // bottom left sprite
                gSpriteSheetTexture.render( SCREEN_WIDTH - gSpriteClips[ 3 ].w, SCREEN_HEIGHT - gSpriteClips[ 3 ].h, &gSpriteClips[ 3 ] );  // bottom right sprite

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