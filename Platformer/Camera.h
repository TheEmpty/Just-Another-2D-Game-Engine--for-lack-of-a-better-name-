/**
 * @file Camera.h
 * @brief Declaration for the Camera class. An object used for creating the illusion of moving through a virtual word (in reality the world is moving while the camera is stationary).
 *
 * @author Mohammad El-Abid
 */

#include "globals.h"
#include "SDL.h"

#ifndef CAMERA_H
#define CAMERA_H 

class Camera
{
public:
    SDL_Rect view;

    /**
     * @brief Creates camera object at origin (0,0) with screen width and height, or specified values.
     *
     * @param x X position of camera with 0 being the left most pixel.
     * @param y Y position of camera with 0 being the top most pixel.
     * @param width The width of the view.
     * @param height The height of the view.
     */
    Camera::Camera(int x = 0, int y = 0, int width = SCREEN_WIDTH, int height = SCREEN_HEIGHT);

    /**
     * @brief Updates values passed that are not NULL.
     *
     * @param x X position of camera with 0 being the left most pixel.
     * @param y Y position of camera with 0 being the top most pixel.
     * @param w The width of the view
     * @param h The height of the view
     */
    void update(int x = NULL, int y = NULL, int w = NULL, int h = NULL);

    /**
     * @brief Copies the cordnates of an SDL_Rect for the camera's x, y and viewing w, h.
     *
     * @param box The SDL_Rect to clone.
     */
    void set_view(SDL_Rect box);

    /**
     * @brief Calculates the x and y values to place the passed SDL_Rect in the center of the viewport.
     *
     * @param box The SDL_Rect to center on.
     */
    void center_view(SDL_Rect box);
};

#endif