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

class Credits : public GameState
{
private:
    // Number of lines
    int messages;
    // The state that sent us to the credits
    int prev_state;
    // Space to add between lines (excluding the text height)
    int padding;

    // A pointer to an array of SDL_Surface pointers. Allowing us to dynamically create an SDL_Surface array without memory errors.
    SDL_Surface** message;
    // A pointer to the font to be used for all the text
    TTF_Font* font;
    // State manager
    GameStateHelper *state_helper;

    // Recursive function for getting the height of a row, or the first non NULL row if row is NULL (blank line)
    int get_height(int index);

public:
    /**
     * @brief Creates a credits state/scene.
     *
     * @param prev Previous state ID.
     * @param newFont Font to use when rendering text
     * @param newPadding Amount of padding to use between lines
     */
    Credits(int prev, TTF_Font* newFont, int newPadding);
    /**
     * @brief A deconstructor to free the SDL_Surface array (message)
     */
    ~Credits();

    /**
     * @brief Handle "events" generally user input.
     */
    void handle_event(const Window* window, SDL_Event* anEvent);

    /**
     * @brief Handle any logic such as physics or autosave.
     */
    void logic( const Window* window );

    /**
     * @brief Renders graphics to the supplied surface.
     *
     * @param screen The SDL_Surface to draw on, generally the surface initalized by SDL.
     */
    void render( const Window* window );
};

#endif // CREDITS_H