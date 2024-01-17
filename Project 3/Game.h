//
//  Game.h
//  Project 3
//
//  Created by Chelsea Lai on 5/16/23.
//

#ifndef Game_h
#define Game_h


#include "Side.h"
#include "Player.h"
#include "Board.h"

class Game
{
public:
    Game(const Board& b, Player* south, Player* north);
    void display() const;
    void status(bool& over, bool& hasWinner, Side& winner) const;
    bool move(Side s);
    void play();
    int beans(Side s, int hole) const;
    
private:
    Board* m_board;
    Player* m_southPlayer;
    Player* m_northPlayer;
};
#endif /* Game_h */
