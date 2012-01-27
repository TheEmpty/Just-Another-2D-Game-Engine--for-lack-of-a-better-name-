#include "InvisableRectangle.h"
#include "Helper.h"

namespace JA2GE
{
    // TODO: seems in the GIT rollback I lost collides(GameObject &thing)
    // which would move objects to non-collision, replacing unmove and
    // collides for movement.

    bool InvisableRectangle::collides(GameObject &thing)
    {
        // InvisableRectangle
        InvisableRectangle* rect = (InvisableRectangle *) &thing;
        if( rect != NULL )
        {
            //The sides of the rectangles
            int leftA,   leftB;
            int rightA,  rightB;
            int topA,    topB;
            int bottomA, bottomB;
            
            //Calculate the sides of rect A
            leftA = get_dim()->x;
            rightA = get_dim()->x + get_dim()->w;
            topA = get_dim()->y;
            bottomA = get_dim()->y + get_dim()->h;
            
            //Calculate the sides of rect B
            leftB = rect->get_dim()->x;
            rightB = rect->get_dim()->x + rect->get_dim()->w;
            topB = rect->get_dim()->y;
            bottomB = rect->get_dim()->y + rect->get_dim()->h;
            
            //If any of the sides from A are outside of B
            if( bottomA <= topB )    return false;
            if( topA    >= bottomB ) return false;
            if( rightA  <= leftB )   return false;
            if( leftA   >= rightB )  return false;
            
            //If none of the sides from A are outside B
            return true;
        }
        
        // InvisableCircle
        // default
        return false;
    }

    char* InvisableRectangle::c_str()
    {
        char* buffer;
        buffer = new char[200];
        sprintf( buffer, "InvisableRectangle @ (%i,%i,%i,%i)", get_dim()->x, get_dim()->y, get_dim()->w, get_dim()->h );
        return buffer;
    }
}