#include "PlayState.h"

// Should create a "select" state for this:
/* 
     Select Level
 
     My Map
     Your Map
     
     back
*/

PlayState::PlayState( TTF_Font** newFont, int prev, char filename[] )
{
    font = newFont;
    previous_state = prev;
    state_helper = GameStateHelper::Instance();
    map.load_map( filename );
    if( map.get_error()->empty() == false ) tile_layer = NULL;
}

PlayState::~PlayState()
{
    // free the tile surface
}

void PlayState::handle_event(const Window *window, SDL_Event *anEvent)
{
    if( map.get_error()->empty() == false )
    {
        // map did not load, if they hit enter or escape, send them back.
        // was it a key?
        if( anEvent->type == SDL_KEYDOWN )
        {
            // was it escape or return?
            if( anEvent->key.keysym.sym == SDLK_ESCAPE || anEvent->key.keysym.sym == SDLK_RETURN )
            {
                // if so, return to previous state
                state_helper->set_next_state( previous_state );
            }
        }
    }
}

void PlayState::logic(const Window *window)
{
}

void PlayState::render(const Window *window)
{
    SDL_FillRect( window->get_screen(), &window->get_screen()->clip_rect, SDL_MapRGB( window->get_screen()->format, 100, 99, 99 ) );
    
    if( tile_layer == NULL )
    {
        // display error
        SDL_Surface* error = TTF_RenderText_Solid( *font, map.get_error()->c_str(), defaultFontColor );
        int x = ( window->get_width() - error->w )/2;
        int y = ( window->get_height() - error->h )/4;
        Helper::apply_surface( x, y, error, window->get_screen() );
        
        // display black
        SDL_Surface* back = TTF_RenderText_Solid( *font, "Back", activeFontColor );
        x = ( window->get_width() - back->w )/2;
        y += y * 2;
        Helper::apply_surface( x, y, back, window->get_screen() );
        
        // free surfaces
        SDL_FreeSurface(error);
        SDL_FreeSurface(back);
    }
}
