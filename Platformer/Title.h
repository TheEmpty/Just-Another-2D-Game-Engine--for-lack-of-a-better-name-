/**
 * @file Title.h
 * @brief Title state, the main menu.
 *
 * @author Mohammad El-Abid
 */

#include "GameState.h"
#include "GameStateHelper.h"
#include "Menu.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "Timer.h"

#ifndef TITLE_H
#define TITLE_H

class Title : public GameState
{
private:
    // Padding between lines of text.
    int padding;
    // Background image.
    SDL_Surface *background;
    // Menu.
    Menu *menu;
    // Font to use on menu.
    TTF_Font* font;
    // State managment
    GameStateHelper *state_helper;
    // Used to tell how long since last reaction to held key.
    Timer reaction;

public:
    /**
     * @brief Creates a new title screen with menu.
     *
     * @param newFont The font to use while rendering.
     * @param newPadding the distance between the lines of text (in addition to the height of the text).
     */
    Title(TTF_Font* newFont, int newPadding);
    /**
     * @brief free any objects that are not automatically freed by scope.
     */
    ~Title();

    /**
     * @brief Handle "events" generally user input.
     */
    void handle_events();

    /**
     * @brief Handle any logic such as physics or autosave.
     */
    void logic();

    /**
     * @brief Renders graphics to the supplied surface.
     *
     * @param screen The SDL_Surface to draw on, generally the surface initalized by SDL.
     */
    void render( SDL_Surface* screen );
};

#endif //TITLE_H