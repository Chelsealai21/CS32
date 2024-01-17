//
//  Sequence.cpp
//  Project 2
//
//  Created by Chelsea Lai on 4/19/23.
//

#include "Sequence.h"
#include <iostream>

void Sequence::insertBeginning (const ItemType& value) // inserts a node in the beginning of a list
{
    Node* p = head; // create a temp pointer that points to the same object head is pointing to (starting with the first item in the list)
    if (p != nullptr) // checks if p is not a nullptr (that is head is no a nullptr)
    {
        Node* newGuy = new Node; //create a new node to be added in
        newGuy -> val = value; // change the val of new node to the value passed in
        newGuy -> next = p; // have the new node's next to point to the first item in the first
        newGuy->prev = nullptr; // set the prev of new node to nullptr (as it is the first item in the list)
        head = newGuy; // have head point to the new node
        p->prev = newGuy; // set the original first node's prev to the new node
        m_size ++; // increase the size of the linked list by one
    }
    
}

void Sequence::insertMiddle (int pos, const ItemType& value) // insert a node in the middle of the list
{
    Node* p = head;  // create a temp pointer that points to the same object head is pointing to (starting with the first item in the list)
    int x = 0;
    while (p->next !=nullptr && x < pos) // while p's next is not null and x is smaller than pos,
    {
        p = p->next;
        x++;
    }
        
    Node* newGuy = new Node; //create a new node to be added in
    newGuy -> val = value; // change the val of new node to the value passed in
    newGuy -> next = p;  // have the new node's next to point to the node temp pointer's next is pointing to
    newGuy -> prev = p -> prev; // have the new node's prev point to the node temp pointer's prev is pointing to
    p->prev->next = newGuy; // have the node that temp pointer's prev's next to point to the new node
    p->prev = newGuy; // have the node the temp pointer's prev is pointing to to point to new node.
    m_size++; // increase the size of the linked list by one
}

void Sequence::insertEnd (const ItemType& value) // insert a node at the end of the list.
{
    Node* p = tail; // create a temp pointer that points to the same object tail is pointing to (starting with the last item in the list)
    if (p != nullptr) // checks if p is not a nullptr (that is tail is not a nullptr)
    {
        Node* newGuy = new Node; //create a new node to be added in
        newGuy->val = value; // change the val of new node to the value passed in
        newGuy->next = nullptr; // set the next of new node to nullptr (as it is the last item in the list)
        newGuy->prev = p; // set the prev of new node to the node the temp pointer is pointing to (as it is the last item in the list)
        p -> next = newGuy; // have the node the temp pointer's next is pointing to to point to new node.
        tail = newGuy; // have tail point to the new node
        m_size++; // increase the size of the linked list by one
    }
    
}

void Sequence::emptyList (const ItemType& value) // insert a node into an empty list
{
    Node* newGuy = new Node; // create a new node to be added in
    newGuy->val = value; // change the val of new node to the value passed in
    newGuy->next = nullptr; // set the new node's next and prev to nullptr
    newGuy->prev = nullptr;
    head = newGuy; // have the head point to the new node
    tail = newGuy; // have the tail point to the new node
    m_size++; // increase the size of the linked list by one
}

Sequence::Sequence() // default constructor
{
    head = nullptr; // set the linked list to size to zero (that is the head and tail are nullptr)
    tail = nullptr;
    m_size = 0;
}

Sequence:: ~Sequence()
{
    // if size is zero, there is nothing to be deleted
    if (m_size == 0)
        return;
    
    // if size is one, delete the only item head (and tail) is pointing to
    else if (m_size == 1)
        delete head;
    
    // if the size is larger than zero, delete every item one by one
    else
    {
        int iamHere = 0; // set a counter to the number of linked list
        while (iamHere < size()) // while iamHere is less than the less than the size of linked list
        {
            Node* p = head; // set a temp pointer to point to head
            head = p->next; // have head point to the temp pointer's next
            delete p; // delete the object temp pointer is pointing to.
            iamHere++; // increase counter by one
        }
    }
}

Sequence::Sequence (const Sequence& other) //copy constructor
{
    if (other.size() == 0) // is other is empty, set sequence also to zero
    {
        head = nullptr;
        tail = nullptr;
        m_size = 0;
        return;
    }
    
    m_size = 1; // set the size of the new sequence to be 1 (before adding new node)
    //create a pointer to point to the head's object of the other sequence
    Node* p = other.head;
    // copy the first element of the other sequence to sequence
    Node* newGuy = new Node;
    head = newGuy;
    tail = newGuy;
    newGuy -> next = nullptr;
    newGuy -> prev = nullptr;
    newGuy -> val = p->val;
    
    int x = 1; // copy the rest of the element in the other's linked list to the new sequence by adding it to the end of the list
    while (x < other.size())
    {
        insertEnd(p->next->val);
        p = p->next;
        x++;
    }
    
}

Sequence&Sequence::operator = (const Sequence& rhs) // assignment operater
{
    // deleting the current linkerlist
    // create a temp pointer that points to the node
    if (this!= &rhs)  // checking if the current sequence is not already the same sequence as other
    {
        Sequence temp(rhs); // copyswap method  - create a temp sequence that copies the rhs
        swap(temp); // swap the temp with the current sequence
        //
    }
    return *this; // return the new sequence
}

bool Sequence::empty() const
{
    if (m_size == 0) // return true if the size is 0
        return true;
    return false;
}

int Sequence::Sequence:: size() const
{
    return m_size; // return the current size of linked list
}

int Sequence::insert(int pos, const ItemType& value) // insert value into pos
{
    
    if (pos < 0 || pos >= m_size + 1) // if pos is out of bound, return -1
        return -1;
    
    
    if (m_size == 0)
    {
        emptyList(value);// if the size of the list is 0, insert a new node into an empty list
        return pos;
    }
    
    if (pos == size())
    {
        insertEnd(value);  // is the pos is the size of the list, inset a new node to the end of the list
        return pos;
    }
    
    if (pos == 0)
    {
        insertBeginning (value); // if the pos is 0, insert new node in the beginning of the list
        return pos;
    }
    
    if (pos > -1 )
    {
        insertMiddle (pos, value); // if pos is between the end and beginning of list, insert new node in the middle of the list.
        return pos;
    }
    
    //otherwise, return -1.
    return -1;
}

int Sequence::insert(const ItemType& value)
{
    int position = size(); // position is where the value will be inserted. If it is larger than every value in the list, insert it to the end.
    
    if (m_size == 0)// if the size of the list is 0, insert a new node into an empty list
    {
        emptyList(value);
        return position;
    }
    
    int iamHere = size()-1; // counter to keep check of where we are in the linked list
    Node* p = tail; // create temp pointer
    while (p!=nullptr) // starting from the tail
    {
        if (p->val > value)
            position = iamHere; //  if the value in the linked list is smaller than value, set position to that index
        iamHere--;
        
        p = p->prev;
    }
    
    if (position == size()) // if position is the same size as the list, insert value to the end
    {
        insertEnd(value);
        return m_size-1;
    }
    
    if (position == 0)  // if position is zero, insert value to the beginning
    {
        insertBeginning (value);
        return position;
    }
    
    if (position > -1) // if position is in the middle, insert value to the middle at position
    {
        insertMiddle (position, value);
        return position;
    }
    
    return -1;
    
}

bool Sequence::erase(int pos)
{
   
    if (pos < 0 || pos >= size()) // if pos is negative, return false
        return false;
    
    // if there is only one item in the list, set the linked list to empty
    if (size() == 1)
    {
        delete head;
        head = nullptr;
        tail = nullptr;
        m_size --;
        return true;
    }
    
    // if we want to erase the first item in a list
    if (pos ==0)
    {
        Node* toBeDeleted = head; // set a new pointer to point to the object of head
        toBeDeleted->next->prev = nullptr; // set the new pointer's object's next object's prev to nullptr
        head = toBeDeleted->next; // have the hed pointer to the second object in list
        delete toBeDeleted; // delete first item of linked list
        m_size--; //decrease size by one
        return true;
    }
    
    // if we want to erase the last item in a list (same logic as erasing the first item)
    if (pos == size()-1)
    {
        Node* toBeDeleted = tail;
        toBeDeleted->prev->next = nullptr;
        tail = toBeDeleted->prev;
        delete toBeDeleted;
        m_size--;
        return true;
    }
    // if pos is in the middle of the list
    // positioning pointer TBD to the position of deletion
    
    Node* toBeDeleted = head;
    int x = 0;
    while (toBeDeleted->next!= nullptr && x<pos)
    {
        toBeDeleted = toBeDeleted ->next;
        x++;
    }
    
    // deleting a middle node
    Node* p = toBeDeleted -> prev;
    p-> next = toBeDeleted->next;
    toBeDeleted->next->prev = p;
    delete toBeDeleted;
    m_size--;
    return true;
    
    
}

int Sequence::remove(const ItemType& value)
{
    int itemsRemoved = 0;// set number of removed items to zer0
    for (int iamHere = 0; iamHere < size(); iamHere ++)
    {
        Node*p = head;
        int x = 0;
        while (x < size())
        {
            if (p->val == value) //if the val of pointer is equal to value, delete it. Counter does not change as items are shifted to the left.
            {
                erase(x);
                itemsRemoved++;
                iamHere --; //decrease position in sequence by 1
                break;
            }
            else
            {
                x++;
                p = p->next; // move pointer to the next is it has not be destroyed
            }
            
        }
    }
    return itemsRemoved;

}

bool Sequence::get(int pos, ItemType& value) const
{
    if (pos < 0 || pos > m_size-1) //if pos is larger than size or negative, return false
        return false;
    // positioning p to the item to get
    Node* p = head;
    int x = 0;
    while (p->next !=nullptr && x < pos)
    {
        p = p->next;
        x++;
    }
    
    value = p->val; // change value to the val of p pointer
    return true;
}

bool Sequence::set(int pos, const ItemType& value)
{
    if (pos< 0 || pos >= size())
        return false;
    
    // positioning p to the item to be set
    Node* p = head;
    int x = 0;
    while (p->next !=nullptr && x < pos)
    {
        p = p->next;
        x++;
    }
    
    p->val = value; // same logic as above but this time set val of p pointer to value.
    return true;
    
}

int Sequence::find(const ItemType& value) const
{
    int position = -1; // position by default is -1 (value is not in the list)
    int iamHere = size()-1; // counter to keep check of where we are in the linked list (starting from the end)
    Node* p = tail;
    while (p != nullptr)
    {
        if (p->val == value)
            position = iamHere; // iterate through the list. If p val is equal to value, set position to counter's position
        iamHere--;
        p=p->prev;
    }
    return position;
    
}

void Sequence::swap(Sequence& other)
{
    Node* h = head;
    Node* t = tail;// create a temp pointer h and t to point to the head and tail of sequence
    
    head=other.head; // set the head and tail to point the object of other's head and tail
    tail=other.tail;
    
    other.head = h; // swapping the head and tails
    other.tail = t;
    
    int tempsize = m_size; // swap the size of sequences
    m_size = other.m_size;
    other.m_size = tempsize;
}


int subsequence(const Sequence& seq1, const Sequence& seq2)

{
    if (seq2.empty())
        return -1;
    
    if (seq1.size() == seq2.size()) // check to see if the size of the two seq are the same
    {
        int match = 0; // counter
        ItemType one;
        seq1.get(0, one);// initailize one to be the first value of seq1
        ItemType two;
        seq2.get(0, two);// initailize two to be the first value of seq2
        for (int x = 0; x < seq1.size(); x++)
        {
            if (seq1.get(x,one) == seq2.get(x,two)) // iterate through the linked list and if the values of seq 1 and seq 2 are equal, increase match by one
                match++;
        }
        if (match == seq1.size()) // if the number of matched is equal to the size of both sequence, there are equivalent and return 0
            return 0;
        return -1; // otherwise, return -1
    }
    
    Sequence big; // create a new sequence big
    Sequence small; // create a new sequence small
    if (seq1.size()>seq2.size())
    {
        big = seq1;
        small = seq2;
    }
    else
    {
        big = seq2;
        small = seq1;
    } // assign big to the longer sequnece and small to the shorter sequence
    
    ItemType First;
    small.get(0, First);// get First to the first value of small sequence
    for (int x= 0; x<big.size(); x++) // create a for loop that iterates through the big sequence
    {
        ItemType point;
        big.get(x, point);
        int match =0; // counter
        // if the value in the big list is equal to first, increase match by one. Then, go through each of the value in the big list after the first match and up to the size of the small list. If by the end, every matches, return x. Otherwise, break out of the loop and find another match. If no match by the end of the big list, return -1.
        if (point == First)
        {
            match++;
            for (int y = 1; y<small.size(); y++)
            {
                ItemType point1;
                ItemType point2;
                small.get(y, point1);
                big.get(x+y, point2);
                if (point1 == point2)
                    match++;
                if (match == small.size())
                    return x;
                if (point1 != point2)
                    break;
            }
        }
    }
    return -1;
}

void concatReverse(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
    int here = 0; // counter
    Sequence temp;
    for (int x = seq1.size()-1 ; x>=0 ; x--)
    {
        ItemType toBeInserted; // create object of type Itemtype
        seq1.get(x, toBeInserted); // assign object to object seq1 at position x (starting from the end to the front)
        temp.insert(here, toBeInserted); // insert the object to temp at position here (startinf from the beginning to the end
        here++; // increase counter by 1
    }
    
    for (int x = seq2.size()-1 ; x>=0 ; x--) // repeat the same for loop as above but for seq 2 and adding on to the temp sequence
    {
        ItemType toBeInserted;
        seq2.get(x, toBeInserted);
        temp.insert(here, toBeInserted);
        here++;
    }
    result = temp;

}

//void Sequence::dump() const
//{
//    if (head == nullptr)
//    {
//        std::cout << " " << std::endl;
//    }
//
//    Node* test = head;
//    while (test != nullptr)
//    {
//        std::cout << test->val << " ";
//        test = test->next;
//    }
//    std::cout << std::endl;
//}
