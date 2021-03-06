#include "Window.h"
#include "Helper.h"

namespace JA2GE
{

    Window::Window(const char title[], int newWidth, int newHeight, int newBPP, bool fullScreen)
    {
        state_helper = GameStateHelper::Instance();
        if( fullScreen ) camera = new JA2GE::Camera(0,0,fullscreenWidth,fullscreenHeight);
        else             camera = new JA2GE::Camera(0,0,newWidth,newHeight);
        windowedWidth = newWidth;
        windowedHeight = newHeight;
        bpp = newBPP;

        SDL_WM_SetCaption( title, NULL );

        minimumWindowedHeight = minimumWindowedWidth = 0;
        errors = false;
        isFullscreen = fullScreen;
        create_screen();
        errors = ( screen == NULL );
        
    }

    bool Window::error()
    {
        return errors;
    }

    bool Window::fullscreen( void )
    {
        return isFullscreen;
    }

    void Window::fullscreen( bool goFull )
    {
        // can we free the surface?
        // Only recreate if switching
        if( isFullscreen == goFull )
            return;

        create_screen( goFull );
        errors = (screen == NULL);
        isFullscreen = goFull;
    }

    void Window::create_screen()
    {
        create_screen( isFullscreen );
    }

    void Window::create_screen( bool fullscreen )
    {
        // SDL_FreeSurface( screen );
        if( fullscreen )
        {
            screen = SDL_SetVideoMode( fullscreenWidth, fullscreenHeight, bpp, SDL_SWSURFACE | SDL_RESIZABLE | SDL_FULLSCREEN );
        }
        else
        {
            screen = SDL_SetVideoMode( windowedWidth, windowedHeight, bpp, SDL_SWSURFACE | SDL_RESIZABLE );
        }
        updateScreen = false;
        screenChange = true;
    }

    inline void Window::fullscreen( SDL_Event* anEvent )
    {
        if( anEvent->type == SDL_KEYDOWN )
        {
            bool control, command, f;
            control = command = f = false;
            SDL_keysym key = anEvent->key.keysym;

            f = ( key.sym == SDLK_f );
    #ifdef _WIN32
            control = ( key.mod & KMOD_LCTRL ) || ( key.mod & KMOD_RCTRL );
    #endif
    #ifdef __APPLE__
            // set command to false because SDL currently "crashes" Mac OS X when fullscreen
            command = false; // || ( key.mod & KMOD_LCOMMAND ) || ( key.mod & KMOD_RCOMMAND );
    #endif

            if( ( control && f ) || ( command && f ) ){
                fullscreen( !isFullscreen );
            }
        }
    }

    inline void Window::resize( SDL_Event* anEvent )
    {
        if( anEvent->type == SDL_VIDEORESIZE )
        {
            set_dim(anEvent->resize.w, anEvent->resize.h);
            errors = ( screen == NULL );
        }
    }

    inline void Window::close( SDL_Event* anEvent )
    {
        if( anEvent->type == SDL_QUIT )
        {
            state_helper->set_next_state( STATE_EXIT );
        }
        else if( anEvent->type == SDL_KEYDOWN )
        {
            bool w = false, close = false, quit = false;
            // w is used by all
             w = ( anEvent->key.keysym.sym == SDLK_w );

    // handle command + q or command + w
    #ifdef __APPLE__
            bool command, q;
            
            q = ( anEvent->key.keysym.sym == SDLK_q );
            command = ( anEvent->key.keysym.mod & KMOD_LCOMMAND ) || ( anEvent->key.keysym.mod & KMOD_RCOMMAND );
            
            if( command && q )
                quit = true;
            else if( command && w )
                close = true;
    #endif

    // handle alt-f4 or control+w on Windows
    #ifdef _WIN32
            bool alt, f4, control;
            alt = f4 = control = false;
            f4 = ( anEvent->key.keysym.sym == SDLK_F4 );
            control = ( anEvent->key.keysym.mod & KMOD_LCTRL ) || ( anEvent->key.keysym.mod & KMOD_RCTRL );
            alt = ( anEvent->key.keysym.mod & KMOD_LALT ) || ( anEvent->key.keysym.mod & KMOD_RALT );

            if( alt && f4 )
                quit = true;
            else if( control && w )
                close = true;
    #endif

            if( quit )
            {
                /* Post a SDL_QUIT event */
                SDL_Event event;
                event.type = SDL_QUIT;
                SDL_PushEvent(&event);
            }
            else if( close )
            {
                state_helper->set_next_state( STATE_EXIT );
            }
        }
    }

    void Window::handle_events()
    {
        SDL_Event anEvent;
        screenChange = false;

        while( SDL_PollEvent( &anEvent ) )
        {
            
            close( &anEvent );
            fullscreen( &anEvent );
            resize( &anEvent );
            
            if( updateScreen ) // did this iteration create a screen diference?
                create_screen();
            
            state_helper->currentState->handle_event( this, &anEvent );
        }
        
    }

    void Window::logic()
    {
        state_helper->currentState->logic( this );
    }

    void Window::render()
    {
        state_helper->currentState->render( this );
    }

    void Window::set_min_width( int newWidth )
    {
        minimumWindowedWidth = newWidth;
        if( windowedWidth < newWidth )
            updateScreen = true;
    }

    void Window::set_min_height( int newHeight )
    {
        minimumWindowedHeight = newHeight;
        if( windowedHeight < newHeight )
            updateScreen = true;
    }

    void Window::set_min_dim( int width, int height )
    {
        set_min_width( width );
        set_min_height( height );
        /*
        minimumWindowedWidth = width;
        minimumWindowedHeight = height;
        if( windowedHeight < height || windowedWidth < width )
            updateScreen = true;
        */
    }

    void Window::set_height( int newHeight )
    {
        if( minimumWindowedHeight != 0 )
        {
            if( newHeight > minimumWindowedHeight )
                windowedHeight = newHeight;
            else if( windowedHeight != minimumWindowedHeight )
                windowedHeight = minimumWindowedHeight;
        }
        else
        {
            // no min height
            if( newHeight != windowedHeight )
                windowedHeight = newHeight;
            else
                return; // no difference, don't update
        }
        camera->view.h = windowedHeight;
        updateScreen = true;
    }

    void Window::set_width(int newWidth)
    {
        if ( minimumWindowedWidth != 0 )
        {
            if( newWidth > minimumWindowedWidth )
                windowedWidth = newWidth;
            else if( windowedWidth != minimumWindowedWidth )
                windowedWidth = minimumWindowedWidth;
        }
        else
        {
            // no min width
            if( newWidth != windowedWidth )
                windowedWidth = newWidth;
            else
                return; // no difference, don't update
        }
        camera->view.w = windowedWidth;
        updateScreen = true;
    }

    // can't make this inline since it is an external function. Would be nice since this is just calling other methods.
    void Window::set_dim( int width, int height )
    {
        set_width( width );
        set_height( height );
    }
}