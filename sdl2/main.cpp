/*
To compile on Ubuntu
* sudo apt-get install --yes libsdl2-dev
* g++ main.cpp -I /usr/include/SDL2/ -lSDL2 -lGL -lSDL2_image -lSDL2_ttf -lSDL2_mixer -g
 -g to debug
* Execute with ./a.out -g

*/

// Using SDL and Standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>  // for sound
#include <stdio.h>
#include <string>
#include <cmath>


// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

// Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

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

        // We don't load function if defined statement is not included
        #ifdef _SDL_TTF_H
        // Creates image from font string
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
        #endif

        // Deallocates texture
        void free();

        // Set color modulation - increase that color throughout the texture
        void setColor( Uint8 red, Uint8 green, Uint8 blue );  // Uint is Unsigned and 8 bit (0 to 255)

        // Set blending
        void setBlendMode( SDL_BlendMode blending );

        // Set alpha modulation
        void setAlpha( Uint8 alpha );

        // Renders texture at given point
        void render( int x, int y , SDL_Rect* clip = NULL, double angle = 0.0,
                     SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
        // SDL_Rect* clip accepts a rect for which part to render / clip
        // SDL_RendererFlip lets you rotate at an angle or flip the object

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

// LButtonSprite
enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

// The mouse button
class LButton
{
    public:
        // Initialize internal variables
        LButton();

        // Sets top left position
        void setPosition( int x, int y );

        // Handles mouse event
        void handleEvent( SDL_Event* e );

        // Shows button sprite
        void render();

    private:
        // Top left position
        SDL_Point mPosition;

        // Currently used global sprite
        LButtonSprite mCurrentSprite;
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
//SDL_Rect gSpriteClips[ 4 ];
//LTexture gSpriteSheetTexture;

// Walking animation
const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect gSpriteWalkClips[ 4 ];
LTexture gSpriteWalkSheetTexture;

// Texture for Rotation and Flipping
LTexture gArrowTexture;

// Globally used font
//TTF_Font *gFont = NULL;
LTexture gTextTexture;

// Mouse Button Sprites
SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];
LTexture gButtonSpriteSheetTexture;

//Buttons objects
LButton gButtons[ TOTAL_BUTTONS ];

//Scene textures
LTexture gPressTexture;
LTexture gUpTexture;
LTexture gDownTexture;
LTexture gLeftTexture;
LTexture gRightTexture;

//Scene texture
LTexture gPromptTexture;

// Music that will be played
Mix_Music *gMusic = NULL;

// Sound effects that will be used
Mix_Chunk *gScratch = NULL;
Mix_Chunk *gHigh = NULL;
Mix_Chunk *gMedium = NULL;
Mix_Chunk *gLow = NULL;

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

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
    // Get rid of preexisting texture
    free();

    // Render text surface, need surface to render onto
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_TTF Error: %s\n", TTF_GetError() );
    }
    else
    {
        // Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            // Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        // Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    // Return success
    return mTexture != NULL;
}
#endif

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

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    // Render a texture, with optional clip param to see what parts to clip and rotation or flip

    // Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    // Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

// Enables blending
void LTexture::setBlendMode( SDL_BlendMode blending )
{
    // set blending function
    SDL_SetTextureBlendMode( mTexture, blending );
}


// The lower the alpha, the more transparent a texture is
void LTexture::setAlpha( Uint8 alpha )
{
    // Modulate texture alpha
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    // Modulate texture
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

// Constructor for LButton
LButton::LButton()
{
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition( int x, int y)
{
    mPosition.x = x;
    mPosition.y = y;
}

// Handle Mouse Events
void LButton::handleEvent( SDL_Event* e )
{
    // If mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        // Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );

        // Check if mouse is in button
        bool inside = true;

        // Check if mouse is left of the button
        if( x < mPosition.x )
        {
            inside = false;
        }

        // Check if mouse above the button
        else if( x > mPosition.x + BUTTON_WIDTH )
        {
            inside = false;
        }

        // Mouse above the button
        else if( y < mPosition.y )
        {
            inside = false;
        }

        // Mouse below the button
        else if( y > mPosition.y + BUTTON_HEIGHT )
        {
            inside = false;
        }

        // Mouse is outside button
        if( !inside )
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }

        // Mouse is inside button
        else
        {
            // Set mouse over sprite
            switch( e->type)
            {
                case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;

                case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                break;

                case SDL_MOUSEBUTTONUP:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                break;
            }
        }
    }
}

void LButton::render()
{
    // Show current button sprite
    gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
}

bool init()
{
    // Initialization Flag
    bool success = true;

    // Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
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
            // Create renderer for the window - Vertical Sync allows rendering to update
            // at the same time as your monitor during vertical refresh
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
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

                // Initialize SDL_TTF
                //if( TTF_Init() == -1 )
                //{
                //    printf("SDL_TTF could not initialize! SDL_TTF Error: %s\n", TTF_GetError() );
                //    success = false;
                //}

                // Initialize SDL_mixer
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_Mixer Error: %s\n", Mix_GetError() );
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
    //if( !gSpriteSheetTexture.loadFromFile( "resources/dots.png" ) )
    //{
    //    printf("Failed to load sprite sheet texture!\n" );
    //    success = false;
    //}
    //else
    //{
    //    // Set top left sprite
    //    gSpriteClips[ 0 ].x = 0;
    //    gSpriteClips[ 0 ].y = 0;
    //    gSpriteClips[ 0 ].w = 100;
    //    gSpriteClips[ 0 ].h = 100;

    //    // Set top right sprite
    //    gSpriteClips[ 1 ].x = 100;
    //    gSpriteClips[ 1 ].y = 0;
    //    gSpriteClips[ 1 ].w = 100;
    //    gSpriteClips[ 1 ].h = 100;

    //    // Set bottom left sprite
    //    gSpriteClips[ 2 ].x = 0;
    //    gSpriteClips[ 2 ].y = 100;
    //    gSpriteClips[ 2 ].w = 100;
    //    gSpriteClips[ 2 ].h = 100;

    //    // Set bottom right sprite
    //    gSpriteClips[ 3 ].x = 100;
    //    gSpriteClips[ 3 ].y = 100;
    //    gSpriteClips[ 3 ].w = 100;
    //    gSpriteClips[ 3 ].h = 100;
    //}

    // Load Walking Sprite Animation
    //if( !gSpriteWalkSheetTexture.loadFromFile( "resources/foo_walk.png" ))
    //{
    //    printf( "Failed to load walking animation texture!\n" );
    //    success = false;
    //}
    //else
    //{
    //    // Set Sprite Class
    //    gSpriteWalkClips[ 0 ].x = 0;
    //    gSpriteWalkClips[ 0 ].y = 0;
    //    gSpriteWalkClips[ 0 ].w = 64;
    //    gSpriteWalkClips[ 0 ].h = 205;

    //    gSpriteWalkClips[ 1 ].x = 64;
    //    gSpriteWalkClips[ 1 ].y = 0;
    //    gSpriteWalkClips[ 1 ].w = 64;
    //    gSpriteWalkClips[ 1 ].h = 205;

    //    gSpriteWalkClips[ 2 ].x = 128;
    //    gSpriteWalkClips[ 2 ].y = 0;
    //    gSpriteWalkClips[ 2 ].w = 64;
    //    gSpriteWalkClips[ 2 ].h = 205;

    //    gSpriteWalkClips[ 3 ].x = 196;
    //    gSpriteWalkClips[ 3 ].y = 0;
    //    gSpriteWalkClips[ 3 ].w = 64;
    //    gSpriteWalkClips[ 3 ].h = 205;
    //}

    // Arrow for rotation and flip
    //if( !gArrowTexture.loadFromFile( "resources/arrow.png" ) )
    //{
    //    printf( "Failed to load arrow texture!\n" );
    //    success = false;
    //}

    // Open the font
    //gFont = TTF_OpenFont( "resources/lazy.ttf", 28);
    //if( gFont == NULL )
    //{
    //    printf( "Failed to load lazy font! SDL_TTF Error: %s\n", TTF_GetError() );
    //    success = false;
    //}
    //else
    //{
    //    // Render text
    //    SDL_Color textColor = { 0, 0, 0 };
    //    if( !gTextTexture.loadFromRenderedText( "Hello World!", textColor ) )
    //    {
    //        printf( "Failed to render text texture!\n" );
    //        success = false;
    //    }
    //}

    // Sprites
    // if( !gButtonSpriteSheetTexture.loadFromFile( "resources/button.png") )
    // {
    //     printf("Failed to load button sprite texture!\n");
    //     success = false;
    // }
    // else
    // {
    //     //Set sprites
	// 	for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
	// 	{
	// 		gSpriteClips[ i ].x = 0;
	// 		gSpriteClips[ i ].y = i * 200;
	// 		gSpriteClips[ i ].w = BUTTON_WIDTH;
	// 		gSpriteClips[ i ].h = BUTTON_HEIGHT;
	// 	}

	// 	//Set buttons in corners
	// 	gButtons[ 0 ].setPosition( 0, 0 );
	// 	gButtons[ 1 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, 0 );
	// 	gButtons[ 2 ].setPosition( 0, SCREEN_HEIGHT - BUTTON_HEIGHT );
	// 	gButtons[ 3 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT );
    // }

    // Load key press textures
    // if( !gPressTexture.loadFromFile( "resources/press.png" ) )
    // {
    //     printf( "Failed to load press texture!\n" );
    //     success = false;
    // }

    // if( !gUpTexture.loadFromFile( "resources/up.png" ) )
    // {
    //     printf( "Failed to load up texture!\n" );
    //     success = false;
    // }
    // if( !gDownTexture.loadFromFile( "resources/down.png" ) )
    // {
    //     printf( "Failed to load down texture!\n" );
    //     success = false;
    // }
    // if( !gLeftTexture.loadFromFile( "resources/left.png" ) )
    // {
    //     printf( "Failed to load left texture!\n" );
    //     success = false;
    // }
    // if( !gRightTexture.loadFromFile( "resources/right.png" ) )
    // {
    //     printf( "Failed to load right texture!\n" );
    //     success = false;
    // }

    // Music
    if( !gPromptTexture.loadFromFile( "resources/prompt.png" ) )
    {
        printf( "Failed to load prompt texture!\n" );
        success = false;
    }
    // Load Music
    gMusic = Mix_LoadMUS( "resources/beat.wav" );
    if( gMusic == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    // Load Sound Effects
    gScratch = Mix_LoadWAV( "resources/scratch.wav" );
    if( gScratch == NULL )
    {
        printf( "Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    gHigh = Mix_LoadWAV( "resources/high.wav" );
    if( gHigh == NULL )
    {
        printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    gMedium = Mix_LoadWAV( "resources/medium.wav" );
    if( gMedium == NULL )
    {
        printf( "Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    gLow = Mix_LoadWAV( "resources/low.wav" );
    if( gLow == NULL )
    {
        printf( "Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    return success;
}

void close()
{
    // Free loaded images
    gFooTexture.free();
    gBackgroundTexture.free();

    // free loaded images
    gTextTexture.free();

    // Free global font
    //TTF_CloseFont( gFont );
    //gFont = NULL;

    // Free loaded image
    //SDL_DestroyTexture( gTexture );
    //gTexture = NULL;

    // Free loaded images
    gPromptTexture.free();

    // Free sound effects
    Mix_FreeChunk( gScratch );
    Mix_FreeChunk( gHigh );
    Mix_FreeChunk( gMedium );
    Mix_FreeChunk( gLow );
    gScratch = NULL;
    gHigh = NULL;
    gMedium = NULL;
    gLow = NULL;

    // Free Music
    Mix_FreeMusic( gMusic );
    gMusic = NULL;


    // Destroy the window and renderer
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    // Shut down Music
    Mix_Quit();

    // Shut down TTF
    //TTF_Quit();

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

void createRotateFlip()
{
    // Clear the screen
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( gRenderer );

    double degrees = 30;  // angle of rotation
    SDL_RendererFlip flipType = SDL_FLIP_NONE;  // type of flip

    flipType = SDL_FLIP_HORIZONTAL;
    //flipType = SDL_FLIP_VERTICAL;

    // Render an arrow
    gArrowTexture.render( ( SCREEN_WIDTH - gArrowTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gArrowTexture.getHeight() ) / 2, NULL, degrees, NULL, flipType );
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

            // Current animation frame
            //int frame = 0;

            // Current rendered texture
            LTexture* currentTexture = NULL;

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

                    // Handle Music
                    else if( e.type == SDL_KEYDOWN )
					{
						switch( e.key.keysym.sym )
						{
							//Play high sound effect
							case SDLK_1:
							Mix_PlayChannel( -1, gHigh, 0 );
							break;

							//Play medium sound effect
							case SDLK_2:
							Mix_PlayChannel( -1, gMedium, 0 );
							break;

							//Play low sound effect
							case SDLK_3:
							Mix_PlayChannel( -1, gLow, 0 );
							break;

							//Play scratch sound effect
							case SDLK_4:
							Mix_PlayChannel( -1, gScratch, 0 );
							break;

							case SDLK_9:
							//If there is no music playing
							if( Mix_PlayingMusic() == 0 )
							{
								//Play the music
								Mix_PlayMusic( gMusic, -1 );
							}
							//If music is being played
							else
							{
								//If the music is paused
								if( Mix_PausedMusic() == 1 )
								{
									//Resume the music
									Mix_ResumeMusic();
								}
								//If the music is playing
								else
								{
									//Pause the music
									Mix_PauseMusic();
								}
							}
							break;

							case SDLK_0:
							//Stop the music
							Mix_HaltMusic();
							break;
						}
					}

                    // Set texture based on current keystate
                    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
                    if( currentKeyStates[ SDL_SCANCODE_UP] )
                    {
                        currentTexture = &gUpTexture;
                    }
                    else if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
                    {
                        currentTexture = &gDownTexture;
                    }
                    else if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
                    {
                        currentTexture = &gLeftTexture;
                    }
                    else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
                    {
                        currentTexture = &gRightTexture;
                    }
                    else
                    {
                        currentTexture = &gPressTexture;
                    }

                    // Handle button events
                    //for( int i = 0; i < TOTAL_BUTTONS; ++i )
                    //{
                    //    gButtons[ i ].handleEvent( &e );
                    //}

                    // Play sound effects

                }

                // Clear the screen with the color last set from SDL_SetRenderDrawColor
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );  // Set clearing color as White

                SDL_RenderClear( gRenderer );

                // Render Buttons
                //for( int i = 0; i < TOTAL_BUTTONS; ++i )
                //{
                //    gButtons[ i ].render();
                //}

                // Render current texture
                currentTexture->render( 0, 0 );

                //SDL_RenderPresent( gRenderer );

                // CREATE VIEWPORTS
                //createViewPorts();

                // CREATE DRAWINGS
                //createDrawings();

                // Render background texture to screen
                //gBackgroundTexture.render( 0, 0 );

                // Render Foo to the screen
                //gFooTexture.render( 240, 190 );

                // Render sprites
                //gSpriteSheetTexture.render( 0, 0, &gSpriteClips[ 0 ] );  // top left sprite
                //gSpriteSheetTexture.render( SCREEN_WIDTH - gSpriteClips[ 1 ].w, 0, &gSpriteClips[ 1 ] );  // top right sprite
                //gSpriteSheetTexture.render( 0, SCREEN_HEIGHT - gSpriteClips[ 2 ].h, &gSpriteClips[ 2 ] );  // bottom left sprite
                //gSpriteSheetTexture.render( SCREEN_WIDTH - gSpriteClips[ 3 ].w, SCREEN_HEIGHT - gSpriteClips[ 3 ].h, &gSpriteClips[ 3 ] );  // bottom right sprite

                // Render walking frames
                //SDL_Rect* currentClip = &gSpriteWalkClips[ frame / 4 ];
                //gSpriteWalkSheetTexture.render( ( SCREEN_WIDTH - currentClip->w ) / 2,
                //    (SCREEN_HEIGHT - currentClip->h) / 2, currentClip );
                //++frame;  // go to next frame
                //// Cycle animation
                //if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                //{
                //    frame = 0;
                //}

                //createRotateFlip();

                // Render Font
                //gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTextTexture.getHeight() ) / 2 );

                // Render Prompt for Music
				gPromptTexture.render( 0, 0 );

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
