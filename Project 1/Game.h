//
//  Game.h
//  Project 1 - C32
//
//  Created by Chelsea Lai on 4/5/23.
//

#ifndef Game_h
#define Game_h

#include <iostream>
#include "globals.h"

class Arena;

bool decodeDirection(char ch, int& dir);
int computeDanger(const Arena& a, int r, int c);
bool recommendMove (const Arena& a, int r, int c);


class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nRabbits);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;
    bool is_h; // a bool that tells us if h was selected

      // Helper functions
    std::string takePlayerTurn();
};

#endif /* Game_h */
