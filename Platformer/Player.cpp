#include "Player.h"
#include "globals.h"

Player::Player()
{
    status = PLAYER_STAND;
    frame = xVel = yVel = 0.0;
    sheet = NULL;
    set_dim( 0, 0, PLAYER_WIDTH, PLAYER_HEIGHT );
    // TODO: this is test stuff ;)
    // Helper::debug ('cuz that's what I search for)
    set_dim( PLAYER_WIDTH * 2, PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT );
    // yVal = -5.0;
    // E/O test
    clipping.x = clipping.y = 0;
    clipping.w = PLAYER_WIDTH;
    clipping.h = PLAYER_HEIGHT;
    last_x = get_dim()->x;
    last_y = get_dim()->y;
}

void Player::init()
{
    sheet = JA2GE::Helper::load_image( JA2GE::Helper::get_path_for_resource( "images/player.bmp" ) );
    Uint32 colorKey = SDL_MapRGB( sheet->format, 0, 255, 0 );
    SDL_SetColorKey( sheet, SDL_SRCCOLORKEY, colorKey );
    error = (sheet == NULL);
}

Player::~Player()
{
    SDL_FreeSurface( sheet );
    sheet = NULL;
}

bool Player::change_state()
{
    PlayerStates prevStatus = status;
    
    // Falling?
    if( yVel > 0 )
    {
        if( xVel == 0 )     status = PLAYER_FALL_DOWN;
        else if( xVel < 0 ) status = PLAYER_FALL_LEFT;
        else                status = PLAYER_FALL_RIGHT;
    }
    // Jumping?
    else if( yVel < 0 )
    {
        if     ( xVel == 0 ) status = PLAYER_JUMP_UP;
        else if( xVel < 0 )  status = PLAYER_JUMP_LEFT;
        else                 status = PLAYER_JUMP_RIGHT;
    }
    // Going right?
    else if( xVel > 0 )
    {
        status = PLAYER_RIGHT;
    }
    // Going left?
    else if( xVel < 0 )
    {
        status = PLAYER_LEFT;
    }
    else
    {
        status = PLAYER_STAND;
    }
    
    return( prevStatus != status );
}

void Player::render( SDL_Surface *destitation, JA2GE::Camera* cam )
{
    // render the player according to the x,y cords, direction, and animation frame
    if( animation_timer.is_started() == false ) animation_timer.start();
    else if( animation_timer.get_ticks() >= 1000 / PLAYER_FRAMES_PER_SECOND )
    {
        animation_timer.start();
        if( change_state() )
        {
            clipping.y = status * PLAYER_HEIGHT;
            clipping.x = 0;
        }
        else
        {
            if( status != PLAYER_STAND )
            {
                clipping.x += PLAYER_WIDTH;
                if( clipping.x >= sheet->w ) clipping.x = 0;
            }
        }
    }
    
    int x = get_dim()->x - cam->view.x;
    int y = get_dim()->y - cam->view.y;
    if( error == false && x >= 0 && x <= destitation->w && y >= 0 && y <= destitation->h ){
        JA2GE::Helper::apply_surface( x, y, sheet, destitation, &clipping );
    }
}

void Player::move()
{
    // Have we already began?
    if( valvCalc.is_started() == false )
    {
        valvCalc.start();
        return;
    }
    
    int movement = valvCalc.get_ticks();
    valvCalc.start();
    
    int x = get_dim()->x;
    if( x != last_x ) last_x = x;
    if( xVel != 0 ) x += xVel * movement;
    
    int y = get_dim()->y;
    if( y != last_y ) last_y = y;
    if( yVel != 0 ) y += yVel * movement;
    
    set_dim( x, y );
}

void Player::unmove()
{
    get_dim()->x = last_x;
    get_dim()->y = last_y;
}

char* Player::c_str()
{
    char* buffer;
    buffer = new char[200];
    sprintf( buffer, "Player @ (%i,%i,%i,%i)", get_dim()->x, get_dim()->y, get_dim()->w, get_dim()->h );
    return buffer;
}