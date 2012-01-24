#ifndef JUST_ANOTHER_2D_GAME_ENGINE_InvisableRectangle_h
#define JUST_ANOTHER_2D_GAME_ENGINE_InvisableRectangle_h

#include "GameObject.h"
#include "SDL.h"
#include "Helper.h"

namespace JA2GE
{

    class InvisableRectangle : public GameObject
    {
    public:
        SDL_Rect dim;
        bool collides( GameObject& thing );
        void move() { return; };
        void unmove() { return; };
        char* c_str();
        SDL_Rect* get_dim() { return &dim; }
        void set_dim(int x = -1, int y = -1, int w = -1, int h = -1)
        {
            if( x >= 0 ) dim.x = x;
            if( y >= 0 ) dim.y = y;
            if( w >= 0 ) dim.w = w;
            if( h >= 0 ) dim.h = h;
        }
        void set_x(int x){ set_dim( x ); }
        void set_y(int y){ set_dim( -1, y ); }
        
        InvisableRectangle(){}
        InvisableRectangle( const GameObject& otherObj )
        {
            GameObject* cast = const_cast<GameObject*>(&otherObj);
            InvisableRectangle* test = (InvisableRectangle*) cast;
            if( test != NULL ) dim = test->dim;
        }
    };
}

#endif
