#ifndef MAP_H
#define MAP_H

#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "SDL.h"
#include "Camera.h"

class Map
{
private:
    std::string working_directory;
    std::string error;
    std::vector <SDL_Rect> clips;
    std::vector < std::vector <int> > map;
    SDL_Surface* tileset;
    
    int line_number;
    const char* current_directive;
    
    void set_error( const char text[] );
    bool change_directive( std::string* line );
    void parse_from_line( std::string* line );
    void get_key_val( std::string* line, std::string* key, std::string* value );

public:
    std::map <std::string, std::string> info;
    
    const std::string* get_error() { return &error; }
    void render( SDL_Surface* screen, Camera* camera );
    void load_map( const char[] );
};

#endif // MAP_H