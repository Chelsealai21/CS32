//
//  History.h
//  Project 1 - C32
//
//  Created by Chelsea Lai on 4/6/23.
//

#ifndef History_h
#define History_h

#include "globals.h"


class History
{
    public:
        History(int nRows, int nCols);
        bool record(int r, int c);
        void display() const;
        
    private:
        int m_rows;
        int m_cols;
    
        int m_Pgrid[MAXROWS][MAXCOLS];
        char m_hgrid[MAXROWS][MAXCOLS];
        
};

#endif /* History_h */
