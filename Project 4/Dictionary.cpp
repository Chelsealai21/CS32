//
//  Dictionary.cpp
//  Dictionary
//
//  Created by Chelsea Lai on 6/1/23.
//

#include "Dictionary.h"
#include <string>
#include <functional>
#include <vector>
#include <list>
#include <cctype>
#include <utility>  // for swap
#include <iostream>
using namespace std;

void removeNonLetters(string& s);
int primes[26] = {283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443}; // array of large prime numbers; one prime for every letter of the alphabet.

class DictionaryImpl
{
  public:
    DictionaryImpl(int maxBuckets)
    {
        if (maxBuckets>1 && maxBuckets%2 == 0)
            m_numOfBuckets= maxBuckets-1;
        else
            m_numOfBuckets = maxBuckets; // if the bucket number is larger than 1  and even, subtract one to allow for a more euqla distribution of buckets. 
        
        HashTable = new vector<vector<pair<unsigned int, list<string>>>> (maxBuckets); // define hash table to be a vector of vector of pairs of unsigned int and list of strings.
    }
    ~DictionaryImpl() {
        delete HashTable;
    }
    void insert(string word);
    void lookup(string letters, void callback(string)) const;
    
    unsigned int hashFunction (string word) const // hash function. Output: bucket number
    {
        unsigned long hash_val = 1; // calculates hash value of word by multiplying prime numbers indexed to the prime array by each character in the word.
        for (int i = 0; i <word.size(); i++){
            hash_val *= primes[word[i] - 'a'];
        }
        unsigned int bucketNum = hash_val % (m_numOfBuckets); // divide hash value of the word by the number of buckets in hash table
        return bucketNum; // return the bucket number
    }
  private:
    vector <vector <pair<unsigned int, list<string>>>>* HashTable; // HashTable is a pointer to the actual hash table data structure
    int m_numOfBuckets;
};

void DictionaryImpl::insert(string word)
{
    removeNonLetters(word); // remove non characters from the word
    unsigned int sum= 1;
    for (int k = 0; k < word.size(); k++)
    {
        sum *= word[k]; // find the unique integer of the word by multiplying every character of the word together.
    }
            
    if (!word.empty())
    {
        vector<pair<unsigned int, list<string>>>::iterator vi;
        for (vi = (*HashTable)[hashFunction(word)].begin(); vi != (*HashTable)[hashFunction(word)].end(); vi++) // In the right bucket number, find the vector with the same unique integer. If not found, make a new vector and push in the new list of anagrams for the word.
        {
            if (vi->first == sum)
            {
                vi->second.push_back(word);
                return;
            }
        }
        list <string> li;
        li.push_back(word);
        (*HashTable)[hashFunction(word)].push_back(make_pair(sum, li));
    }
    
}

void DictionaryImpl::lookup(string letters, void callback(string)) const
{
    if (callback == nullptr) // if callback function is invalid, return
        return;

    removeNonLetters(letters); // remove non characters in the word
    if (letters.empty()) // if word is empty, return
        return;
    
    int position = hashFunction(letters);
    unsigned int sum= 1;
    for ( int k = 0; k < letters.size(); k++) // calculate for word's unique value
    {
        sum *= letters[k];
    }
    
    vector <pair<unsigned int, list<string>>>::iterator vi;
    for (vi = (*HashTable)[position].begin();
         vi != (*HashTable)[position].end(); vi++) // find the right bucket and vector containing all its anagrams within the bucket. For every anagram, call it into the callback function.
    {
        if (vi->first == sum)
        {
            for (list <string>::iterator li = vi->second.begin(); li != vi->second.end(); li++)
                callback(*li);
        }
    }
    
}

void removeNonLetters(string& s) // remove non letter words
{
    string::iterator to = s.begin();
    for (string::const_iterator from = s.begin(); from != s.end(); from++)
    {
        if (isalpha(*from))
        {
            *to = tolower(*from);
            to++;
        }
    }
    s.erase(to, s.end());  // chop everything off from "to" to end.
}

//******************** Dictionary functions ******************************

// These functions simply delegate to DictionaryImpl's functions
// You probably don't want to change any of this code

Dictionary::Dictionary(int maxBuckets)
{
    m_impl = new DictionaryImpl(maxBuckets);
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const
{
    m_impl->lookup(letters,callback);
}
