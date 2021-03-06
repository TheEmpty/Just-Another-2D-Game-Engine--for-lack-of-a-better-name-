/**
 * @file Intro.h
 * @brief Declaration for the Intro state. This is the state displayed when the user first start the application.
 *
 * @author Mohammad El-Abid
 */

#include "GameState.h"
#include "GameStateHelper.h"
#include "globals.h"
#include "Helper.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "Timer.h"

class Intro : public JA2GE::GameState
{
private:
    // Background image surface
    SDL_Surface* background;
    // Version text surface
    SDL_Surface* message;
    // Font that will be used
    TTF_Font** font;
    // State managment
    JA2GE::GameStateHelper* state_helper;
    // If the user doesn't click or press any button, we'll forward them based on the ticks since state started.
    Timer wait;

public:
    /**
     * @brief Creates the intro/opening application state/scene.
     *
     * @param newFont Font to use when rendering text
     */
    Intro( TTF_Font** newFont );

    /**
     * @brief Frees the SDL_Surfaces used and any other responsabilites need to be performed for a clean deletion.
     */
    ~Intro();

    /**
     * @brief Handle "events" generally user input.
     */
    void handle_event(const JA2GE::Window* window, SDL_Event* anEvent);

    /**
     * @brief Handle any logic such as physics or autosave.
     */
    void logic( const JA2GE::Window* window );

    /**
     * @brief Renders graphics to the supplied surface.
     *
     * @param screen The SDL_Surface to draw on, generally the surface initalized by SDL.
     */
    void render( const JA2GE::Window* window );
    
    void get_message( int command, std::string* response ) {};
};