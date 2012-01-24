/**
 * @file Camera.cpp
 * @brief Implementation for the Camera class. See header for more details on this object.
 *
 * @author Mohammad El-Abid
 */

#include "Camera.h"

namespace JA2GE
{

    Camera::Camera( int x, int y, int w, int h )
    {
        view.x = x;
        view.y = y;
        view.w = w;
        view.h = h;
    }

    void Camera::update( int x, int y, int h, int w )
    {
        view.x = x;
        view.y = y;
        view.h = h;
        view.w = w;
    }

    void Camera::set_view( SDL_Rect box )
    {
        view = box;
    }

    void Camera::center_view( SDL_Rect box )
    {
        view.x = box.x - ( view.w / 2 ) + ( box.w / 2 );
        view.y = box.y - ( view.h / 2 ) + ( box.h / 2 );
        
        if( view.x < 0 ) view.x = 0;
        if( view.y < 0 ) view.y = 0;
    }

}