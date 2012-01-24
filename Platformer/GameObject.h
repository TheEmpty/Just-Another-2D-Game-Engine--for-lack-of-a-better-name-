/**
 * @file GameObject.h
 * @brief The class used to create polymorphic game objects.
 *
 * @author Mohammad El-Abid
 */

#ifndef JUST_ANOTHER_2D_GAME_ENGINE_GAMEOBJECT_H
#define JUST_ANOTHER_2D_GAME_ENGINE_GAMEOBJECT_H

class GameObject
{
public:
    virtual bool  collides( GameObject& object ) = 0;
    virtual void  move()  = 0;
    virtual void  unmove()  = 0;
    virtual void  set_x(int x) = 0;
    virtual void  set_y(int y) = 0;
    virtual char* c_str() = 0;
};

#endif // JUST_ANOTHER_2D_GAME_ENGINE_GAMEOBJECT_H