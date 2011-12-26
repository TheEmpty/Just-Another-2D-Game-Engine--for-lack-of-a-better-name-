// KEPT AS REFRENCE


/*
 
#include "Map.h"

Map::Map( std::string filename )
{
    //defaults
    name    = "N/A";
    creator = "N/A";

    //tile offsets
    int x = 0, y = 0, tiles_index = 0;

    //open map
    std::ifstream map_file = std::ifstream( filename.c_str() );

    //If the map file could not be loaded
    if( map_file == NULL )
    {
//        tiles = NULL;
        loaded = false;
    }
    else
    {
        std::string operation = "";
        std::string line;
        
        loaded = true; // set to false if we encounter any errors

        while( map_file.good() )
        {
            // get the next line to be parsed
            std::getline( map_file, line );
            
            // if it's a blank line or lacks a colon unless graphics, skip this iteration
            if( line.find_first_not_of( " \n\r\t" ) == -1 || ( line.find_first_of( ":" ) == -1 && operation != "graphics" ) )
                continue;

            // confirm key: value lines
            if( line[0] == ' ' && ( operation == "tileset" || operation == "information" || operation == "tiles" ) )
            {
                // no key name
                if( line[ line.find_first_not_of(' ') ] == ':' )
                {
                    loaded = false;
                    break;
                }

                // no value
                unsigned int offset = line.find_first_of(':') + 1;
                // there is nothing after the colon
                if( offset > line.length() )
                {
                    loaded = false;
                    break;
                }

                // only white space after colon
                if( line.find_first_not_of( " \n\r\t", offset ) == -1 )
                {
                    loaded = false;
                    break;
                }
            }

            // is the first character not a space? (new operation)
            if( line[0] != ' ' )
            {
                // then just change the operation and move on
                line.erase(line.find_last_of(":"));
                operation = Helper::lower_case(line); // lowercase, remove white space and colon
            }
            // set the tiles up!
            else if( operation == "graphics" )
            {
                // strip line
                int off = line.find_first_not_of( " " );
                line = line.substr(off, line.find_last_not_of( " " ) - off + 1);
                
                // while there are still tiles to be proccessed
                while( line.find_first_of("0123456789") != -1 )
                {
                    // find where to move "start" of line
                    int new_start = line.find_first_of( " " );
                    if( new_start == -1 )
                        new_start = line.length() - 1;

                    // parse number
                    int clip_number = atoi(line.substr(0, new_start).c_str());
                    // remove number from line
                    off = new_start + 1;
                    line = line.substr( off, line.length() - off );

                    // create tile
//					tiles[ tiles_index++ ] = * new Tile( x, y, tile_width, tile_height, clip_number );
                    x += tile_width;
                }
                y += tile_height;
                x = 0;
            }
            // these two are essentially maps
            else if( operation == "information" || operation == "tileset" )
            {
                // get the key by striping away the white spaces on the left and stoping at the colon
                std::string key = line.substr( line.find_first_not_of( " " ), line.find_last_of( ":" ) - 1);
                key = Helper::lower_case(key);

                // get the value
                int off = line.find_last_of( ":" ) + 1;
                std::string value = line.substr( off, line.length() - off );
                // strip lefthand whitespace
                off = value.find_first_not_of( " " );
                if( off != 0 )
                    value = value.substr( off, value.length() - off );
                // stright righthand side
                value = value.substr(0, value.find_last_not_of( " \n\r\t" ) + 1);

                if( operation == "information" )
                {
                    if( key == "name" )
                    {
                        name = value;
                    }
                    else if( key == "creator" )
                    {
                        creator = value;
                    }
                }
                else if( operation == "tileset" )
                {
                    if( key == "file" )
                    {
                        tileset = Helper::load_image(value);
                    }
                    else if( key == "width" )
                    {
                        tile_width = atoi(value.c_str());
                    }
                    else if( key == "height" )
                    {
                        tile_height = atoi(value.c_str());
                    }
                    else if( key == "tiles" )
                    {
                        total_tiles = atoi(value.c_str());
//						tiles = new Tile[total_tiles];
                    }
                    else if( key == "sprites" )
                    {
                        total_sprites = atoi(value.c_str());
                    }
                }
            }
        }
        if( tileset == NULL || tile_width == NULL || tile_height == NULL || total_tiles == NULL || total_sprites == NULL ){
            loaded = false;
        }
        make_clips();
    }
}

void Map::make_clips()
{
    clips = new SDL_Rect[ total_sprites ];

    int x = 0, y = 0;
    for( int i = 0; i < total_sprites; i++ )
    {
        clips[i].x = x;
        clips[i].y = y;
        clips[i].w = tile_width;
        clips[i].h = tile_height;

        y += tile_height;
        if(y >= tileset->h)
        {
            y = 0;
            x += tile_width;
        }
    }
}

bool Map::is_loaded()
{
    return loaded;
}


std::string Map::get_name()
{
    return name;
}

std::string Map::get_creator()
{
    return creator;
}

SDL_Surface* Map::get_tileset()
{
    return tileset;
}

void Map::show( SDL_Surface* screen, Camera* cam )
{
    for( int i = 0; i < total_tiles; i++ )
    {
//		tiles[ i ].show(tileset, screen, cam, &clips[ tiles[ i ].get_tile_number() ]);
    }
}
// void show(SDL_Surface *tiles, SDL_Surface *screen, Camera *cam, SDL_Rect *clip[]);

int Map::get_tile_height()
{
    return tile_height;
}

/*void Map::get_tiles(Tile **pointer)
{
    *pointer = tiles;
}*/ /*

int Map::get_total_tiles()
{
    return total_tiles;
} */