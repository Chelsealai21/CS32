//
//  Rabbit.h
//  Project 1 - C32
//
//  Created by Chelsea Lai on 4/5/23.
//

#ifndef Rabbit_h
#define Rabbit_h

#include "globals.h"

class Arena; //Arena is only implemented as a pointer type

class Rabbit
{
  public:
      // Constructor
    Rabbit(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_health;
    int    m_idleTurnsRemaining;
};


#endif /* Rabbit_h */
