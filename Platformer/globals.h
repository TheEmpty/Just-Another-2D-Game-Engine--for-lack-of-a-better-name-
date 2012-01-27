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
const int DEFAULT_SCREEN_WIDTH = 940;
const int DEFAULT_SCREEN_HEIGHT = 470;
const int SCREEN_BITS_PER_PIXEL = 32;
const int FRAMES_PER_SECOND = 60;
const int MINIMUM_SCREEN_WIDTH = 640;
const int MINIMUM_SCREEN_HEIGHT = 480;
const int KMOD_LCOMMAND = 1024;
const int KMOD_RCOMMAND = 2048;

// Font
const SDL_Color DEFAULT_FONT_COLOR = {0, 0, 0};
const SDL_Color ACTIVE_FONT_COLOR  = {0, 122, 153};

// Game state ids
enum GameStates
{
    // NULL and EXIT are in JA2GE
    STATE_INTRO,
    STATE_TITLE,
    STATE_SELECT_MAP,
    STATE_MAP,
    STATE_CREDITS,
};

#endif // DEFINITIONS_H