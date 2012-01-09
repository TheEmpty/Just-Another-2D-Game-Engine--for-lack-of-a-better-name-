#ifndef Platformer_PlayState_h
#define Platformer_PlayState_h

#include "GameState.h"
#include "GameStateHelper.h"
#include "SDL.h"
#include "Map.h"
#include "SDL_ttf.h"
#include "Helper.h"

class PlayState : public GameState
{
private:
    Uint32 backgroundColor;
    SDL_Surface* tile_layer;
    TTF_Font** font;
    Map map;
    int previous_state;
    GameStateHelper* state_helper;
    
public:
    PlayState( TTF_Font** newFont, int prev, const char filename[] );
    
    /**
     * @brief Frees the SDL_Surfaces used and any other responsabilites need to be performed for a clean deletion.
     */
    ~PlayState();
    
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
    
    void get_message( int command, std::string* response ) {};
};

#endif
