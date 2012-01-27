#include "SelectState.h"
#include <vector>
#include <iostream>

#ifdef _WIN32
#include "dirent_win32.h"
#endif
#ifdef __APPLE__
#include <dirent.h>
#endif

SelectState::SelectState(TTF_Font** newFont)
{
    font = newFont;
    state_helper = JA2GE::GameStateHelper::Instance();
    backgroundColor = 0;
    load_files();
}

SelectState::~SelectState()
{
    delete menu;
}

void SelectState::load_files()
{
    // load files, make menu from files
    std::vector <std::string> files;
    DIR *dp = opendir( JA2GE::Helper::get_path_for_resource("maps").c_str() );
    struct dirent *dirp;
    
    if( dp == NULL ){
        JA2GE::Helper::debug(150, "Unable to open folder %s.", JA2GE::Helper::get_path_for_resource("maps").c_str());
        menu = NULL;
    } else {

        while ( ( dirp = readdir( dp ) ) != NULL ){
            std::string dir = std::string( dirp->d_name );
            if( dir.compare( "." ) != 0 && dir.compare( ".." ) != 0 ) files.push_back( dir );
        }

        closedir(dp);
        
        menu = new Menu( font, DEFAULT_FONT_COLOR, ACTIVE_FONT_COLOR, files.size());
        
        for( int i = 0; i < files.size(); i++ ) menu->setText(i, files[i]);
    }
}

void SelectState::handle_event(const JA2GE::Window* window, SDL_Event* anEvent)
{
    if( anEvent->type == SDL_KEYDOWN )
    {
        switch( anEvent->key.keysym.sym )
        {
            case( SDLK_ESCAPE ):
                state_helper->set_next_state( STATE_TITLE );
                break;
            case( SDLK_UP ):
            case( SDLK_DOWN ):
                menu->reactToKey( anEvent->key.keysym.sym );
                reaction.start();
                break;
            case( SDLK_RETURN ):
                state_helper->set_next_state( STATE_MAP );
                break;
        }
    }
}

void SelectState::get_selected_file( std::string* file )
{
    file->clear();
    file->append("maps/");
    file->append( menu->getText( menu->getPos() )->c_str() );
}

void SelectState::logic( const JA2GE::Window* window )
{
    if( window->screenChange )
        menu->redraw();
    
    if( reaction.get_ticks() >= 350 )
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

void SelectState::render( const JA2GE::Window* window )
{
    if( backgroundColor == 0 ) backgroundColor = SDL_MapRGB( window->get_screen()->format, 100, 99, 99 );
    SDL_FillRect( window->get_screen(), &window->get_screen()->clip_rect, backgroundColor );
    
    int x, max;
    int y = 8;
    int i = 0;
    
    if( menu == NULL ){
        max = 0;
        state_helper->set_next_state( STATE_TITLE );
    } else {
        max = menu->getLength();
        int displayable = window->get_height() / ( 8 + menu->getMenuItem(0)->h ) ;
        i = ( menu->getPos() - ( displayable / 2 ) );
        if( i < 0 ) i = 0;
        if( i > max) i = max; // TODO: don't think this is possiable is it?
    }

    while( i < max && max > 0 && y < window->get_height() )
    {
        SDL_Surface *item = menu->getMenuItem( i );
        x = ( window->get_width() - item->w ) / 2; // center it
        JA2GE::Helper::apply_surface( x, y, item, window->get_screen() );
        y += item->h + 8;
        i++;
    }
}