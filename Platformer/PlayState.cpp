#include "PlayState.h"

PlayState::PlayState( TTF_Font** newFont, int prev, const char filename[] )
{
    font = newFont;
    previous_state = prev;
    paused = false;
    pausedMenu = NULL;
    state_helper = JA2GE::GameStateHelper::Instance();
    backgroundColor = 0;
    
    map.load_map( filename );
    if( map.get_error()->empty() )
    {
        player.init();
        map.moving_items.push_back( &player );
    }
}

PlayState::~PlayState()
{
    if( pausedMenu != NULL ) delete pausedMenu;
}

void PlayState::handle_event(const JA2GE::Window *window, SDL_Event *anEvent)
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
    else
    {
        // Normal controls
        if( anEvent->type == SDL_KEYDOWN )
        {
            switch( anEvent->key.keysym.sym )
            {
                case( SDLK_ESCAPE ):
                    paused = !paused;
                    if( pausedMenu != NULL && paused == false ) delete pausedMenu;
                    break;
                case( SDLK_UP ):
                case( SDLK_DOWN ):
                    if( paused ) pausedMenu->reactToKey( anEvent->key.keysym.sym );
                    break;
                case( SDLK_RETURN ):
                    if ( paused )
                    {
                        switch( pausedMenu->getPos() )
                        {
                            case( 0 ):
                                paused = false;
                                delete pausedMenu;
                                break;
                            case ( 1 ):
                                state_helper->set_next_state( STATE_SELECT_MAP );
                                break;
                            case ( 2 ):
                                state_helper->set_next_state( STATE_TITLE );
                                break;
                        }
                    }
                    break;
            }
        }
    }
}

void PlayState::logic(const JA2GE::Window *window)
{
    if( paused && pausedMenu == NULL )
    {
        pausedMenu = new Menu( font, defaultFontColor, activeFontColor, 3 );
        pausedMenu->setText( 0, "Resume" );
        pausedMenu->setText( 1, "Reselect" );
        pausedMenu->setText( 2, "Main Menu" );
    } else {
        // check button downs, change player velocity
        Uint8 *keystates = SDL_GetKeyState( NULL );
        
        player.setXVel( 0 );
        player.setYVel( 0 );
        
        if( !paused )
        {
            if     ( keystates[ SDLK_UP ]    || keystates[ SDLK_w ] ) player.setYVel( -5.0 );
            else if( keystates[ SDLK_DOWN ]  || keystates[ SDLK_s ] ) player.setYVel(  5.0 );
            if     ( keystates[ SDLK_LEFT ]  || keystates[ SDLK_a ] ) player.setXVel( -5.0 );
            else if( keystates[ SDLK_RIGHT ] || keystates[ SDLK_d ] ) player.setXVel(  5.0 );
            
            if( keystates[ SDLK_RSHIFT ] || keystates[ SDLK_LSHIFT ] )
            {
                player.setYVel( player.getYVel() * FRAMES_PER_SECOND * 2.5 );
                player.setXVel( player.getXVel() * FRAMES_PER_SECOND * 2.5 );
            }
            
            // move everything on the map (Players)
            map.move();
        }
    }
}

void PlayState::render(const JA2GE::Window *window)
{
    window->get_camera()->center_view( *player.get_dim() );
    
    if( backgroundColor == 0 ) backgroundColor = SDL_MapRGB( window->get_screen()->format, 100, 99, 99 );
    SDL_FillRect( window->get_screen(), &window->get_screen()->clip_rect, backgroundColor );
    
    if( map.get_error()->empty() )
    {
        map.render( window->get_screen(), window->get_camera() );
        player.render( window->get_screen(), window->get_camera() );
        if( paused && pausedMenu != NULL )
        {
            // render pausedMenu
            int x, y = 60;
            int max = pausedMenu->getLength();
            for( int i = 0; i < max; i++ )
            {
                SDL_Surface *item = pausedMenu->getMenuItem( i );
                x = ( window->get_width() - item->w ) / 2; // center it
                JA2GE::Helper::apply_surface( x, y, item, window->get_screen() );
                y += item->h + 60;
            }
        }
    }
    else
    {
        // display error
        SDL_Surface* error = TTF_RenderText_Solid( *font, map.get_error()->c_str(), defaultFontColor );
        int x = ( window->get_width() - error->w )/2;
        int y = ( window->get_height() - error->h )/4;
        JA2GE::Helper::apply_surface( x, y, error, window->get_screen() );
        
        // display back
        SDL_Surface* back = TTF_RenderText_Solid( *font, "Back", activeFontColor );
        x = ( window->get_width() - back->w )/2;
        y += y * 2;
        JA2GE::Helper::apply_surface( x, y, back, window->get_screen() );
        
        // free surfaces
        SDL_FreeSurface(error);
        SDL_FreeSurface(back);
    }
}
