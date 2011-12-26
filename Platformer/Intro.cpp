/**
 * @file Intro.cpp
 * @brief Implementation for the intro state. See header for more details.
 *
 * @author Mohammad El-Abid
 */

#include "Intro.h"

Intro::Intro( TTF_Font* newFont ) // take font
{
    state_helper = GameStateHelper::Instance();
    font = newFont;

    //Load the background
    background = Helper::load_image( "images/intro.bmp" );
    message = TTF_RenderText_Solid( font, "Version 0.0.5", defaultFontColor );

    //Monitor how long they have looked at the version
    wait.start();
}

Intro::~Intro()
{
    //Free the surfaces
    SDL_FreeSurface( background );
    SDL_FreeSurface( message );
}

void Intro::handle_events()
{
    SDL_Event event;
    //While there's events to handle
    while( SDL_PollEvent( &event ) )
    {
        if( event.type == SDL_QUIT )
        {
            state_helper->set_next_state( STATE_EXIT );
        }
        // mouse click or key buttons = skip
        else if( ( event.type == SDL_KEYDOWN ) || ( event.type == SDL_MOUSEBUTTONDOWN ) )
        {
            state_helper->set_next_state( STATE_TITLE );
        }
    }
}

void Intro::logic()
{
    // 3sec max on intro
    if( wait.get_ticks() >= 3000 )
        state_helper->set_next_state( STATE_TITLE );
}

void Intro::render( SDL_Surface* screen )
{
    Helper::apply_surface( 0, 0, background, screen );
    Helper::apply_surface( 0, ( SCREEN_HEIGHT - message->h ), message, screen );
}