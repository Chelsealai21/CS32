//
//  Game.cpp
//  Project 3
//
//  Created by Chelsea Lai on 5/16/23.
//

#include "Game.h"
#include <iostream>


//Construct a Game to be played with the indicated players on a copy of the board b. The player on the south side always moves first.
Game::Game(const Board& b, Player* south, Player* north) : m_southPlayer (south), m_northPlayer (north)
{
    m_board = new Board(b);
}

//Display the game's board in a manner of your choosing, provided you show the names of the players and a reasonable representation of the state of the board.
void Game::display() const
{
    // Draw the grid
    std::string northName = m_northPlayer -> name();
    std:: string displayNorthName = northName + "'s pot";
    std::string southName = m_southPlayer -> name();
    std:: string displaySouthName = southName + "'s pot";
    
    // cout first line:
    std::cout << " North " <<std::endl;
    // cout second line:
    std::cout << std::string(displayNorthName.size() + 5, ' ');
    for (int x = 0 ; x<m_board->holes(); x++)
    {
        std::cout<<m_board -> beans (NORTH,x+1);
        std::cout << " ";
    }
    std:: cout <<std::endl;
    
    // cout third line:
    std::cout << displayNorthName << "  " << m_board -> beans(NORTH, POT) << std::string(m_board->holes()*2, ' ') << "  " << m_board -> beans(SOUTH, POT) << "  " << displaySouthName <<std::endl;
    
    // cout 4th line:
    std::cout << std::string(displayNorthName.size() + 5, ' ');
    
    for (int x = 0 ; x<m_board->holes(); x++)
    {
        
        std::cout<<m_board -> beans (SOUTH, x+1);
        std::cout << " ";
    }
    std:: cout << std::endl;
    //cout 5th line:
    std::cout << std::string(displayNorthName.size() + 5 + m_board->holes()*2 + 5, ' ') << " South " <<std::endl;
    
}

void Game::status(bool& over, bool& hasWinner, Side& winner) const
{
    // if both sides still have beans, the game is not over
    if (m_board->beansInPlay(SOUTH) != 0 && m_board->beansInPlay(NORTH) != 0)
    {
        over = false;
        return;
    }
    // otherwise, the game is over
    over = true;
    
    // if the number of beans in the pot are the same, it is a tie
    if (m_board->beans(NORTH, 0) == m_board->beans(SOUTH, 0))
        hasWinner = false;
    
    // if the number of in north's pot is larger than south, north wins
    if (m_board->beans(NORTH, 0) > m_board->beans(SOUTH, 0))
    {
        hasWinner = true;
        winner = NORTH;
    }
    // otherwise, south wins
    else if (m_board->beans(NORTH, 0) < m_board->beans(SOUTH, 0))
    {
        hasWinner = true;
        winner = SOUTH;
    }
}
//If the game is over (i.e., the move member function has been called and returned false), set over to true; otherwise, set over to false and do not change anything else. If the game is over, set hasWinner to true if the game has a winner, or false if it resulted in a tie. If hasWinner is set to false, leave winner unchanged; otherwise, set it to the winning side.


bool Game::move(Side s)
{
    Side es;
    int eh;
    
    bool hasWinner, over;
    Side winner;
    status (over, hasWinner, winner);
    
    while (over != true)
    {
        if (s == NORTH)
        {
            // north chooses a move
            int chosenhole = m_northPlayer->chooseMove(*m_board, s);
            std::cout <<m_northPlayer->name() << " choose hole: " << chosenhole << std::endl;
            m_board->sow(s, chosenhole,es, eh);
            
            // ending in our own pot, continue turn
            if (eh == 0 && es == NORTH)
            {
                display();
                status (over, hasWinner, winner);
                if (over!= true)
                {
                    std::cout << m_northPlayer->name() << "'s turns continue"<<std::endl;
                }
                continue;
            }
            
            // capture
            else if (es == NORTH && eh != 0 &&m_board->beans(s, eh) == 1 && m_board->beans(SOUTH, eh) != 0)
            {
                m_board->moveToPot(NORTH, eh, NORTH);
                m_board->moveToPot(SOUTH, eh, NORTH);
                display();
                std::cout << m_northPlayer->name() << " has made a capture!"<< std::endl;
                return true;
            }
            return true;

        }
        
        // south chooses a move
        else if (s == SOUTH)
        {
            int chosenhole = m_southPlayer->chooseMove(*m_board, s);
            std::cout <<m_southPlayer->name() << " choose hole: " << chosenhole << std::endl;
            m_board->sow(s, chosenhole,es, eh);
            {
                // ending in our own pot, continue turn
                if (eh == 0 && es == SOUTH)
                {
                    display();
                    status (over, hasWinner, winner);
                    if (over!= true)
                    {
                        std::cout << m_southPlayer->name() << "'s turns continue"<<std::endl;
                    }
                    continue;
                }
                // capture
                else if (es == SOUTH && eh != 0 && m_board->beans(s, eh) == 1 && m_board->beans(NORTH, eh) != 0)
                {
                    display();
                    m_board->moveToPot(SOUTH, eh, SOUTH);
                    m_board->moveToPot(NORTH, eh, SOUTH);
                    std::cout << m_southPlayer->name() << " has made a capture!"<< std::endl;
                    return true;
                }
                return true;
            }
        }
    }
    // incomplete move, sweeping
    if (m_board->beansInPlay(NORTH) == 0 && m_board->beansInPlay(SOUTH) != 0)
    {
        for (int x = 1; x <= m_board->holes(); x++)
            m_board->moveToPot(SOUTH, x, SOUTH);
        
        std::cout << "Sweeping remaining beans into " << m_southPlayer->name() << "'s pot"<< std::endl;
    }
    
    else if (m_board->beansInPlay(SOUTH) == 0&& m_board->beansInPlay(NORTH) != 0)
    {
        for (int x = 1; x <= m_board->holes(); x++)
            m_board->moveToPot(NORTH, x, NORTH);
        
        std::cout << "Sweeping remaining beans into " << m_northPlayer->name() << "'s pot"<< std::endl;

    }

    return false;
}
    //Attempt to make a complete move for the player playing side s. "Complete" means that the player sows the seeds from a hole and takes any additional turns required or completes a capture. Whenever the player gets an additional turn, you should display the board so someone looking at the screen can follow what's happening. If the move can be completed, return true; if not, because the move is not yet completed but side s has no holes with beans to pick up and sow, sweep any beans in s's opponent's holes into that opponent's pot and return false.
    
    void Game::play()
    {
        bool hasWinner, over;
        Side winner;
        
        status (over, hasWinner, winner);
        // starting side is south
        Side StartingSide = SOUTH;
        while (over!=true)
        {
            
            display();
            // if both players are not interactive, then prompt user to press enter to have the compiler make one move at a time
            if (m_northPlayer->isInteractive() == false && m_southPlayer->isInteractive() == false)
            {
                std:: cout <<"Please press Enter to continue:"<<std::endl;
                std::cin.ignore();
            }
            //Starting Side makes a move
            move(StartingSide);
            // swtich turns
            StartingSide = opponent(StartingSide);
            
            status (over, hasWinner, winner);
        }
        // when game is over, make the last move
        move(StartingSide);
        status (over, hasWinner, winner);
        display();
        // call winner or tie when game is over
        if (over == true && hasWinner == true)
        {
            if (winner == NORTH)
                std::cout<< "The winner is "<< m_northPlayer->name() <<std::endl;
            else if (winner == SOUTH)
                std::cout<< "The winner is "<< m_southPlayer->name() <<std::endl;
        }
        if (over == true && hasWinner == false)
        {
            std::cout << "It' a tie!"<<std::endl;
        }
        
    }
    //Play the game. Display the progress of the game in a manner of your choosing, provided that someone looking at the screen can follow what's happening. If neither player is interactive, then to keep the display from quickly scrolling through the whole game, it would be reasonable periodically to prompt the viewer to press ENTER to continue and not proceed until ENTER is pressed. (The ignore function for input streams is useful here.) Announce the winner at the end of the game. You can apportion to your liking the responsibility for displaying the board between this function and the move function.
    int Game::beans(Side s, int hole) const
    {
        if (hole < 0 || hole > m_board->holes())
        {
            return -1;
        }
        
        if (s == NORTH) {
            return m_board -> beans (NORTH, hole);
        } else if (s == SOUTH) {
            return m_board -> beans (SOUTH, hole);
        }
        return -1; // Invalid side
    }
//Return the number of beans in the indicated hole or pot of the game's board, or −1 if the hole number is invalid. This function exists so that we and you can more easily test your program.
