/**
 * @file Window.h
 * @brief Allows us to eaisly create and maintain SDL Windows.
 *
 * @author Mohammad El-Abid
 */

#ifndef WINDOW_H
#define WINDOW_H

#include "SDL.h"
#include "globals.h"
#include "GameStateHelper.h" // includes GameStateHelper which includes GameState which includes Window...

class Window
{
private:
    GameStateHelper* state_helper;
    bool isFullscreen;
    bool errors;

    int windowedWidth;
    int windowedHeight;
    int bpp;

    SDL_Surface* screen;
public:
    Window(const char title[], int newWidth = DEFAULT_SCREEN_WIDTH, int newHeight = DEFAULT_SCREEN_HEIGHT, int screen_bpp = SCREEN_BITS_PER_PIXEL);
    
    int fullscreenWidth;
    int fullscreenHeight;

    bool error();
    bool fullscreen();
    void fullscreen( bool on );
    void create_screen( void );
    void create_screen( bool fullscreen );
    
    SDL_Surface* get_screen() const { return screen; }
    int get_width() const { return isFullscreen ? fullscreenWidth : windowedWidth; }
    int get_height() const { return isFullscreen ? fullscreenHeight : windowedHeight; }
    int get_bpp() const { return bpp; }

    void logic();
    void render();
    void handle_events();
    void resize( SDL_Event* anEvent );
    void fullscreen( SDL_Event* anEvent );
};

#endif // WINDOW_H