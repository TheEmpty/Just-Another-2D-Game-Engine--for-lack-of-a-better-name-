// KEPT AS REFRENCE

#include "Camera.h"
#include "Helper.h"
#include <fstream>
#include "SDL.h"
#include <string>

#ifndef MAP_H
#define MAP_H

class Map
{
private:
    std::string name;
    std::string creator;
    //Tile* tiles;
    SDL_Rect *clips;
    SDL_Surface *tileset;
    int tile_width;
    int tile_height;
    int total_sprites;
    int total_tiles;
    bool loaded;
    void Map::make_clips();

public:
    Map::Map(std::string filename);
    bool is_loaded();
    SDL_Surface* get_tileset();
    void show(SDL_Surface* screen, Camera* cam);

    std::string get_name();
    std::string get_creator();

    int get_tile_height();
    int get_total_tiles();
};

#endif MAP_H