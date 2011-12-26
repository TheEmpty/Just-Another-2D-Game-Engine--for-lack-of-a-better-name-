/**
 * @file GameState.h
 * @brief The class used to create polymorphic game states.
 *
 * @author Mohammad El-Abid
 */

#include "SDL.h"

#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState
{
public:
    virtual ~GameState(){};

    /**
     * @brief Handle "events" generally user input.
     */
    virtual void handle_events() = 0;

    /**
     * @brief Handle any logic such as physics or autosave.
     */
    virtual void logic() = 0;

    /**
     * @brief Renders graphics to the supplied surface.
     *
     * @param screen The SDL_Surface to draw on, generally the surface initalized by SDL.
     */
    virtual void render( SDL_Surface* screen ) = 0;
};

#endif