//
//  Sequence.h
//  Project 2
//
//  Created by Chelsea Lai on 4/19/23.
//

#ifndef Sequence_h
#define Sequence_h

#include <string>
#include <iostream>

using ItemType = std::string;


class Sequence
{
  public:
    Sequence(); // constructor
    
    ~Sequence(); // deconstructor
    
    Sequence (const Sequence& other); // copy constructor
    
    Sequence& operator = (const Sequence& rhs); // Assignment operator
    
    bool empty() const; // return true if empty
    
    int size() const; // return current size
    
    int insert(int pos, const ItemType& value); // insert a value at position pos.
    
    int insert(const ItemType& value); // inserting a value where it is smallest
    
    bool erase(int pos); // erasing a value at position pos
     
    int remove(const ItemType& value); // removing all values of value
    
    bool get(int pos, ItemType& value) const; // get the value at position pos
    
    bool set(int pos, const ItemType& value); // set the value at position pos to value
    
    int find(const ItemType& value) const; // find the value with smallest integer
    
    void swap(Sequence& other); // swap two sequences.
    
//    void dump() const;
    
    
    private:
        int m_size; // the size of the linked list
        struct Node // struct of the node (a doubly linked list with item, next, and prev
        {
            public:
             ItemType val;
             Node* next;
             Node* prev;
        };
        Node* head; // create a head that point to the start of the linked list
        Node* tail; // create a tail that points to th end of the linked list
        void insertBeginning (const ItemType& value); // function to insert node in the beginning of the linked list
        void insertMiddle (int pos, const ItemType& value); // increase a node in the middle of a linked list
        void insertEnd (const ItemType& value); // insert to the end of a linked list
        void emptyList (const ItemType& value); // insert a node into an empty list
};

//Non member functions
int subsequence(const Sequence& seq1, const Sequence& seq2); // function to see if one seq is a subsequence of the other
void concatReverse(const Sequence& seq1, const Sequence& seq2, Sequence& result); //concatenate the revese of the sequences.

#endif /* Sequence_h */
