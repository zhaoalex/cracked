#include "provided.h"
#include "substituteMyHash.h" // TODO replace
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;

class WordListImpl
{
public:
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
    bool addToMap(string input);
    
    MyHash<string, int> wordMap;
};

/**
 * Loads the word list from the file.
 * @runtime O(W), W = number of words. (looping through each character in each word is constant time)
 */
bool WordListImpl::loadWordList(string filename)
{
    wordMap.reset();
    ifstream infile("wordlist.txt");
    if (!infile) return false; // failed to open file
    string s;
    while (getline(infile, s)) {
        for (int i = 0; i < s.size(); i++) {
            if (s[i] != '\'' || !isalpha(s[i])) {
                break; // move on to the next line if word contains non alpha/non apostrophe
            }
            
        }
    }
    
    return false;  // This compiles, but may not be correct
}

bool WordListImpl::contains(string word) const
{
    return false; // This compiles, but may not be correct
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    return vector<string>();  // This compiles, but may not be correct
}

// Private member functions

bool WordListImpl::addToMap(string s) {
    for (int i = 0; i < s.size(); i++) {
        if (s[i] != '\'' || !isalpha(s[i])) {
            return false; // move on to the next line if word contains non alpha/non apostrophe
        }
    }
}


//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
   return m_impl->findCandidates(cipherWord, currTranslation);
}
