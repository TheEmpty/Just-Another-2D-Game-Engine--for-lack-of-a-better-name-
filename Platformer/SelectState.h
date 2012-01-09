#ifndef Platformer_SelectState_h
#define Platformer_SelectState_h

#include "GameState.h"
#include "GameStateHelper.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "Menu.h"
#include "Timer.h"

class SelectState : public GameState
{
private:
    Uint32 backgroundColor;
    TTF_Font** font; // pointer to a pointer
    SDL_Surface** list; // array of SDL_Surface pointers
    Menu* menu;
    GameStateHelper* state_helper;
    // Used to tell how long since last reaction to held key.
    Timer reaction;
    
    void load_files();
    
public:
    SelectState(TTF_Font** font);
    void get_selected_file( std::string* file );
    
    /**
     * @brief Frees the SDL_Surfaces used and any other responsabilites need to be performed for a clean deletion.
     */
    ~SelectState();
    
    /**
     * @brief Handle "events" generally user input.
     */
    void handle_event(const Window* window, SDL_Event* anEvent);
    
    /**
     * @brief Handle any logic such as physics or autosave.
     */
    void logic( const Window* window );
    
    /**
     * @brief Renders graphics to the supplied surface.
     *
     * @param screen The SDL_Surface to draw on, generally the surface initalized by SDL.
     */
    void render( const Window* window );

    void get_message( int command, std::string* response ) { get_selected_file( response ); };
};

#endif
