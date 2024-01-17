//
//  Player.cpp
//  Project 3
//
//  Created by Chelsea Lai on 5/16/23.
//

#include "Player.h"
#include <iostream>

//========================================================================
// Timer t;                 // create a timer
// t.start();               // start the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#include <chrono>
class Timer
{
  public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double, std::milli> diff =
                          std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
  private:
    std::chrono::high_resolution_clock::time_point m_time;
};

class JumpyTimer
{
  public:
    JumpyTimer(int jumpInterval)
     : m_jumpInterval(jumpInterval), m_callsMade(0)
    {
        actualElapsed();
    }
    double elapsed()
    {
        m_callsMade++;
        if (m_callsMade == m_jumpInterval)
        {
            m_lastElapsed = m_timer.elapsed();
            m_callsMade = 0;
        }
        return m_lastElapsed;
    }
    double actualElapsed()
    {
        m_lastElapsed = m_timer.elapsed();
        return m_lastElapsed;
    }
  private:
    Timer m_timer;
    int m_jumpInterval;
    int m_callsMade;
    int m_lastElapsed;
};

//unsigned long f_helper(int n, double timeLimit, JumpyTimer& timer)
//{
//    unsigned long callCount = 1;
//    if (n == 0)
//        callCount += 0;  /// do a base case computation
//    else if (timeLimit <= 0)
//        callCount += 0;  //  do non-recursive heuristic computation
//    else
//    {
//        for (int branchesLeftToExplore = 10; branchesLeftToExplore > 0; branchesLeftToExplore--)
//             {
//                   // Allocate remaining time limit equally among remaining braches to explore
//                 double thisBranchTimeLimit = timeLimit / branchesLeftToExplore;
//
//                   // Save branch start time so can compute the time the branch took
//                 double startTime = timer.elapsed();
//                   // Explore this branch
//                 callCount += f_helper(n-1, thisBranchTimeLimit, timer);
//                   // Adjust remaining time
//                 timeLimit -= (timer.elapsed() - startTime);
//                 if (timeLimit <= 0){
//                     timeLimit = 0;
//                     break;
//                 }
//             }
//    }
//    return callCount;
//}
//========================================================================

//Create a Player with the indicated name.
Player::Player (std::string name): m_name (name)
{}

//Return the name of the player.
std::string Player::name() const
{
    return m_name;
}

//Return false if the player is a computer player. Return true if the player is human. Most kinds of players will be computer players.
bool Player::isInteractive() const
{
    return false; // since most are computer players, default will be false
}

Player::~Player() {}


HumanPlayer::HumanPlayer (std::string name): Player(name)
{}

// human player is interactive
bool HumanPlayer::isInteractive() const
{
    return true;
}



int HumanPlayer::chooseMove(const Board& b, Side s) const
{
    // human player must choose a hole larger than 0 but smaller than the number of holes. Hole must also have beans.
    int hole=0;
    while (hole <=0 ||  hole > b.holes() || b.beans(s,hole)== 0)
    {
        std::cout << "Select a hole " << name() << ":";
        std::cin >> hole;
        std::cout << std::endl;
    }
    return hole;
}

BadPlayer::BadPlayer (std::string name): Player(name)
{}

int BadPlayer::chooseMove(const Board& b, Side s) const
{
    // bad player always chooses the first hole with beans. Otherwise, return -1
    for (int k = 1; k <= b.holes(); k++)
    {
        if (b.beans(s, k) != 0)
            return k;
    }
    return -1;
    
}
SmartPlayer::SmartPlayer (std::string name): Player(name)
{}

int SmartPlayer::eval(const Board& b, Side s, Side SmartSide, int hole) const
{
    // provides a evalValue for every possible position on the board.
    Board tempBoard (b);
    int evalValue;
    
        
    // if the turns leads to another turn
    if (hole == 0 && b.beansInPlay(s) != 0)
    {
        int x = 1;
        while (x<=tempBoard.holes() && hole == 0)
        {
            int eh;
            Side es;
            if (tempBoard.sow(s, x, es, eh) != false)
            {
                if (es == s && tempBoard.beans(es, eh) == 1 && tempBoard.beans(opponent(es), eh) != 0)
                {
                    tempBoard.moveToPot(s, eh, s);
                    tempBoard.moveToPot(opponent(s), eh, s);
                }
            }
            x++;
        }
    }

    // if the hole leads to a capture, move beans to pot
    if (tempBoard.beans(s, hole) == 1 && tempBoard.beans(opponent(s), hole) != 0 && s == SmartSide)
    {
        tempBoard.moveToPot(s, hole, s);
        tempBoard.moveToPot(opponent(s), hole, s);
    }

    
    // if the resulting moves lead to having more than half of the beans in play, set evalValue to either + infinity or - infinity
    if (b.beans(SmartSide, POT) >= (b.totalBeans()/2))
        evalValue = 1000000;
    if (b.beans(opponent(SmartSide), POT) > (b.totalBeans()/2))
        evalValue = -1000000;


    // if game ends. Set evalValue to either + inifinity or - inifinity. If it is a tie, evalValue is 0.
    Board CopytempBoard(b);
    if (CopytempBoard.beansInPlay(s) == 0)
    {
        for (int x = 1; x <= CopytempBoard.holes(); x++)
        {
            CopytempBoard.moveToPot(opponent(s), x, opponent(s));
        }
        if (CopytempBoard.beans(SmartSide, 0) > CopytempBoard.beans(opponent(s), 0))
            evalValue= 1000000;
        else if (CopytempBoard.beans(SmartSide, 0) < CopytempBoard.beans(opponent(SmartSide), 0))
            evalValue= -1000000;
        else if (CopytempBoard.beans(s, 0) == CopytempBoard.beans(opponent(s), 0))
            evalValue = 0; // results in a tie
    }
    
    // regular case, the eval is just the difference in pot numbers
    evalValue = tempBoard.beans(s, POT) - tempBoard.beans(opponent(s), POT);

    
    return evalValue;
}

void SmartPlayer:: minimax(const Board& b, Side s, int& besthole, int& value, int depth, const Side SmartSide, double timeLimit, JumpyTimer& timer) const
{
    Board tempb(b);
    
    // base case: The game has ended, set value to + or - infinity and besthole is -1
    if (b.beansInPlay(s) == 0 || b.beansInPlay(opponent(s))== 0)
    {
        for (int x = 1; x <= tempb.holes(); x++)
        {
            tempb.moveToPot(opponent(s), x, opponent(s));
            tempb.moveToPot(s, x, s);
        }

        if (tempb.beans(SmartSide, 0) > tempb.beans(opponent(SmartSide), 0))
            value = 1000000;
        else if (tempb.beans(SmartSide, 0) < tempb.beans(opponent(SmartSide), 0))
            value = -1000000;
        else if (tempb.beans(s, 0) == tempb.beans(opponent(s), 0))
            value = 0; // results in a tie
        besthole = -1;
        return;
    }

//     base Case: time elapsed is greater than 5 seconds
    if(timer.actualElapsed() > timeLimit)
    {
        //cerr << "time limit reached" << endl;
        std::vector<int> besteval;
        int eh;
        Side es;
        for (int k  = 1; k <= b.holes(); k++)
        {
            Board COPYb (b);
            if (COPYb.sow(s, k, es, eh) == false)
                continue;
            besteval.push_back(eval(COPYb, es, SmartSide, eh));
        }
    
        int x = 0;
        for (int k  = 0; k < besteval.size()-1; k++)
        {
            if (besteval[k] < besteval[k+1])
                x = k+1;
        }
        value = besteval[x];
        return;
    }
    
    // base case: depth is greater than 5, call the eval function for each possible position on the board

    if (depth >= 5)
    {
        std::vector<int> besteval;
        int eh;
        Side es;
        for (int k  = 1; k <= b.holes(); k++)
        {
            Board COPYb (b);
            if (COPYb.sow(s, k, es, eh) == false)
                continue;
            besteval.push_back(eval(COPYb, es, SmartSide, eh));
        }
    
        int x = 0;
        for (int k  = 0; k < besteval.size()-1; k++)
        {
            if (besteval[k] < besteval[k+1])
                x = k+1;
        }
        value = besteval[x];
        return;
    }

    //sow each possible position on the board
    Side es;
    for (int y = 1; y<=b.holes(); y++)
    {
        int eh;
        Board tempBoard(b);
        if (tempBoard.sow(s, y, es, eh) == false)
            continue;
        
        // if the turns leads to another turn
        if (eh == 0 && es == s && tempBoard.beansInPlay(s) != 0)
        {
            int x = 1;
            while (x<=tempBoard.holes() && eh == 0)
            {
                if (tempBoard.sow(s, x, es, eh) != false)
                {
                    if (es == s && tempBoard.beans(es, eh) == 1 && tempBoard.beans(opponent(es), eh) != 0)
                    {
                        tempBoard.moveToPot(s, eh, s);
                        tempBoard.moveToPot(opponent(s), eh, s);
                    }
                }
                x++;
            }
        }
        
        // if the turn leads to a capture
        if (eh != 0 && es == s && tempBoard.beans(es, eh) == 1 && tempBoard.beans(opponent(es), eh) != 0)
        {
            tempBoard.moveToPot(s, eh, s);
            tempBoard.moveToPot(opponent(s), eh, s);
        }
        
        // if hole leads to player having more than half the beans on the boad, set value to either + or - infinity
        if (tempBoard.beans(SmartSide, POT) >= (tempBoard.totalBeans()/2))
        {
            besthole = y;
            value = 1000000;
            return;
        }
        if (tempBoard.beans(opponent(SmartSide), POT) > (tempBoard.totalBeans()/2))
        {
            value = -1000000;
            return;
        }

       
            
        // opponent's turn
        int opponentbesthole, opponentvalue=0;
        minimax(tempBoard, opponent(s), opponentbesthole, opponentvalue, depth + 1, SmartSide, timeLimit, timer);
        
        // if we are not on SmartSide and our opponentvalue is smaller than value, set value to opponent's value and besthole to hole if it is not empty
        if (s != SmartSide && value >= opponentvalue && b.beans(SmartSide, y) > 0)
        {
            value = opponentvalue;
            besthole = y;
        }

        // if we are on SmartSide and our opponentvalue is larger than value, set value to opponent's value and besthole to hole if it is not empty
        if (s == SmartSide && value <= opponentvalue && b.beans(SmartSide, y) > 0)
        {
            value = opponentvalue;
            besthole = y;
        }
        
    }
    return;
}


int SmartPlayer::chooseMove(const Board& b, Side s) const
{
//    // if there is only one hole with bean, choose that hole
    int numZero = 0;
    for (int k = 1; k<=b.holes(); k++)
    {
        if (b.beans(s, k) !=0)
            numZero++;
    }

    if (numZero == 1)
    {
        int bestHole = 1;
        for (int k = 1; k<=b.holes(); k++)
        {
            if (b.beans(s, k) !=0)
                bestHole = k;
        }
        return bestHole;
    }
    
    if(b.beansInPlay(SOUTH) == 0 || b.beansInPlay(NORTH) == 0)
        return -1; //if beans in both sides' holes are empty, return -1
    
    int value=0;
    int bestHole;
    int depth = 0;
    
    double timeLimit = 4990; // 4.99 seconds; allow 10 ms for wrapping up
    JumpyTimer t(1000);                 // create and start a timer
    
    // call minimax function to evaluate best hole
    minimax(b, s, bestHole, value, depth, s, timeLimit, t);

//    std::cout << t.actualElapsed() << " ms" << std::endl;
//    unsigned long result = f_helper(depth, timeLimit, t);
//    std::cerr << result / 1000000.0  << " million calls" << std::endl;

    if (bestHole<= 0 || bestHole>b.holes())
    {
        for (int k = 1; k <= b.holes(); k++)
        {
            if (b.beans(s, k) != 0)
                bestHole=k;
        }
    }
    
    return bestHole;
}
