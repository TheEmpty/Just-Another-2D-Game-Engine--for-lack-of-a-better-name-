/**
 * @file main.cpp
 * @brief Handles core game functionality such as creating the window and handling GameStates.
 *
 * @author Mohammad El-Abid
 */

// IDEA: Menu with mouse would be nice, but rest of game doesn't use mouse (though perhaps RPGs would).

// following is a suggested order

// TODO: screen scaling
// TODO: load map (meta data, tileset(s) data, tile data (collision), map data, events (scripting)
// TODO: Player in map (physics, control, gamepad, multiplayer)
// THOUGHT: in multiplayer, would the camera follow player 1 or find the midpoint of the everyone and prevent them from leaving camera?
// TODO: create maps
// TODO: Mac OS X port (should just involve setting the delegate up [also debug])
// TODO: Cutscene scripting
// TODO: GUI overlays
// TODO: AI Scripting
// TODO: audio
// TODO: better sprites

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
 * @return The surface initalized by SDL
 */
SDL_Surface* init()
{
    // Tell SDL to load
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        return NULL;

    // Create the main screen
    SDL_Surface* screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BITS_PER_PIXEL, SDL_SWSURFACE );
    if( screen == NULL )
        return NULL;

    // Tell SDL_ttf to load
    if( TTF_Init() == -1 )
        return NULL;

    // Open our font
    font = TTF_OpenFont( "Squada One.ttf", 36 );
    if( font == NULL)
        return NULL;

    // Set the window title
    SDL_WM_SetCaption( SCREEN_TITLE, NULL );
    return screen;
}

/**
 * @brief Preforms all actions to close the application, free memory, save files, or other vital functions.
 *
 * @param screen The video surface we are drawing on
 */
void clean_up(SDL_Surface* screen)
{
    // Delete current state
    delete state_helper->currentState;

    // Free surfaces
    SDL_FreeSurface( screen );

    // Let SDL TTF unload
    TTF_CloseFont( font );
    TTF_Quit();

    // Let SDL free items
    SDL_Quit();
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
    SDL_Surface* screen = init();
    if( screen == NULL )
        return 1;

    Timer fps_cap;

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
        state_helper->currentState->handle_events();
        
        // Do state logic
        state_helper->currentState->logic();
        
        // Change state if needed
        change_state();
        
        // Do state rendering
        state_helper->currentState->render(screen);
        
        // Update the screen
        if( SDL_Flip( screen ) == -1 )
            return 1;
        
        // Prevent short frames
        if( fps_cap.get_ticks() < 1000 / FRAMES_PER_SECOND )
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps_cap.get_ticks() );
    }

    // Free our data, do anything important
    clean_up( screen );

    return 0;
}
