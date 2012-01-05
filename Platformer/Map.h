#include <fstream>
#include <string>
#include <map>

#ifndef MAP_H
#define MAP_H

class Map
{
private:
    std::string error;
    std::map <std::string, std::string> info;
    
    int line_number;
    const char* current_directive;
    
    void set_error( const char text[] );
    bool change_directive( std::string* line );
    void parse_from_line( std::string* line );
    std::string* get_key_val( std::string* line );
public:
    const std::string* get_error() { return &error; }
    void load_map( const char[] );
};

#endif // MAP_H