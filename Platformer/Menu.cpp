/**
 * @file Menu.cpp
 * @brief Defenitions for a dynamic menu system. See more in the header file.
 *
 * @author Mohammad El-Abid
 */

#include "Menu.h"

Menu::Menu( TTF_Font **setFont, SDL_Color defaultC, SDL_Color selectedC, int size )
{
    font = setFont;
    defaultColor = defaultC;
    selectedColor = selectedC;

    menuLength = size;
    selectedMenuItem = 0;
    menuText  = new std::string[size];
    menuItems = new SDL_Surface* [size];
    for( int i = 0; i < size; i++ ) 
      menuItems[i] = SDL_CreateRGBSurface(0,0,0,0,0,0,0,0);
}

Menu::~Menu()
{
    for( int i = 0; menuLength > i; i++ )
        SDL_FreeSurface( menuItems[i] );
    delete menuItems;
    delete [] menuText;
}

void Menu::setText( int index, std::string text )
{
    if( index >= 0 && menuLength > index )
    {
        menuText[index] = text;
        renderText( index );
    }
}

void Menu::redraw()
{
    for( int i = 0; i < menuLength; i++ )
        renderText( i );
}

void Menu::renderText( int index )
{
    SDL_FreeSurface( menuItems[index] );

    SDL_Color *fg;
    if( selectedMenuItem != index )
        fg = &defaultColor;
    else
        fg = &selectedColor;

    menuItems[index] = TTF_RenderText_Solid( *font, menuText[index].c_str(), *fg );
}

void Menu::reactToKey( SDLKey key )
{
    int prev = selectedMenuItem;

    if( key == SDLK_UP )
        selectedMenuItem--;
    else if( key == SDLK_DOWN )
        selectedMenuItem++;

    if( 0 > selectedMenuItem )
        selectedMenuItem = menuLength + selectedMenuItem;
    else if( menuLength <= selectedMenuItem )
        selectedMenuItem -= menuLength;

    renderText( prev );
    renderText( selectedMenuItem );
}

SDL_Surface *Menu::getMenuItem( int index )
{
    if( index < 0 || index > menuLength-1 )
        return NULL;
    else
        return menuItems[index];
}

int Menu::getLength()
{
    return menuLength;
}

int Menu::getPos()
{
    return selectedMenuItem;
}