/**
 * @file Helper.h
 * @brief A collection of fucntions used widely throughout the project.
 *
 * @author Mohammad El-Abid
 */

#include "SDL.h"
#include <string>

#ifdef _WIN32
#include <Windows.h> // OutputDebugString
#endif // _WIN32

#ifndef HELPER_H
#define HELPER_H

namespace Helper
{
    extern std::string resourcePath;
    
    /**
     * @brief Returns the proper path for a resource
     *
     * @param path to the item if relative path is the source folder
     * @return std::string the path to load
     */
    std::string get_path_for_resource( std::string filename );
    
    /**
     * @brief Loads a BMP image onto a SDL_Surface.
     *
     * @param filename The file to load.
     * @return SDL_Surface with the same width, height, and "image" of file.
     */
    SDL_Surface* load_image( std::string filename );

    /**
     * @brief Puts source surface onto the top of destination surface.
     *
     * @param x X coordnate to start the blip.
     * @param y Y coordnate to start the blip.
     * @param source Surface you want to paint.
     * @param detination Surface you want to paint on.
     * @param clip Select only a portion of the source surface, NULL for full surface.
     */
    void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );

    /**
     * @brief Converts a string to it's lowercase conterpart.
     *
     * @param orginal The string to return lowere cased.
     * @return The passed string with all A-Z characters lowercased.
     */
    std::string lower_case(std::string orginal);

    /**
     * @brief A Sprintf wrapper to print to debug window, will hopefully be cross paltform.
     *
     * @argument bufferSize Size of the character array to store the result of spritnf into.
     * @argument msg The format string.
     * @argument ... Suppliments for %i, etc.
     */
    void debug( int bufferSize, char* msg, ... );
}

#endif //HELPER_H