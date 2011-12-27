/**
 * @file GameState.h
 * @brief The class used to create polymorphic game states.
 *
 * @author Mohammad El-Abid
 */

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "SDL.h"
#include "Window.h"
class Window;

class GameState
{
public:
    virtual ~GameState(){};

    /**
     * @brief Handle "events" generally user input.
     */
    virtual void handle_event(const Window* window, SDL_Event* anEvent) = 0;

    /**
     * @brief Handle any logic such as physics or autosave.
     */
    virtual void logic( const Window* window ) = 0;

    /**
     * @brief Renders graphics to the supplied surface.
     *
     * @param screen The SDL_Surface to draw on, generally the surface initalized by SDL.
     */
    virtual void render( const Window* window ) = 0;
};

#endif // GAMESTATE_H