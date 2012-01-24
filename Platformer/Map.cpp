#include "Map.h"
#include "Helper.h"
#include <fstream>
#include <sstream>

namespace JA2GE
{

    void Map::load_map( const char filename[] )
    {
        line_number = 0;
        
        std::ifstream mapFile;
        mapFile.open( Helper::get_path_for_resource( std::string( filename ) ).c_str() );

        if (mapFile.is_open())
        {
            std::string working_directory = std::string( filename );
            working_directory.erase( working_directory.find_last_of("/\\") );
            
            std::string line;
            
            current_directive = NULL;

            while ( error.empty() && mapFile.good() )
            {
                getline( mapFile, line );
                line_number++;
                if( error.empty() && change_directive( &line ) == false ) parse_from_line( &line );
            }

            mapFile.close();
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
                // create tileset surface with clipings
                if( info[ "file" ].empty() || info[ "width" ].empty() || info[ "height" ].empty() || info[ "tile_width" ].empty() || info[ "tile_height" ].empty() ){
                    set_error( "Missing tileset file, width, height, tile_width, or tile_height." );
                } else {
                    // load surface
                    tileset = SDL_LoadBMP( Helper::get_path_for_resource( working_directory + info[ "file" ] ).c_str() );
                    // clipings
                    if( tileset != NULL )
                    {
                        int x = 0, y = 0;
                        int tile_width  = atoi( info[ "tile_width" ].c_str() );
                        int tile_height = atoi( info[ "tile_height" ].c_str() );
                        
                        // we have not exceded the height of the tileset
                        while( y < tileset->h )
                        {
                            // we have not exceded the width of the tileset
                            while( x < tileset->w )
                            {
                                SDL_Rect current;
                                current.x = x;
                                current.y = y;
                                current.w = tile_width;
                                current.h = tile_height;
                                clips.push_back( current );
                                x += tile_width;
                            }
                            y += tile_height;
                        }
                    }
                    else
                    {
                        set_error( "Unable to load tileset" );
                    }
                }
                // next directive
                current_directive = "tiles";
                return true;
            }
            else if( strncmp( current_directive, "tiles", 5 ) == 0 && stripped == "map" )
            {
                current_directive = "map";
                return true;
            }
            else if( strncmp( current_directive, "map", 3) == 0 && stripped == "events" )
            {
                // create physics
                for( int i = 0; i < map.size(); i++ )
                {
                    for( int j = 0; j < map[i].size(); j++ )
                    {
                        int tile = map[i][j];
                        
                        if( tile_physic_templates[ tile ] != NULL )
                        {
                        
                            GameObject* obj;
                            // Convert for deep copy
                            InvisableRectangle* test = (InvisableRectangle*) tile_physic_templates[ tile ];
                            
                            if( test != NULL ){
                                obj = new InvisableRectangle( *tile_physic_templates[ tile ] );
                            }
                            
                            if( obj != NULL )
                            {
                                obj->set_y( i * atoi(info["tile_height"].c_str()) );
                                obj->set_x( j * atoi(info["tile_width"].c_str()) );
                                stable_items.push_back( obj );
                            }
                        }
                    }
                }
                
                current_directive = "events";
                return true;
            }
            // cutscenes
        }
        return false;
    }

    void Map::parse_from_line( std::string* line )
    {
        if( line->empty() == false && line->find_first_not_of( " \n\r\t" ) != -1)
        {
            if( strncmp( current_directive, "information", 11 ) == 0 )
            {
                std::string key, value;
                get_key_val( line, &key, &value );
                if( key.empty() == false && value.empty() == false ) info[ key ] = value;
            }
            else if( strncmp( current_directive, "tileset", 7 ) == 0 )
            {
                std::string key, value;
                get_key_val( line, &key, &value );
                if( key.empty() == false && value.empty() == false ) info[ key ] = value;
            }
            else if( strncmp( current_directive, "tiles", 5) == 0 )
            {
                // 00: NULL
                // 01: rect( x, y, w, h)
                
                std::string key, value;
                get_key_val( line, &key, &value );
                int tile = atoi( key.c_str() );
                
                if( value != "NULL" )
                {
                    std::string command;
                    get_command( &value, &command );
                    if( command == "rect" ){
                        value = value.substr( value.find_first_of("("), value.length() - 1 );
                        std::string arguments[4];
                        get_arguments( &value, arguments, 4 );
                        
                        tile_physic_templates[ tile ] = new InvisableRectangle();
                        InvisableRectangle* rect;
                        rect = (InvisableRectangle*) tile_physic_templates[ tile ];
                        
                        rect->dim.x = std::atoi( arguments[0].c_str() );
                        rect->dim.y = std::atoi( arguments[1].c_str() );
                        rect->dim.w = std::atoi( arguments[2].c_str() );
                        rect->dim.h = std::atoi( arguments[3].c_str() );
                    }
                    // Circle command? x,y,r?
                }
            }
            else if( strncmp( current_directive, "map", 3 ) == 0 )
            {
                std::stringstream stream( line->c_str() );
                std::string item;
                std::vector<int> elements;
                
                while(std::getline(stream, item, ' ')) {
                    if( item.empty() == false ) elements.push_back( std::atoi(item.c_str()) );
                }
                
                if( elements.size() == 0 )
                {
                    set_error( "Blank line during map definition" );
                }
                else if( map.size() > 0 && map[0].size() != elements.size() )
                {
                    set_error( "Line does not have same length as first map line." );
                }
                else
                {
                    map.push_back( elements );
                }
            }
        }
    }

    void Map::get_command( std::string* line, std::string* command )
    {
        int start, end;
        
        // first non-whitespace, except new lines
        start = line->find_first_not_of(" \t");
        // first whitespace or '(' afterward
        end   = line->find_first_of(" \t(", start);
        
        if( start == -1 )
        {
            set_error( "Could not find command" );
        }
        else if( end == -1 )
        {
            set_error( "Could not find end of command" );
        }
        else
        {
            if( command->empty() == false ) command->clear();
            command->append( Helper::lower_case( line->substr( start, end ) ) );
        }
    }

    bool Map::get_arguments( std::string* arg_list, std::string arguments[], int size )
    {
        int start, end, i = 0;
        start = arg_list->find_first_not_of(" \t(");
        end   = arg_list->find_last_of(" \t\n\r)") - 1;
        
        std::stringstream stream( arg_list->substr( start, end ).c_str() );
        std::string item;
        
        while(std::getline(stream, item, ',')) {
            if( item.empty() == false )
            {

                if( size > i ) arguments[i].append( item );
                i++;
            }
        }
        
        if( i != size )
        {
            char buffer[150];
            sprintf( buffer, "Got %i arguments, expected %i", i, size );
            set_error( buffer );
            return false;
        }
        
        return true;
    }

    void Map::get_key_val( std::string* line, std::string* key, std::string* value )
    {
        int start, end;
        
        start = line->find_first_not_of( " \t" );
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
            key->clear();
            key->append( line->substr( start, end - start ) );
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
            if( value->empty() == false ) value->clear();
            value->append( line->substr( start, end - start ) );
        }
    }

    void Map::render(SDL_Surface *screen, Camera *camera)
    {
        
        // find x,y of top left corner, then draw from left to right
        int tile_width = atoi( info[ "tile_width" ].c_str() );
        int tile_height =  atoi( info[ "tile_height" ].c_str() );
        int x = (camera->view.x / tile_width);
        int y = (camera->view.y / tile_height);
        
        // while we have not reached the bottom of the screen
        while( ( y * tile_height ) < ( screen->h + camera->view.y ) )
        {
            if( y >= map.size() ) break; // or bottom of the map
            // while we have not reached the side of the screen
            while( ( x * tile_width ) < (screen->w + camera->view.h ) )
            {
                if( x >= map[0].size() ) break; // or the side of the map
                
                Helper::apply_surface((x * tile_width) - camera->view.x, (y * tile_height) - camera->view.y, tileset, screen, &clips[ map[y][x] ]);
                
                x += 1;
            }
            y += 1;
            x = 0;
        }
    }

    void Map::set_error( const char text[] )
    {
        char buffer[100];
        sprintf( buffer, "Line: %i, Error: ", line_number );
        std::string builder = buffer;
        builder.append( text );
        error = builder.c_str();
    }

    void Map::move()
    {
        for( int i = moving_items.size() - 1; i >= 0; i-- )
        {
            moving_items[i]->move(); // only check for collision if changed?
            
            // TODO: moving_items check against moving_items
            for( int j = stable_items.size() - 1; j >= 0; j-- )
            {
                if( moving_items[i]->collides( *stable_items[j] ) )
                {
                    moving_items[i]->unmove();
                    j = i = -1;
                }
            }
        }
    }
}