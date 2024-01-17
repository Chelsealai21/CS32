//
//  Player.h
//  Project 3
//
//  Created by Chelsea Lai on 5/16/23.
//

#ifndef Player_h
#define Player_h

#include <iostream>
#include "Board.h"


class JumpyTimer;

class Player
{
public:
    Player(std::string name);
    std::string name() const;
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const = 0;
    virtual ~Player();
private:
    std::string m_name;
};

class HumanPlayer: public Player
{
public:
    HumanPlayer (std::string name);
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const;
};

class BadPlayer: public Player
{
public:
    BadPlayer (std::string name);
    virtual int chooseMove(const Board& b, Side s) const;
    
};

class SmartPlayer: public Player
{
public:
    SmartPlayer (std::string name);
    virtual int chooseMove(const Board& b, Side s) const;
private:
    int eval(const Board& b, Side s, Side SmartSide, int hole) const;
    void minimax(const Board& b, Side s, int& besthole, int& value, int depth, const Side SmartSide, double timeLimit, JumpyTimer& timer) const;
    
};

#endif /* Player_h */
