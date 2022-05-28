#include <iostream>
// Example program:
// Using SDL2 to create an application window

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    SDL_Window *sdl_window = nullptr;                    // Declare a pointer

    if (SDL_Init(SDL_INIT_EVERYTHING)){
        printf("Failed to init SDL, error: %s", SDL_GetError());
        return -1;
    };              // Initialize SDL2

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);
    if ((initted & flags) != flags)
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );

    int win_width = 640;
    int win_height = 480;
    enum class SCREENSIZE
    {
        is640x480,
        is1366x768,
        fullscreen
    } curr_screen_size = SCREENSIZE::is640x480,
    last_non_fullscreen_size = SCREENSIZE::is640x480;

    // Create an application window with the following settings:
    sdl_window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        win_width,                               // width, in pixels
        win_height,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (sdl_window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    
    // creates a renderer to render our images
    SDL_Renderer* rend = SDL_CreateRenderer(sdl_window, -1, render_flags);
 
    // creates a surface to load an image into the main memory
    SDL_Surface* surface = nullptr;

    surface = IMG_Load("./Assets/Tablero.png");
    if (surface == nullptr)
        printf("%s\n", SDL_GetError());

    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surface);
    if (tex == nullptr)
        printf("%s\n", SDL_GetError());

    SDL_RenderCopy(rend, tex, NULL, NULL);
    SDL_RenderPresent(rend);

    // clears main-memory
    SDL_FreeSurface(surface);

    // let us control our image position
    // so that we can move it with our keyboard.
    // connects our texture with dest to control position
    //SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);


    // The window is open: could enter program loop here (see SDL_PollEvent())

    SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

    // Close and destroy the window
    SDL_DestroyWindow(sdl_window);

    // Clean up
    SDL_Quit();
    return 0;
}