/**
 * @file Credits.cpp
 * @brief Implementation for the credits state. See header for more details.
 *
 * @author Mohammad El-Abid
 */

#include "Credits.h"

Credits::Credits( int prev, TTF_Font** newFont, int newPadding )
{
    state_helper = JA2GE::GameStateHelper::Instance();
    prev_state = prev;
    backgroundColor = 0;
    font = newFont;
    padding = newPadding;
    create_messages();
}

void Credits::create_messages()
{
    messages = 7;
    message = new SDL_Surface* [messages];
    // initalize
    for( int i = 0; i < messages; i++ )
        message[i] = SDL_CreateRGBSurface(0,0,0,0,0,0,0,0);
    
    message[0] = TTF_RenderText_Solid( *font, "Credits", defaultFontColor );
    message[1] = TTF_RenderText_Solid( *font, "Mohammad El-Abid - Developer & Designer", defaultFontColor );
    message[2] = TTF_RenderText_Solid( *font, "Lazy Foo for his tutorials", defaultFontColor );
    message[3] = TTF_RenderText_Solid( *font, "Joe Prince for the 'Squada One' font", defaultFontColor );
    message[4] = TTF_RenderText_Solid( *font, "And you, for playing my first C++ app", defaultFontColor);
    message[5] = NULL; // blank line, takes height of previous message
    message[6] = TTF_RenderText_Solid( *font, "Back", activeFontColor );
}

Credits::~Credits()
{
    // 4k memory leak?
    for( int i = 0; i < messages; i++ )
        SDL_FreeSurface( message[i] );
    delete [] message;
}

void Credits::logic( const JA2GE::Window* window )
{
    if( window->screenChange )
    {
        create_messages();
    }
}

void Credits::handle_event(const JA2GE::Window* window, SDL_Event* anEvent)
{
    // was it a key?
    if( anEvent->type == SDL_KEYDOWN )
    {
        // was it escape or return?
        if( anEvent->key.keysym.sym == SDLK_ESCAPE || anEvent->key.keysym.sym == SDLK_RETURN )
        {
            // if so, return to previous state
            state_helper->set_next_state( prev_state );
        }
    }
}

void Credits::render( const JA2GE::Window* window )
{
    if( backgroundColor == 0 ) backgroundColor = SDL_MapRGB( window->get_screen()->format, 100, 99, 99 );
    SDL_FillRect( window->get_screen(), &window->get_screen()->clip_rect, backgroundColor );

    int height = 0;
    for( int i = 0; i < messages; i++ )
        height += get_height(i) + padding; // height + padding

    // use this information to find the value to start at for Y to make them centerd horizontally
    int x, y = ( window->get_height() - height ) / 2;

    for( int i = 0; i < messages; i++ )
    {
        if( message[i] != NULL )
        {
            x = ( window->get_width() - message[i]->w ) / 2; // center it
            JA2GE::Helper::apply_surface( x, y, message[i], window->get_screen() );
            y += message[i]->h + padding;
        }
        else
        {
            y += get_height( i - 1 ); // we already know i is going to call i - 1
        }
    }
}

int Credits::get_height( int i )
{
    // Not fool proof, will go to index -1
    if( message[i] != NULL )
        return message[i]->h;
    else
        return get_height( i - 1 );
}