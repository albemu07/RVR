#include <iostream>
// Example program:
// Using SDL2 to create an application window

#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    SDL_Window *sdl_window = nullptr;                    // Declare a pointer

    if (SDL_Init(SDL_INIT_VIDEO)){
        printf("Failed to init SDL VIDEO, error: %s", SDL_GetError());
        return -1;
    };              // Initialize SDL2

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

    // The window is open: could enter program loop here (see SDL_PollEvent())

    SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

    // Close and destroy the window
    SDL_DestroyWindow(sdl_window);

    // Clean up
    SDL_Quit();
    return 0;
}