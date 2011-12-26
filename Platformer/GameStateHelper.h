/**
 * @file GameStateHelper.h
 * @brief Decleration and defenition of a singleton class for game state managment.
 *
 * @author Mohammad El-Abid
 */

#include "GameState.h"
#include "globals.h"

#ifndef GAME_STATE_HELPER_H
#define GAME_STATE_HELPER_H

class GameStateHelper
{
private:
    /**
     * @brief Initalize the states to NULL.
     */
    GameStateHelper()
    {
        stateID = STATE_NULL;
        nextState = STATE_NULL;
        currentState = NULL;
    };

    /**
     * @brief Prevent copy-construction.
     */
    GameStateHelper(const GameStateHelper&);

    /**
     * @brief Prevent assignment/overwrite.
     */
    GameStateHelper& operator=(const GameStateHelper&);

public:
    /**
     * @brief Get a pointer singleton instance.
     */
    static GameStateHelper* Instance()
    {
        static GameStateHelper singleton;
        return &singleton;
    }

    // Current state ID.
    int GameStateHelper::stateID;
    // Set to the next state ID, waiting to be proccessed by external code.
    int GameStateHelper::nextState;

    /**
     * @breif Returns pointer to current state object.
     */
    GameState* GameStateHelper::currentState;

    /**
     * @brief Change the next state ID, thus queueing the change. But don't overwrite an exit state
     */
    void set_next_state( int newState ) { if( nextState != STATE_EXIT ) nextState = newState; }
};

#endif // GAME_STATE_HELPER_H