#include "provided.h"
#include "MyHash.h"
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
    void addToMaps(const string& input);
    string calcPattern(string s) const;
    string toLowerCase(string s) const;
    
    MyHash<string, string> m_wordToPattern;
    MyHash<string, vector<string>> m_patternToWord;
};

/**
 * Loads the word list from the file.
 * @runtime O(W), W = number of words. (looping through each character in each word is constant time)
 */
bool WordListImpl::loadWordList(string filename)
{
    m_wordToPattern.reset();
    m_patternToWord.reset();
    
    //ifstream infile("/Users/alexzhao/Documents/wordlist.txt");
    ifstream infile(filename);
    if (!infile) return false; // failed to open file
    string s;
    while (getline(infile, s)) { // O(W)
        bool isValid = true;
        for (int i = 0; i < s.size(); i++) { // we want to convert the word to lowercase and check if it's valid
            s[i] = tolower(s[i]); // convert to lowercase
            
            if (s[i] != '\'' && !isalpha(s[i])) { // move on to the next line if word contains non alpha/non apostrophe
                isValid = false;
            }
        }
        if (isValid) {
            addToMaps(s); // O(1)
        }
    }
    
    return true; // successfully opened file
}

/**
 * Return true if the specified word is in the word list.
 * @runtime O(1)
 */
bool WordListImpl::contains(string word) const
{
    // convert word to lowercase
    string lower = toLowerCase(word);
    
    return m_wordToPattern.find(lower) != nullptr; // O(1) to find
}

/**
 * Returns a vector of words with same letter pattern as cipherWord + consistent with currTranslation
 * @runtime O(Q), Q = number of words that match cipher pattern
 */
vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    if (cipherWord.size() != currTranslation.size()) return vector<string>(); // if sizes don't match, return empty vector
    
    // change both to lowercase
    string lowerCipher = toLowerCase(cipherWord);
    string lowerTrans = toLowerCase(currTranslation);
    
    string cipherPattern = calcPattern(lowerCipher); // O(1)
    const vector<string>* v = m_patternToWord.find(cipherPattern); // O(1)
    
    if (v == nullptr) return vector<string>(); // if pattern not found, return empty vector
    
    vector<string> candidates;
    
    // iterate through the vector
    for (int i = 0; i < v->size(); i++) { // O(Q)
        bool isValidWord = true;
        for (int j = 0; j < lowerTrans.size(); j++) { // O(1)
            if (isalpha(lowerTrans[j])) { // if letter
                if (!isalpha(lowerCipher[j])) return vector<string>(); // if cipher isn't also letter, return empty vector
                if (lowerTrans[j] != (*v)[i][j]) { // if letter doesn't match current word in list, move on to next word
                    isValidWord = false;
                    break;
                }
            } else if (lowerTrans[j] == '?') { // if question mark
                if (!isalpha(lowerCipher[j])) return vector<string>(); // if cipher isn't letter, return empty vector
            } else if (lowerTrans[j] == '\'') { // if apostrophe
                if (lowerCipher[j] != '\'') return vector<string>(); // if not also apostrophe, return empty vector
                if ((*v)[i][j] != '\'') { // if current word doesn't also have apostrophe, move on to next word
                    isValidWord = false;
                    break;
                }
            }
        }
        if (isValidWord) {
            candidates.push_back((*v)[i]);
        }
    }
    
    return candidates; // If no words matched, this will also return empty vector
}

// Private member functions

/**
 * Add the given string to both word list maps.
 * @runtime O(1)
 */
void WordListImpl::addToMaps(const string& s) {
    string pattern = calcPattern(s);
    
    // add assocation to word-to-pattern map
    m_wordToPattern.associate(s, pattern);
    
    // add assocation to pattern-to-word map
    vector<string>* v = m_patternToWord.find(pattern); // check if a vector already exists for that pattern
    if (v == nullptr) { // if vector not found
        vector<string> newVec;
        newVec.push_back(s);
        m_patternToWord.associate(pattern, newVec); // O(1)
    } else {
        v->push_back(s); // O(1)
    }

}

// Private member functions

/**
 * For a given string (e.g. Google), return the letter pattern of that string (e.g. ABBACD).
 * @runtime O(1) (technically O(C^2), C = number of characters)
 */
string WordListImpl::calcPattern(string s) const {
    char patternCounter = 'A'; // will keep track of which letters to associate
    for (int i = 0; i < s.size(); i++) { // essentially O(1)
        char currentChar = s[i];
        
        // ignore apostrophes and uppercase letters
        // (we guarantee that the input will be lowercase)
        if (currentChar == '\'' || isupper(currentChar)) continue;
        
        if (currentChar == '\'') continue; // ignore apostrophes
        for (int j = 0; j < s.size(); j++) { // essentially O(1)
            if (s[j] == currentChar) {
                s[j] = patternCounter;
            }
        }
        patternCounter++;
    }
    return s;
}

/**
 * Converts a string to lowercase.
 * @runtime O(1)
 */
string WordListImpl::toLowerCase(string s) const {
    for (int i = 0; i < s.size(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
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
