//
//  Player.h
//  Project 1 - C32
//
//  Created by Chelsea Lai on 4/5/23.
//

#ifndef Player_h
#define Player_h

//#include "Arena.h" You don't need to know everything about the class to make a pointer to that class "incompatible type declaration"

#include <iostream>
#include "globals.h"

class Arena;

class Player
{
  public:
      // Constructor
    Player(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    std::string dropPoisonedCarrot();
    std::string move(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
    
};

#endif /* Player_h */
