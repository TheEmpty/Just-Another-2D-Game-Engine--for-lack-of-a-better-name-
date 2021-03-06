/**
 * @file Credits.h
 * @brief Declaration for the credits game state. Displays dynamic credits and sends users back when they press return.
 *
 * @author Mohammad El-Abid
 */

#include "GameState.h"
#include "GameStateHelper.h"
#include "Helper.h"
#include "SDL.h"
#include "SDL_ttf.h"

#ifndef CREDITS_H
#define CREDITS_H

class Credits : public JA2GE::GameState
{
private:
    // Number of lines
    int messages;
    // The state that sent us to the credits
    int prev_state;
    // Space to add between lines (excluding the text height)
    int padding;
    
    Uint32 backgroundColor;

    // A pointer to an array of SDL_Surface pointers. Allowing us to dynamically create an SDL_Surface array without memory errors.
    SDL_Surface** message;
    // A pointer to the pointer to the font to be used for all the text
    TTF_Font** font;
    // State manager
    JA2GE::GameStateHelper *state_helper;

    // Recursive function for getting the height of a row, or the first non NULL row if row is NULL (blank line)
    int get_height( int index );
    
    void create_messages();

public:
    /**
     * @brief Creates a credits state/scene.
     *
     * @param prev Previous state ID.
     * @param newFont Font to use when rendering text
     * @param newPadding Amount of padding to use between lines
     */
    Credits( int prev, TTF_Font** newFont, int newPadding );
    /**
     * @brief A deconstructor to free the SDL_Surface array (message)
     */
    ~Credits();

    /**
     * @brief Handle "events" generally user input.
     */
    void handle_event( const JA2GE::Window* window, SDL_Event* anEvent );

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

#endif // CREDITS_H