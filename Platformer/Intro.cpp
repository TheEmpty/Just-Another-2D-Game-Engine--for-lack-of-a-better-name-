/**
 * @file Intro.cpp
 * @brief Implementation for the intro state. See header for more details.
 *
 * @author Mohammad El-Abid
 */

#include "Intro.h"

Intro::Intro( TTF_Font** newFont ) // take font
{
    state_helper = JA2GE::GameStateHelper::Instance();
    font = newFont;

    //Load the background
    background = JA2GE::Helper::load_image( JA2GE::Helper::get_path_for_resource("images/intro.bmp").c_str() );
    message = TTF_RenderText_Solid( *font, "Version 0.0.5", DEFAULT_FONT_COLOR );

    //Monitor how long they have looked at the version
    wait.start();
}

Intro::~Intro()
{
    //Free the surfaces
    SDL_FreeSurface( background );
    SDL_FreeSurface( message );
}

void Intro::handle_event(const JA2GE::Window* window, SDL_Event* anEvent)
{
    // mouse click or key buttons = skip
    if( ( anEvent->type == SDL_KEYDOWN ) || ( anEvent->type == SDL_MOUSEBUTTONDOWN ) )
    {
        state_helper->set_next_state( STATE_TITLE );
    }
}

void Intro::logic( const JA2GE::Window* window )
{
    // 3sec max on intro
    if( wait.get_ticks() >= 3000 )
        state_helper->set_next_state( STATE_TITLE );
}

void Intro::render( const JA2GE::Window* window )
{
    JA2GE::Helper::apply_surface( 0, 0, background, window->get_screen() );
    JA2GE::Helper::apply_surface( 0, ( window->get_height() - message->h ), message, window->get_screen() );
}