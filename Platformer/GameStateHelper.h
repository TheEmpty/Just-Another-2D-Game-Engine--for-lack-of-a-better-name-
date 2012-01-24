/**
 * @file GameStateHelper.h
 * @brief Decleration and defenition of a singleton class for game state managment.
 *
 * @author Mohammad El-Abid
 */

#ifndef JUST_ANOTHER_2D_GAME_ENGINE_GAME_STATE_HELPER_H
#define JUST_ANOTHER_2D_GAME_ENGINE_GAME_STATE_HELPER_H

#include "GameState.h"
#include "globals.h"

namespace JA2GE {

    class GameState; // forward decleration

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
        GameStateHelper( const GameStateHelper& );

        /**
         * @brief Prevent assignment/overwrite.
         */
        GameStateHelper& operator=( const GameStateHelper& );

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
        int stateID;
        // Set to the next state ID, waiting to be proccessed by external code.
        int nextState;

        /**
         * @breif Pointer to current state object.
         */
        GameState* currentState;

        /**
         * @brief Change the next state ID, thus queueing the change. But don't overwrite an exit state
         */
        void set_next_state( int newState ) { if( nextState != STATE_EXIT ) nextState = newState; }
    };
}
#endif // JUST_ANOTHER_2D_GAME_ENGINE_GAME_STATE_HELPER_H