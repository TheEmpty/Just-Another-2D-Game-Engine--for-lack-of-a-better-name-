// TODO: ~Map

#ifndef JUST_ANOTHER_2D_GAME_ENGINE_MAP_H
#define JUST_ANOTHER_2D_GAME_ENGINE_MAP_H

#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "SDL.h"
#include "Camera.h"
#include "GameObject.h"
#include "InvisableRectangle.h"

namespace JA2GE
{

    class Map
    {
    private:
        std::string working_directory;
        std::string error;
        std::vector <SDL_Rect> clips;
        std::vector < std::vector <int> > map;
        std::map <int, GameObject*> tile_physic_templates;
        SDL_Surface* tileset;
        
        int line_number;
        const char* current_directive;
        
        void set_error( const char text[] );
        bool change_directive( std::string* line );
        void parse_from_line( std::string* line );
        void get_command( std::string* line, std::string* command );
        bool get_arguments( std::string* arg_list, std::string arguments[], int size );
        void get_key_val( std::string* line, std::string* key, std::string* value );

    public:
        std::vector <GameObject*> moving_items;
        std::vector <GameObject*> stable_items;
        std::map <std::string, std::string> info;
        
        const std::string* get_error() { return &error; }
        void render( SDL_Surface* screen, Camera* camera );
        void load_map( const char[] );
        void move();
    };
}

#endif // JUST_ANOTHER_2D_GAME_ENGINE_MAP_H