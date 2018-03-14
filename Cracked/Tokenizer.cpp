#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const string& s) const;
private:
    bool isSeparator(char c) const;
    
    vector<char> m_sepArr;
};

/**
 * Initalizes a new Tokenizer object.
 * @runtime O(P), P = number of separators
 */
TokenizerImpl::TokenizerImpl(string separators)
{
    for (int i = 0; i < separators.size(); i++) { // loop P times
        m_sepArr.push_back(separators[i]); // push_back is O(1) amortized
    }
}

/**
 * Split s into token strings based on the separators provided in the constructor.
 * @return vector of strings.
 * @runtime O(SP), S = number of characters, P = number of separators
 */
vector<string> TokenizerImpl::tokenize(const string& s) const
{
    vector<string> tokens;
    int i = 0;
    while (i < s.size()) { // will only loop through each character in the string once
        int begIndex = i;
        while (!isSeparator(s[i]) && i < s.size()) { // loop until we hit a seperator or the end of the string
            i++;
        }
        int endIndex = i;
        if (begIndex == endIndex) { // i.e. the only character was a separator
            i++;
            continue; // so we ignore it and move to the next character
        }
        tokens.push_back(s.substr(begIndex, endIndex - begIndex));
    }
    return tokens;
}

// Private helper functions

/**
 * Returns true if the character is a separator.
 * @runtime O(P), P = number of separators
 */
bool TokenizerImpl::isSeparator(char c) const {
    for (int i = 0; i < m_sepArr.size(); i++) {
        if (c == m_sepArr[i]) return true;
    }
    return false;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const string& s) const
{
    return m_impl->tokenize(s);
}
