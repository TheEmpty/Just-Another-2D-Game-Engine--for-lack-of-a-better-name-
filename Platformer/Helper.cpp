/**
 * @file Helper.cpp
 * @brief Definitions of methods used widely thoughout the project.
 *
 * @author Mohammad El-Abid
 */

#include "Helper.h"

namespace Helper
{
    std::string resourcePath;
}

std::string Helper::lower_case( std::string orginal )
{
    std::string dupe = orginal;

    for( unsigned int i = 0; i < orginal.length(); i++ )
    {
        char op = orginal[i];
        if( op <= 'Z' && op >= 'A' )
        {
            dupe[i] = op - ( 'Z' - 'z' );
        }
    }
    
    return dupe;
}

std::string Helper::get_path_for_resource( std::string item )
{
    return resourcePath + "/" + item;
}

// This function was written by "Lazy Foo" and taken from his site (lazyfoo.net) with permission
SDL_Surface* Helper::load_image( std::string filename )
{
    // The image that's loaded
    SDL_Surface* loadedImage = NULL;

    // The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    // Load the image
    loadedImage = SDL_LoadBMP( filename.c_str() );

    // If the image loaded
    if( loadedImage != NULL )
    {
        // Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        // Free the old surface
        SDL_FreeSurface( loadedImage );

        // If the surface was optimized
        if( optimizedImage != NULL )
            // Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
    }

    // Return the optimized surface
    return optimizedImage;
}

// This function was written by "Lazy Foo" and taken from his site (lazyfoo.net) with permission
void Helper::apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

void Helper::debug( int bufferSize, char* msg, ... )
{
    va_list arg;
    char* buffer;
    buffer = new char[bufferSize];
    va_start( arg, msg );
    vsnprintf( buffer, bufferSize, msg, arg );
    va_end( arg );
#ifdef _WIN32
    strcat( buffer, "\n" );
    OutputDebugString( buffer );
#endif
#ifdef __APPLE__
    printf( "%s\n", buffer );
#endif
}