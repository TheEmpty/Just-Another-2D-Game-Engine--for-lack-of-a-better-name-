/**
 * @file Camera.h
 * @brief Declaration for the Camera class. An object used for creating the illusion of moving through a virtual word (in reality the world is moving while the camera is stationary).
 *
 * @author Mohammad El-Abid
 */

#include "SDL.h"

#ifndef JUST_ANOTHER_2D_GAME_ENGINE_CAMERA_H
#define JUST_ANOTHER_2D_GAME_ENGINE_CAMERA_H 

namespace JA2GE
{

    class Camera
    {
    public:
        SDL_Rect view;

        /**
         * @brief Creates camera object at specified values with speicifed size.
         *
         * @param x X position of camera with 0 being the left most pixel.
         * @param y Y position of camera with 0 being the top most pixel.
         * @param width The width of the view.
         * @param height The height of the view.
         */
        Camera( int x, int y, int width, int height );

        /**
         * @brief Mass update values of the internal SDL_Rect
         *
         * @param x X position of camera with 0 being the left most pixel.
         * @param y Y position of camera with 0 being the top most pixel.
         * @param w The width of the view
         * @param h The height of the view
         */
        void update( int x, int y, int w, int h );

        /**
         * @brief Copies the cordnates of an SDL_Rect for the camera's x, y and viewing w, h.
         *
         * @param box The SDL_Rect to clone.
         */
        void set_view( SDL_Rect box );

        /**
         * @brief Calculates the x and y values to place the passed SDL_Rect in the center of the viewport.
         *
         * @param box The SDL_Rect to center on.
         */
        void center_view( SDL_Rect box );
    };
}

#endif