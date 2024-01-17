//
//  Board.cpp
//  Project 3
//
//  Created by Chelsea Lai on 5/16/23.
//

#include "Board.h"
#include "Side.h"
#include <vector>
#include <iostream>

//Construct a Board with the indicated number of holes per side (not counting the pot) and initial number of beans per hole. If nHoles is not positive, act as if it were 1; if nInitialBeansPerHole is negative, act as if it were 0.
Board::Board(int nHoles, int nInitialBeansPerHole)
{
    // if hole is less than 0, the number of of hole is 1
    if (nHoles <= 0 )
        nHoles = 1;
    m_numHoles = nHoles;
    // if the number of initial beans is less than 0, set the number of beans per hole to 0
    if (nInitialBeansPerHole < 0)
        nInitialBeansPerHole = 0;
    // pot start with 0 beans
    northHoles.push_back (0);
    southHoles.push_back (0);
    // for each hole, push back a holes with nInitialBeansPerHole number of beans
    for (int x = 0; x< nHoles ; x++)
    {
        northHoles.push_back (nInitialBeansPerHole);
        southHoles.push_back (nInitialBeansPerHole);
    }
}

//Return the number of holes on a side (not counting the pot).
int Board::holes() const
{
    return m_numHoles;
}

//Return the number of beans in the indicated hole or pot, or −1 if the hole number is invalid.
int Board::beans(Side s, int hole) const {
    // return the number of beans in a certain hole
    if (hole < 0 || hole > m_numHoles) {
        return -1; // Invalid hole number
    }

    if (s == NORTH) {
        return northHoles[hole];
    } else if (s == SOUTH) {
        return southHoles[hole];
    }

    return -1; // Invalid side
}

//Return the total number of beans in all the holes on the indicated side, not counting the beans in the pot.
int Board::beansInPlay(Side s) const
{
    // sum up all the beans in play of Side s
    int SUM = 0;
    if (s == NORTH)
    {
        for (int x = 1; x<m_numHoles+1 ;x++)
            SUM+=northHoles[x];
        return SUM;
    }
    if (s == SOUTH)
    {
        int SUM = 0;
        for (int x = 1; x<m_numHoles+1 ; x++)
            SUM+=southHoles[x];
        return SUM;
    }
    return SUM;
}

//Return the total number of beans in the game, including any in the pots.
int Board::totalBeans() const
{
    // sums up all the beans on the board
    int SUM = 0;
    for (int x = 0; x<m_numHoles+1 ;x++)
    {
        SUM+=northHoles[x];
        SUM+=southHoles[x];
    }
    return SUM;
}

//If the hole indicated by (s,hole) is empty or invalid or a pot, this function returns false without changing anything. Otherwise, it will return true after sowing the beans: the beans are removed from hole (s,hole) and sown counterclockwise, including s's pot if encountered, but skipping s's opponent's pot. The function sets the parameters endSide and endHole to the side and hole where the last bean was placed. (This function does not make captures or multiple turns; different Kalah variants have different rules about these issues, so dealing with them should not be the responsibility of the Board class.)

bool Board::sow(Side s, int hole, Side& endSide, int& endHole)
{
    if (!isValidSide(s))
        return false;
    if (!isValidHole(hole))
        return false;
    
    if (s == NORTH && northHoles[hole] == 0)
        return false;
    else if (s == SOUTH && southHoles[hole] == 0)
        return false;
    
    // initialize end side and hole
    endSide = s;
    endHole = hole;
    
    // assign holeside to the side s is on
    std::vector<int>& holeSide = (s == NORTH)? northHoles : southHoles;
    
    // collect beans from the selected hole and empty it
    int numBeans = holeSide[hole];
    holeSide[hole] = 0;
    
    // while the number of beans we have on hand is larger than 0...
    while (numBeans > 0)
    {
        // depending on which side we are on, either increment or decrement our endhole.
        if (endSide == SOUTH)
            endHole ++;
        else if (endSide == NORTH)
            endHole --;
        
        // if we are on north side and our endhole is smaller than 0, this means we have moved to the south side
        if (endSide == NORTH && endHole <= 0)
        {
            // before we move to the south side, if s is north, we increment north pot by 1
            if (s == endSide)
            {
                northHoles[0] ++;
                numBeans --;
            }
            // if our number of beans is 0, the move ends
            if (numBeans == 0)
            {
                endHole = 0;
                continue;
            }
            // otherwise, we move to south and start at position 1 of south side.
            endSide = SOUTH;
            endHole = 1;
        }
        // if we are on south side and our endhole is larger than the number of holes, this means we have moved to the north side
        else if (endSide == SOUTH && endHole > m_numHoles)
        {
            // before we move to the north side, if s is south, we increment south pot by 1
            if (s == endSide)
            {
                southHoles[0] ++;
                numBeans --;
            }
            // if our number of beans is 0, the move ends
            if (numBeans == 0)
            {
                endHole = 0;
                continue;
            }
            // otherwise, we move to north and start at the end position of north side.
            endSide = NORTH;
            endHole = m_numHoles;
        }
        
        // if we end up on a pot on player's side, increment pot by 1
        if (endHole == POT)
        {
            if (s == SOUTH)
                southHoles[POT] ++;
            else if (s == NORTH)
                northHoles[POT] ++;
            numBeans --;
            continue;
        }
        
        //increment the hole player is on by 1
        if (endSide == SOUTH)
            southHoles[endHole] ++;
        
        else if (endSide == NORTH)
            northHoles[endHole] ++;
        
        // decrease number of beans by one
        numBeans --;
        
        
    }
    return true;
        
}

//If the indicated hole is invalid or a pot, return false without changing anything. Otherwise, move all the beans in hole (s,hole) into the pot belonging to potOwner and return true.
bool Board::moveToPot(Side s, int hole, Side potOwner)
{
    if (!isValidSide(s))
        return false;
    if (!isValidSide(potOwner))
        return false;
    if (!isValidHole(hole))
        return false;
    // move beans from hole in s to potowner
    else
    {
        if (potOwner == SOUTH && s == SOUTH)
        {
            southHoles[0] += southHoles[hole];
            southHoles[hole] = 0;
        }
        else if (potOwner == SOUTH && s == NORTH)
        {
            southHoles[0] += northHoles[hole];
            northHoles[hole] = 0;
        }
        else if (potOwner == NORTH && s == NORTH)
        {
            northHoles[0] += northHoles[hole];
            northHoles[hole] = 0;
        }
        else
        {
            northHoles[0] += southHoles[hole];
            southHoles[hole] = 0;
        }
    }
    return true;
}

//If the indicated hole is invalid or beans is negative, this function returns false without changing anything. Otherwise, it will return true after setting the number of beans in the indicated hole or pot to the value of the third parameter. (This could change what beansInPlay and totalBeans return if they are called later.) This function exists solely so that we and you can more easily test your program: None of your code that implements the member functions of any class is allowed to call this function directly or indirectly. (We'll show an example of its use below.)
bool Board::setBeans(Side s, int hole, int beans)
{
    if (hole < 0)
        return false;
    if (hole > m_numHoles)
        return false;
    if (!isValidSide(s))
        return false;
    if (beans < 0)
        return false;
    
    if (s == NORTH)
    {
        northHoles[hole] = beans;
    }
    else
    {
        southHoles[hole] = beans;
    }
    return true;
}

// evaluates if s is NORTH or SOUTH
bool Board::isValidSide (Side s)
{
    if (s == NORTH)
        return true;
    if (s == SOUTH)
        return true;
    return false;
    
}
// evaluates if the hole is valid (i.e. hole is larger than 0 but less than number of holes
bool Board::isValidHole (int hole)
{
    if (hole <= 0)
        return false;
    if (hole > m_numHoles)
        return false;
    return true;
}
