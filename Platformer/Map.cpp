#include "Map.h"
#include "Helper.h"
#include <fstream>

void Map::load_map( const char filename[] )
{
    std::ifstream mapFile;
    mapFile.open( Helper::get_path_for_resource( std::string( filename ) ).c_str() );
    line_number = 0;

    if (mapFile.is_open())
    {
        std::string line;
        current_directive = NULL;

        while ( error.empty() && mapFile.good() )
        {
            getline( mapFile, line );
            line_number++;
            if( error.empty() && change_directive( &line ) == false ) parse_from_line( &line );
        }

        mapFile.close();
        Helper::debug(150, "Loaded: %s", info[ "name" ].c_str() );
    }
    else
    {
        set_error( "Unable to load file." ); // Line 0
    }
}

bool Map::change_directive( std::string* line )
{
    if( line->empty() == false && line->find_first_of( " \n\r\t" ) != 0 )
    {
        // copy the line so we can start striping it
        std::string stripped = *line;

        int index = stripped.find_first_not_of( " \n\r\t" );
        if( index != 0 )
            stripped = stripped.substr( index, stripped.length() - index );

        index = stripped.find_last_of( ":" );
        if( index == -1 )
        {
            set_error( "Directive is missing a colon." );
            return false;
        } else {
            stripped = stripped.substr( 0, index );
        }

        // ensure, Information -> Tileset -> Tiles -> Map -> Events -> Cutscenes
        stripped = Helper::lower_case( stripped );
        if( stripped == "information" )
        {
            current_directive = "information";
            return true;
        }
        // how come strncmp on current_directive, but not on stripped? Shouldn't string use compare?
        else if( strncmp( current_directive, "information", 11 ) == 0 && stripped == "tileset" )
        {
            current_directive = "tileset";
            return true;
        }
        else if( strncmp( current_directive, "tileset", 7 ) == 0 && stripped == "tiles" )
        {
            current_directive = "tiles";
            return true;
        }
        else if( strncmp( current_directive, "tiles", 5 ) == 0 && stripped == "map" )
        {
            current_directive = "map";
            return true;
        }
        // events, cutscenes
        return false;
    }
}

void Map::parse_from_line( std::string* line )
{
    if( strncmp( current_directive, "information", 11 ) == 0 )
    {
        std::string* keyv = get_key_val( line );
        if( keyv[0].empty() == false && keyv[1].empty() == false ) info[ keyv[0] ] = keyv[1];
    }
}

std::string* Map::get_key_val( std::string* line )
{
    std::string key_val[2];
    int start, end;
    
    start = line->find_first_not_of( " " );
    end = line->find_last_of( ":" );
    if( start == -1 )
    {
        set_error( "Invalid start of key." );
    }
    else if( end == -1 )
    {
        set_error( "Invalid key, value format" );
    }
    else
    {
        key_val[0] = line->substr( start, end - start );
    }
    
    start = line->find_first_not_of(" ", end + 1);
    end = line->find_last_of( " \n\r" );
    if( start == -1 )
    {
        set_error( "Invalid start of value." );
    }
    else if( end == -1 )
    {
        set_error( "Unable to parse value" );
    }
    else
    {
        key_val[1] = line->substr( start, end - start );
    }
    
    Helper::debug(250, "%s: %s", key_val[0].c_str(), key_val[1].c_str());

    return key_val;
}

void Map::set_error( const char text[] )
{
    char buffer[100];
    sprintf( buffer, "Line: %i, Error: ", line_number );
    std::string builder = buffer;
    builder.append( text );
    error = builder.c_str();
}

// Vector <key, value> of "name: bob"