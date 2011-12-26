/**
 * @file Menu.h
 * @brief Defenitions for a dynamic menu system.
 *
 * @author Mohammad El-Abid
 */

#include "Helper.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <string>

#ifndef MENU_H
#define MENU_H

class Menu
{
private:
    // Font used to render the menu text
    TTF_Font *font;
    // Default color of the text
    SDL_Color defaultColor;
    // Color used when "selected" or "hovering"
    SDL_Color selectedColor;

    // How many items are on the menu
    int menuLength;
    // Which index is "selected"
    int selectedMenuItem;
    // Array of the menu strings like "Play" and "Credits"
    std::string *menuText;
    // The rendered surfaces of menu text.
    SDL_Surface **menuItems;

    /**
     * @brief Free previous surface and re-renders the menu item. Used when color need to change.
     *
     * @param index The index of the surface in menuItems.
     */
    void renderText( int index );

public:
    /**
     * @brief Creates a menu based on passed font, colors, and length.
     *
     * @param font Font to be used when "writing."
     * @param defaultC Default color of text for menu items.
     * @param selectedC Color of text for selected menu items.
     * @param length The number of entries in the menu. "Play", "Settings", "Credit" would be 3.
     */
    Menu( TTF_Font *font, SDL_Color defaultC, SDL_Color selectedC, int length );

    /**
     * @brief Unloads dynamic arrays.
     */
    ~Menu();

    /**
     * @brief Sets the text for a menu item.
     *
     * @param index The index of the entry to modify.
     * @param text The text to render.
     */
    void setText( int index, std::string text );

    /**
     * @brief Used to calculate new selected item, useful when user presses up at top of list.
     *
     * @param key SDLK_UP or SDLK_DOWN are the only keys it responds too.
     */
    void reactToKey( SDLKey key );
    //void reactToMouse(int x, int y);

    /**
     * @brief Returns the length of the menu items
     */
    int getLength();
    /**
     * @brief Returns the currently selected position on the menu (selectedMenuItem).
     */
    int getPos();

    /**
     * @brief returns the SDL_Surface of the rendered text for the supplied index.
     */
    SDL_Surface* getMenuItem( int index );
};

#endif // MENU_H