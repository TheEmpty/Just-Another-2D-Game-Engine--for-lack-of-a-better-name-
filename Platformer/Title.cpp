/**
 * @file Title.cpp
 * @brief Main menu state, see header for more details.
 *
 * @author Mohammad El-Abid
 */

#include "Title.h"

Title::Title( TTF_Font* newFont, int newPadding )
{
    state_helper = GameStateHelper::Instance();
    font = newFont;
    padding = newPadding;
    
    background = Helper::load_image( Helper::get_path_for_resource("images/title.bmp") );
    
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

void Title::handle_events()
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
                case( SDLK_UP ):
                case( SDLK_DOWN ):
                    menu->reactToKey( event.key.keysym.sym );
                    reaction.start();
                    break;
                case( SDLK_RETURN ):
                    switch( menu->getPos() )
                    {
                        case( 2 ):
                            state_helper->set_next_state( STATE_CREDITS );
                            break;
                        case( 3 ):
                            state_helper->set_next_state( STATE_EXIT );
                            break;
                    }
            }
        }
    }
}

void Title::logic()
{
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

void Title::render( SDL_Surface* screen )
{
    Helper::apply_surface( 0, 0, background, screen );
    
    int max = menu->getLength(); // more effecient then calling the method every loop
    int x, y = 60;
    for( int i = 0; i < max; i++ )
    {
        SDL_Surface *item = menu->getMenuItem( i );
        x = ( SCREEN_WIDTH - item->w ) / 2; // center it
        Helper::apply_surface( x, y, item, screen );
        y += item->h + padding;
    }
}