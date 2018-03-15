#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class TranslatorImpl
{
public:
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
    vector<string> m_stack;
    
    // we use indices as A-Z normal-text characters; init to all ?s
    string m_map = "??????????????????????????";
};

/**
 * Push a new mapping onto the translator.
 * @return false if the mapping is invalid in some way.
 * @runtime O(N+L), N = length of strings, L = num letters in alphabet; essentially O(1)
 */
bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    if (ciphertext.size() != plaintext.size()) return false; // if different sizes, return false
    
    // create a temp map
    string tempMap = m_map;
    
    for (int i = 0; i < ciphertext.size(); i++) { // O(N)
        if (!isalpha(ciphertext[i]) || !isalpha(plaintext[i])) return false; // if not letter, return false
        
        // convert both strings to uppercase
        ciphertext[i] = toupper(ciphertext[i]);
        plaintext[i] = toupper(plaintext[i]);
        
        if (tempMap[ciphertext[i] - 'A'] != '?') return false; // if one ciphertext maps to two plaintexts, return false (this is inconsistent)
        
        tempMap[ciphertext[i] - 'A'] = plaintext[i];
    }
    
    // check if two ciphertexts map to the same plaintext (inconsistent)
    bool isUsed[26];
    for (int i = 0; i < 26; i++) { // O(L)
        isUsed[i] = false;
    }
    for (int i = 0; i < 26; i++) { // O(L)
        if (tempMap[i] == '?') continue; // ignore all question marks
        if (isUsed[tempMap[i] - 'A']) return false; // return false if already used
        isUsed[tempMap[i] - 'A'] = true;
    }
    
    // add mapping to stack
    m_stack.push_back(tempMap);
    
    // set m_map to tempMap
    m_map = tempMap;
    
    return true;
}

/**
 * Pop most recent mapping from the stack and revert mapping table.
 * @runtime O(L)
 */
bool TranslatorImpl::popMapping()
{
    if (m_stack.size() == 0) return false; // can't pop more than you've pushed
    
    m_stack.pop_back();
    
    if (m_stack.size() == 0) { // back to base mapping
        for (int i = 0; i < 26; i++) {
            m_map[i] = '?';
        }
    } else { // back to previous mapping
        string oldMapping = m_stack.back();
        
        for (int i = 0; i < 26; i++) {
            m_map[i] = oldMapping[i];
        }
    }
    
    return true;
}

/**
 * Gets the translation of the cipher text in the current mapping table.
 */
string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    string trans = "";
    for (int i = 0; i < ciphertext.size(); i++) {
        if (islower(ciphertext[i])) { // lowercase
            // convert char to uppercase; subtract by A to get the correct index;
            // find the correct translated value in the map; convert back to lowercase
            trans += tolower(m_map[toupper(ciphertext[i]) - 'A']);
        } else if (isupper(ciphertext[i])) { // uppercase
            trans += m_map[ciphertext[i] - 'A'];
        } else { // anything else
            trans += ciphertext[i];
        }
    }
    return trans; // This compiles, but may not be correct
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}
