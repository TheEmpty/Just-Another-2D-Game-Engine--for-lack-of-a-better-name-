/**
 * @file Title.cpp
 * @brief Main menu state, see header for more details.
 *
 * @author Mohammad El-Abid
 */

#include "Title.h"

Title::Title( TTF_Font** newFont, int newPadding )
{
    state_helper = GameStateHelper::Instance();
    font = newFont;
    padding = newPadding;
    
    background = Helper::load_image( Helper::get_path_for_resource( "images/title.bmp" ) );
    backgroundColor = 0;
    
    // Menu
    menu = new Menu( font, defaultFontColor, activeFontColor, 4 );
    menu->setText( 0, "Load Map" );
    menu->setText( 1, "Create Map" );
    menu->setText( 2, "Credits" );
    menu->setText( 3, "Exit" );
}

Title::~Title()
{
    delete menu; // frees surfaces
    SDL_FreeSurface( background );
}

void Title::handle_event(const Window* window, SDL_Event* anEvent)
{
    if( anEvent->type == SDL_KEYDOWN )
    {
        switch( anEvent->key.keysym.sym )
        {
            case( SDLK_ESCAPE ):
                state_helper->set_next_state( STATE_EXIT );
                break;
            case( SDLK_UP ):
            case( SDLK_DOWN ):
                menu->reactToKey( anEvent->key.keysym.sym );
                reaction.start();
                break;
            case( SDLK_RETURN ):
                switch( menu->getPos() )
                {
                    case( 0 ):
                        state_helper->set_next_state( STATE_SELECT_MAP );
                        break;
                    case( 2 ):
                        state_helper->set_next_state( STATE_CREDITS );
                        break;
                    case( 3 ):
                        state_helper->set_next_state( STATE_EXIT );
                        break;
                }
                break;
        }
    }
}

void Title::logic( const Window* window )
{
    if( window->screenChange )
        menu->redraw();
    
    if( reaction.get_ticks() >= 400 )
    {
        Uint8 *keystates = SDL_GetKeyState( NULL );
        if( keystates[SDLK_UP] )
        {
            menu->reactToKey( SDLK_UP );
            reaction.start();
        }
        else if( keystates[SDLK_DOWN] )
        {
            menu->reactToKey( SDLK_DOWN );
            reaction.start();
        }
    }
}

void Title::render( const Window* window )
{
    if( backgroundColor == 0 ) backgroundColor = SDL_MapRGB( window->get_screen()->format, 100, 99, 99 );
    SDL_FillRect( window->get_screen(), &window->get_screen()->clip_rect, backgroundColor );

    int x, y;
    x = ( window->get_width() - background->w ) / 2;
    y = ( window->get_height() - background->h ) / 2;
    Helper::apply_surface( x, y, background, window->get_screen() );
    
    y += 60;
    if( 0 > y ) y = 0;
    
    int max = menu->getLength(); // more effecient then calling the method every loop
    for( int i = 0; i < max; i++ )
    {
        SDL_Surface *item = menu->getMenuItem( i );
        x = ( window->get_width() - item->w ) / 2; // center it
        Helper::apply_surface( x, y, item, window->get_screen() );
        y += item->h + padding;
    }
}