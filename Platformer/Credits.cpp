/**
 * @file Credits.cpp
 * @brief Implementation for the credits state. See header for more details.
 *
 * @author Mohammad El-Abid
 */

#include "Credits.h"

Credits::Credits( int prev, TTF_Font* newFont, int newPadding )
{
    state_helper = GameStateHelper::Instance();
    prev_state = prev;
    font = newFont;
    padding = newPadding;
    
    messages = 7;
    message = new SDL_Surface* [messages];
    for( int i = 0; i < messages; i++ )
        message[i] = SDL_CreateRGBSurface(0,0,0,0,0,0,0,0);
    
    message[0] = TTF_RenderText_Solid( font, "Credits", defaultFontColor );
    message[1] = TTF_RenderText_Solid( font, "Mohammad El-Abid - Developer", defaultFontColor );
    message[2] = TTF_RenderText_Solid( font, "Lazy Foo for his tutorials", defaultFontColor );
    message[3] = TTF_RenderText_Solid( font, "Joe Prince for the 'Squada One' font", defaultFontColor );
    message[4] = NULL; // blank line, takes height of previous message
    message[5] = NULL; // blank line, takes height of previous message
    message[6] = TTF_RenderText_Solid( font, "Back", activeFontColor );
}

Credits::~Credits()
{
    // 4k memory leak?
    for( int i = 0; i < messages; i++ )
        SDL_FreeSurface( message[i] );
    delete [] message;
}

void Credits::logic()
{
}

void Credits::handle_events()
{
    SDL_Event event;
    while( SDL_PollEvent( &event ) )
    {
        if( event.type == SDL_QUIT )
        {
            state_helper->set_next_state( STATE_EXIT );
        }
        else if( event.type == SDL_KEYDOWN )
        {
            switch( event.key.keysym.sym )
            {
                case( SDLK_ESCAPE ):
                    state_helper->set_next_state( STATE_EXIT );
                    break;
                case( SDLK_RETURN ):
                    state_helper->set_next_state( prev_state );
                    break;
            }
        }
    }
}

void Credits::render( SDL_Surface* screen )
{
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 100, 99, 99 ) );

    int height = 0;
    for( int i = 0; i < messages; i++ )
        height += get_height(i) + padding; // height + padding

    // use this information to find the value to start at for Y to make them centerd horizontally
    int x, y = ( SCREEN_HEIGHT - height ) / 2;

    for( int i = 0; i < messages; i++ )
    {
        if( message[i] != NULL )
        {
            x = ( SCREEN_WIDTH - message[i]->w ) / 2; // center it
            Helper::apply_surface( x, y, message[i], screen );
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