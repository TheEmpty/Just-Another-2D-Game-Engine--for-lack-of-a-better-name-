// TODO: file comment header

#include "Window.h"
#include "Helper.h"

Window::Window(const char title[], int newWidth, int newHeight, int newBPP)
{
    state_helper = GameStateHelper::Instance();
    windowedWidth = newWidth;
    windowedHeight = newHeight;
    bpp = newBPP;

    errors = false;
    screen = SDL_SetVideoMode( windowedWidth, windowedHeight, bpp, SDL_SWSURFACE | SDL_RESIZABLE );
    errors = (screen == NULL);

    isFullscreen = false;
    SDL_WM_SetCaption(title, NULL);
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
    SDL_FreeSurface( screen );
    if( fullscreen )
    {
        screen = SDL_SetVideoMode( fullscreenWidth, fullscreenHeight, bpp, SDL_SWSURFACE | SDL_RESIZABLE | SDL_FULLSCREEN );
    }
    else
    {
        screen = SDL_SetVideoMode( windowedWidth, windowedHeight, bpp, SDL_SWSURFACE | SDL_RESIZABLE );
    }
}

inline void Window::fullscreen( SDL_Event* anEvent )
{
    if( anEvent->type == SDL_KEYDOWN )
    {
        bool control, command, f;
        control = command = f = false;
        SDL_keysym key = anEvent->key.keysym;

        f = (key.sym == SDLK_f);
#ifdef _WIN32
        control = ( key.mod & KMOD_LCTRL ) || ( key.mod & KMOD_RCTRL );
#endif
#ifdef __APPLE__
        command = ( key.mod & SDLK_LMETA ) || ( key.mod & SDLK_RMETA );
#endif

        if( (control && f) || (command & f) ){
            fullscreen( !isFullscreen );
        }
    }
}

inline void Window::resize( SDL_Event* anEvent )
{
    if( anEvent->type == SDL_VIDEORESIZE )
    {
        windowedWidth = anEvent->resize.w;
        windowedHeight = anEvent->resize.h;
        create_screen();
        errors = (screen == NULL);
    }
}

void Window::handle_events()
{
    SDL_Event anEvent;

    while( SDL_PollEvent( &anEvent ) )
    {
        if( anEvent.type == SDL_QUIT )
            state_helper->set_next_state( STATE_EXIT );
        // Continue, in case one saves data or tries to prevent it
        fullscreen(&anEvent);
        resize(&anEvent);
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