//
//  Board.h
//  Project 3
//
//  Created by Chelsea Lai on 5/16/23.
//

#ifndef Board_h
#define Board_h

#include "Side.h"
#include <iostream>
#include <vector>

class Board
{
    public:
        // constructor
        Board(int nHoles, int nInitialBeansPerHole);
    
        int holes() const;
        int beans(Side s, int hole) const;
        int beansInPlay(Side s) const;
        int totalBeans() const;
        bool sow(Side s, int hole, Side& endSide, int& endHole);
        bool moveToPot(Side s, int hole, Side potOwner);
        bool setBeans(Side s, int hole, int beans);
    
    private:
        int m_numHoles;
        std::vector<int> southHoles;
        std::vector<int> northHoles;
        bool isValidSide (Side s);
        bool isValidHole (int hole);
    
        
    
};

#endif /* Board_h */
