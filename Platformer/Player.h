#ifndef PLAYER_H
#define PLAYER_H

#include "SDL.h"
#include "InvisableRectangle.h"
#include "Helper.h"
#include "Timer.h"
#include "Map.h"
#include "Camera.h"

const int PLAYER_WIDTH  = 64;
const int PLAYER_HEIGHT = 205;
const int PLAYER_FRAMES_PER_SECOND = 8;

enum PlayerStates { 
    PLAYER_STAND,
    PLAYER_RIGHT,
    PLAYER_LEFT,
    PLAYER_FALL_DOWN,
    PLAYER_FALL_RIGHT,
    PLAYER_FALL_LEFT,
    PLAYER_JUMP_UP,
    PLAYER_JUMP_RIGHT,
    PLAYER_JUMP_LEFT
};

class Player : public JA2GE::InvisableRectangle
{
private:
    double xVel, yVel;
    int last_x, last_y;
    PlayerStates status;
    int frame;
    bool error;
    Timer valvCalc;
    Timer animation_timer;
    SDL_Surface* sheet;
    SDL_Rect clipping;
    bool change_state();
    
public:
    Player();
    ~Player();
    double getXVel() { return xVel; }
    double getYVel() { return yVel; }
    void setXVel( double nxVel ){ xVel = nxVel / 60; }
    void setYVel( double nyVel ){ yVel = nyVel / 60; }
    void render( SDL_Surface *destitation, JA2GE::Camera* cam );
    void init();
    void move();
    void unmove();
    char* c_str();
};

#endif // PLAYER_H