//
//  Game.cpp
//  Project 1 - C32
//
//  Created by Chelsea Lai on 4/5/23.
//

#include "globals.h"
#include "Arena.h"
#include "Player.h"
//#include "History.h"
#include "Game.h"
#include <iostream>
//#include <cstdlib>

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////
///
///
///
///////////////////////////////////////////////////////////////////////////
//  Compute Danger
///////////////////////////////////////////////////////////////////////////
int computeDanger(const Arena& a, int r, int c)
{
      // Our measure of danger will be the number of rabbits that might move
      // to position (r,c).  If a rabbit is at that position, it is fatal,
      // so a large value is returned.

    if (a.numberOfRabbitsAt(r,c) > 0)
        return MAXRABBITS+1;

    int danger = 0;
    if (r > 1)
        danger += a.numberOfRabbitsAt(r-1,c);
    if (r < a.rows())
        danger += a.numberOfRabbitsAt(r+1,c);
    if (c > 1)
        danger += a.numberOfRabbitsAt(r,c-1);
    if (c < a.cols())
        danger += a.numberOfRabbitsAt(r,c+1);

    return danger;
}

bool decodeDirection(char ch, int& dir)
{
    switch (tolower(ch))
    {
      default:  return false;
      case 'n': dir = NORTH; break;
      case 'e': dir = EAST;  break;
      case 's': dir = SOUTH; break;
      case 'w': dir = WEST;  break;
    }
    return true;
}

// Recommend a move for a player at (r,c):  A false return means the
 // recommendation is that the player should drop a poisoned carrot and
 // not move; otherwise, this function sets bestDir to the recommended
 // direction to move and returns true.

bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
     // How dangerous is it to stand?
   int standDanger = computeDanger(a, r, c);

     // if it's not safe, see if moving is safer
   if (standDanger > 0)
   {
       int bestMoveDanger = standDanger;
       int bestMoveDir = NORTH;  // arbitrary initialization

         // check the four directions to see if any move is
         // better than standing, and if so, record the best
       for (int dir = 0; dir < NUMDIRS; dir++)
       {
           int rnew = r;
           int cnew = c;
           if (attemptMove(a, dir, rnew, cnew))
           {
               int danger = computeDanger(a, rnew, cnew);
               if (danger < bestMoveDanger)
               {
                   bestMoveDanger = danger;
                   bestMoveDir = dir;
               }
           }
       }

         // if moving is better than standing, recommend move
       if (bestMoveDanger < standDanger)
       {
           bestDir = bestMoveDir;
           return true;
       }
   }
   return false;  // recommend standing
}



Game::Game(int rows, int cols, int nRabbits)
{
    if (nRabbits < 0)
    {
        std::cout << "***** Cannot create Game with negative number of rabbits!" << std::endl;
        std::exit(1);
    }
    if (nRabbits > MAXRABBITS)
    {
        std::cout << "***** Trying to create Game with " << nRabbits
             << " rabbits; only " << MAXRABBITS << " are allowed!" << std::endl;
        std::exit(1);
    }
    int nEmpty = rows * cols - nRabbits - 1;  // 1 for Player
    if (nEmpty < 0)
    {
        std::cout << "***** Game created with a " << rows << " by "
             << cols << " arena, which is too small to hold a player and "
             << nRabbits << " rabbits!" << std::endl;
        std::exit(1);
    }

      // Create the arena
    m_arena = new Arena(rows, cols);

      // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);

      // Populate with rabbits
    while (nRabbits > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (r == rPlayer && c == cPlayer)
            continue;
        m_arena->addRabbit(r, c);
        nRabbits--;
    }
    
    // initialize is_h to false
    is_h = false;
}

Game::~Game()
{
    delete m_arena;
}

std::string Game::takePlayerTurn()
{
    
    for (;;)
    {
        std::cout << "Your move (n/e/s/w/c/h or nothing): ";
        std::string playerMove;
        getline(std::cin, playerMove);
        
        if (playerMove == "h")
        {
            is_h = true; // if player types h, is_h is set to true;
            m_arena -> history().display(); //***
            std::cout<<"Press enter to continue.";
            std::cin.ignore(10000,'\n');
            return ""; // return empty string
        }

        Player* player = m_arena->player();
        int dir;

        if (playerMove.size() == 0)
        {
            is_h = false; // if h was not typed, it is false
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->dropPoisonedCarrot();
        }
        else if (playerMove.size() == 1 && playerMove != "h") // only when h is not called can a turn be counted
        {
            is_h = false;
            if (tolower(playerMove[0]) == 'c')
                return player->dropPoisonedCarrot();
            else if (decodeDirection(playerMove[0], dir))
                return player->move(dir);
        }
        std::cout << "Player move must be nothing, or 1 character n/e/s/w/c/h." << std::endl;
    }
}

void Game::play()
{
    m_arena->display("");
    Player* player = m_arena->player();
    if (player == nullptr)
        return;
    while ( ! player->isDead()  &&  m_arena->rabbitCount() > 0)
    {
        std::string msg = takePlayerTurn();
        m_arena->display(msg);
        if (player->isDead())
            break;
        if (is_h == false)
            m_arena->moveRabbits();
        m_arena->display(msg);
    }
    if (player->isDead())
        std::cout << "You lose." << std::endl;
    else
        std::cout << "You win." << std::endl;
}

