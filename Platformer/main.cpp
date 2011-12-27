/**
 * @file main.cpp
 * @brief Handles core game functionality such as creating the window and handling GameStates.
 *
 * @author Mohammad El-Abid
 */

// THOUGHT: Menu with mouse would be nice, but rest of game doesn't use mouse (though perhaps RPGs would).
// THOUGHT: in multiplayer, would the camera follow player 1 or find the midpoint of the everyone and prevent them from leaving camera?
// RESPONSE: Just use 2 options on multiplayer cameras

// following is a suggested order

// TODO: how to make a scaling background?
// TODO: GameStateHelper should not be singleton, be prepared to allow multiple windows (SDL 1.3)
// TODO: check definitions for any uneeded variable copies
// TODO: there is little consitency on spaces between argument parameters ( this ) or (that)
// TODO: load map (meta data, tileset(s) data, tile data (collision), map data, events (scripting)
// TODO: Player in map (physics, control, gamepad, multiplayer)
// TODO: create maps
// TODO: Cutscene scripting
// TODO: GUI overlays
// TODO: AI Scripting
// TODO: audio
// TODO: better sprites
// TODO: SDL 1.3, SDL 1.3 SDL_TTF, SDL 1.3 desktop width/height

#include "Camera.h"
#include "Credits.h"
#include "GameState.h"
#include "GameStateHelper.h"
#include "globals.h"
#include "Helper.h"
#include "Intro.h"
#include "Menu.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "Timer.h"
#include "Title.h"
#include "Window.h"
#include <string>

GameStateHelper* state_helper = GameStateHelper::Instance();

// Font stuff, TODO: find a better place for them
TTF_Font *font = NULL;
const int FONT_PADDING = 8;

/**
 * @brief Will change the current scene/state when required
 */
void change_state()
{
    // If the state needs to be changed
    if( state_helper->nextState != STATE_NULL )
    {
        //Delete the current state
        if( state_helper->nextState != STATE_EXIT )
            delete state_helper->currentState;
         
        // Change the state
        switch( state_helper->nextState )
        {
            case STATE_TITLE:
                state_helper->currentState = new Title( font, FONT_PADDING );
                break;
            case STATE_CREDITS:
                state_helper->currentState = new Credits( state_helper->stateID, font, FONT_PADDING );
                break;
        }

        state_helper->stateID = state_helper->nextState;
        state_helper->nextState = STATE_NULL;    
    }
}

/**
 * @brief Initalizes the libraries and loads any common data
 *
 * @return 0 for all good, 1 for SDL eror, 2 for SDL_TTF error, 3 for any resource errors
 */
int init()
{
    // Tell SDL to load
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        return 1;

    // Tell SDL_ttf to load
    if( TTF_Init() == -1 )
        return 2;

    // Open our font
    font = TTF_OpenFont( Helper::get_path_for_resource("Squada One.ttf").c_str(), 36 );
    if( font == NULL)
        return 3;

    return 0;
}


/**
 * @brief Preforms all actions to close the application, free memory, save files, or other vital functions.
 *
 * @param screen The video surface we are drawing on
 */
void clean_up()
{
    // Delete current state
    delete state_helper->currentState;

    // Let SDL TTF unload
    TTF_CloseFont( font );
    TTF_Quit();

    // Let SDL free items
    SDL_Quit();
}

/**
 * @brief Sets the resource path for the application.
 */
void setResourcePath( char executablePath[] )
{
    // ../application folder/platformer
    // ..\application folder\platformer.exe
    std::string path(executablePath);
    // remove binary item
#ifndef _WIN32
    path = path.substr(0, path.find_last_of("/"));
#endif
#ifdef _WIN32
    path = path.substr(0, path.find_last_of("\\"));
#endif

#if _WIN32
    Helper::resourcePath = path;
#endif
#if __APPLE__
    path = path.substr(0, path.find_last_of("/")); // remove MacOS folder
    Helper::resourcePath = path + "/Resources"; // add resources folder
#endif
}

/**
 * @brief Calls initalizing code, then constantly runs the state management at FRAMES_PER_SECOND
 *
 * @param argc Argument count, length of follow paramter.
 * @param args[] Command line arguments
 * @return exit code
 */
int main( int argc, char* args[] )
{
    Timer fps_cap;
    setResourcePath(args[0]);
    
    if( init() != 0 )
        return 1;
    
    // Before creating a screen, get the size of the default monitor
    const SDL_VideoInfo* ptrVidInfo = SDL_GetVideoInfo();
    int desktopHeight = ptrVidInfo->current_h;
    int desktopWidth = ptrVidInfo->current_w;

    Window window = Window( SCREEN_TITLE );
    if( window.error() )
        return 1;
    // share our sizes
    window.fullscreenHeight = desktopHeight;
    window.fullscreenWidth = desktopWidth;

    // Set the current state ID
    state_helper->stateID = STATE_INTRO;

    // Set the current game state object
    state_helper->currentState = new Intro(font);

    // While the user hasn't quit
    while( state_helper->stateID != STATE_EXIT )
    {
        // Start the frame timer
        fps_cap.start();
        
        // Do state event handling
        window.handle_events();
        
        // Do state logic
        window.logic();
        
        // Change state if needed
        change_state();
        
        // Do state rendering
        window.render();
        
        // Update the screen
        if( SDL_Flip( window.get_screen() ) == -1 )
            return 1;
        
        // Prevent short frames
        if( fps_cap.get_ticks() < 1000 / FRAMES_PER_SECOND )
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps_cap.get_ticks() );
    }

    // Free our data, do anything important
    clean_up();

    return 0;
}