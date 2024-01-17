//
//  History.cpp
//  Project 1 - C32
//
//  Created by Chelsea Lai on 4/6/23.
//

#include "History.h"
#include "globals.h"
#include <iostream>

History::History (int nRows, int nCols)
: m_rows (nRows), m_cols(nCols)
{
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            m_hgrid [r-1][c-1] = '.'; // initializes a grid that starts with all dots
    
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            m_Pgrid[r-1][c-1] = 0; // initializes a grid that will record the number of turns a poisoned rabbit has been in that point
    
}

bool History::record(int r, int c)
{
    
    if (r <= 0  ||  c <= 0  ||  r > MAXROWS  ||  c > MAXCOLS)
    {
        return false; // returns false if r and c are out of bound
    }
    
    m_Pgrid[r-1][c-1] ++; // record will be called upon the turn after the rabbit has been poisoned so we increment one (one for first turn poisoned)
    
    if (m_Pgrid[r-1][c-1] >= 26) // if the number of turns since poisoned is larger than equal to 26, the alphabet at that point is Z
    {
        m_hgrid [r-1][c-1] = 'Z';
    }
   
    else if (m_Pgrid[r-1][c-1] == 1) // if the number of turns since poisoned is 1, the alphabet at that point is A
    {
        m_hgrid [r-1][c-1] = 'A';
    }
    
    else if (m_Pgrid[r-1][c-1] >= 2) // if the number of turns since poisoned is larger and equal to 2, the alphabet will increment from A to Z
    {
        m_hgrid [r-1][c-1] ++;
    }

    return true;
}


void History::display() const
{
    clearScreen(); // clears screen
    
    for (int r = 1; r <= m_rows; r++)
    {
        for (int c = 1; c <= m_cols; c++)
        {
            std::cout <<m_hgrid [r-1][c-1]; // display the grid with the history of poisoned rabbits
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
