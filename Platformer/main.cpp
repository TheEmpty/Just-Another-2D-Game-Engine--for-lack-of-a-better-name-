/**
 * @file main.cpp
 * @brief Handles core game functionality such as creating the window and handling GameStates.
 *
 * @author Mohammad El-Abid
 */

// Engine 
    // TODO: JA2GE variables
    // TODO: bring documentation back upto date and consitency (learning C++ a lot here)
    // TODO: check definitions for any newb mistakes (passing in objects instead of pointers)
    // TODO: instead of "unmove" would be nice if it didn't move into the wall. So diagonal force would just ignore one force instead of both)
    // TODO: also to put it as close to the object as it can instead of last known working position
    // TODO: gravity
    // TODO: animation

    // TODO: parse spawn and end points
    // TODO: Gamepad
    // TODO: multiplayer?
    // TODO: create maps
    // TODO: Event scripting
    // TODO: Cutscene scripting
    // TODO: AI Scripting
    // TODO: audio (like character.bump_sound = ... )
    // TODO: GameStateHelper should not be singleton, be prepared to allow multiple windows (SDL 1.3)
    // TODO: SDL 1.3, SDL 1.3 SDL_TTF, SDL 1.3 desktop width/height

// Application 
    // TODO: At least fix the intro instead of showing black borders
    // TODO: An intro that is actually a level and a player running through it would be cool.

#include "Camera.h"
#include "Credits.h"
#include "GameState.h"
#include "GameStateHelper.h"
#include "globals.h"
#include "Helper.h"
#include "Intro.h"
#include "Map.h"
#include "Menu.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "Timer.h"
#include "Title.h"
#include "Window.h"
#include "PlayState.h"
#include "SelectState.h"
#include "InvisableRectangle.h"
#include <string>

JA2GE::GameStateHelper* state_helper = JA2GE::GameStateHelper::Instance();

// Font stuff
TTF_Font *font = NULL;
const int FONT_PADDING = 8;

/**
 * @brief Will change the current scene/state when required
 */
void change_state()
{
    // If the state needs to be changed
    if( state_helper->nextState != JA2GE::STATE_NULL )
    {
        
        JA2GE::GameState* newState = NULL;
        
        // Change the state
        switch( state_helper->nextState )
        {
            case STATE_TITLE:
                newState = new Title( &font, FONT_PADDING );
                break;
            case STATE_CREDITS:
                newState = new Credits( state_helper->stateID, &font, FONT_PADDING );
                break;
            case STATE_MAP:
                {
                    std::string* file = new std::string;
                    state_helper->currentState->get_message( 0, file );
                    newState = new PlayState( &font, state_helper->stateID, file->c_str() );
                    break;
                }
            case STATE_SELECT_MAP:
                newState = new SelectState( &font );
                break;
        }
        
        // Delete previous/current state and switch states
        if( state_helper->nextState != JA2GE::STATE_EXIT ){
            delete state_helper->currentState;
            // Move newState to currentState
            state_helper->currentState = newState; 
        }
        
        state_helper->stateID = state_helper->nextState;
        state_helper->nextState = JA2GE::STATE_NULL;  
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
    font = TTF_OpenFont( JA2GE::Helper::get_path_for_resource( "Squada One.ttf" ).c_str(), 36 );
    if( font == NULL )
        return 3;

    // Load icon, should be used alongside OS standards such as Mac's PLIST
    // Avast seems to think the following code is a trojan... win32::downloader ftb
    /*
    SDL_Surface* icon = SDL_LoadBMP( Helper::get_path_for_resource( "images/heart.bmp" ).c_str() );
    if( icon != NULL )
    {
        Uint32 colorKey = SDL_MapRGB( icon->format, 0, 255, 0 );
        SDL_SetColorKey( icon, SDL_SRCCOLORKEY, colorKey );
        SDL_WM_SetIcon( icon, NULL );
    } else return 3;
    */

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
    std::string path( executablePath );
    // remove binary item
#ifndef _WIN32
    // Apple, linux, etc.
    path = path.substr( 0, path.find_last_of( "/" ) );
#endif
#ifdef _WIN32
    path = path.substr( 0, path.find_last_of( "\\" ) );
#endif

#ifndef __APPLE__
    // Linux and Windows don't have resource folders
    JA2GE::Helper::resourcePath = path;
#endif
#if __APPLE__
    // Only Apple
    path = path.substr( 0, path.find_last_of( "/" ) ); // remove MacOS folder
    JA2GE::Helper::resourcePath = path + "/Resources"; // add resources folder
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
    setResourcePath( args[0] );
    
    if( init() != 0 )
        return 1;
    
    // Before creating a screen, get the size of the default monitor
    const SDL_VideoInfo* ptrVidInfo = SDL_GetVideoInfo();
    int desktopHeight = ptrVidInfo->current_h;
    int desktopWidth = ptrVidInfo->current_w;

    // Create the main window
    JA2GE::Window window = JA2GE::Window( SCREEN_TITLE );
    if( window.error() )
        return 1;
    
    // share our sizes
    window.fullscreenHeight = desktopHeight;
    window.fullscreenWidth = desktopWidth;
    
    // set minimum sizes
    window.set_min_dim(MINIMUM_SCREEN_WIDTH, MINIMUM_SCREEN_HEIGHT);

    // Set the current state ID
    state_helper->stateID = STATE_INTRO;

    // Set the current game state object
    state_helper->currentState = new Intro(&font);

    // While the user hasn't quit
    while( state_helper->stateID != JA2GE::STATE_EXIT && window.error() == false )
    {
        // Start the frame timer
        fps_cap.start();
        
        // Do state event handling
        window.handle_events();
        
        // Scale text, would probably do this in a parent gamestate. But will do in abstraction
        if( window.screenChange )
        {
            int fontSize = ( window.get_width()/31 + window.get_height()/31 );
            
            TTF_Font* newFont = TTF_OpenFont( JA2GE::Helper::get_path_for_resource( "Squada One.ttf" ).c_str(), fontSize );
            if( newFont != NULL )
            {
                TTF_CloseFont( font );
                font = newFont;
            }
        }
        // E/O TEMP
        
        // Do state logic
        window.logic();
        
        // Change state if needed
        change_state();
        
        // Do state rendering
        window.render();
        
        // Update the screen
        if( SDL_Flip( window.get_screen() ) == -1 )
            return 1;
        
        // Prevent short frames by waiting the excess time
        if( fps_cap.get_ticks() < 1000 / FRAMES_PER_SECOND )
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps_cap.get_ticks() );
    }

    // Free our data, do anything important
    clean_up();

    return 0;
}