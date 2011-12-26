/**
 * @file globals.h
 * @brief Any variables that are constant and used in various files.
 *
 * @author Mohammad El-Abid
 */

#include "SDL.h"

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Screen
const char SCREEN_TITLE[] = "Just Another 2D Game Engine (for lack of a better name)";
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BITS_PER_PIXEL = 32;
const int FRAMES_PER_SECOND = 60;

// Font
const SDL_Color defaultFontColor = {0, 0, 0};
const SDL_Color activeFontColor  = {0, 122, 153};

// Game state ids
enum GameStates
{
    STATE_NULL,
    STATE_INTRO,
    STATE_TITLE,
    STATE_CREDITS,
    STATE_MAP,
    STATE_EXIT,
};

#endif // DEFINITIONS_H