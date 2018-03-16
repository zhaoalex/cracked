#include "provided.h"
#include <string>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

class DecrypterImpl
{
public:
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    void crackHelper(Translator* mapping, Tokenizer* tk, vector<string>* tokens, string ciphertext, vector<string>* possibilities);
    int getMostUntranslated(Translator* tl, const vector<string>* tokens) const;
    bool isFullyTranslated(const string& s) const;

    WordList m_wordList;
    
};

/**
 * Loads the word list using the given filename.
 * @return true if loaded successfully.
 * @runtime O(W), W = num unique words
 */
bool DecrypterImpl::load(string filename)
{
    return m_wordList.loadWordList(filename);
}

/**
 * Cracks a given encrypted message, finding all valid decryptions
 * and outputting them in sorted order to the vector.
 * @runtime who knows.
 */
vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    // Step 1: Start with empty mapping
    Translator tl;
    
    // Step 2: Tokenize cipher and choose unchosen token with most unknown letters
    Tokenizer tk("0123456789 ,;:.!()[]{}-\"#$%^&");
    vector<string> tokens = tk.tokenize(ciphertext);
    
    vector<string> possiblities;
    
    crackHelper(&tl, &tk, &tokens, ciphertext, &possiblities);
    
    sort(possiblities.begin(), possiblities.end());
    
    return possiblities;
}

void DecrypterImpl::crackHelper(Translator* mapping, Tokenizer* tk, vector<string>* tokens, string ciphertext, vector<string>* possibilities) {
    
    // Step 2 cont. TODO put this in main fcn?
    int cipherIndex = getMostUntranslated(mapping, tokens);
    
    string cipherWord = (*tokens)[cipherIndex];

    // Step 3: Translate encrypted word
    string currTranslation = mapping->getTranslation(cipherWord);
    
    // Step 4: Find candidates
    vector<string> candidates = m_wordList.findCandidates(cipherWord, currTranslation);
    
    // Step 5: If empty, return to previous recursive call
    if (candidates.empty()) {
        mapping->popMapping();
        return;
    }
    
    // Step 6
    for (int i = 0; i < candidates.size(); i++) {
        // Step 6a: Create a temp mapping table
        if (!mapping->pushMapping(cipherWord, candidates[i])) continue; // encoding didn't work; throw away and move to next candidate
        
        // Now we have a partial mapping:
        // Step 6b: Use mapping to translate entire cipher to proposed plaintext
        string newTranslation = mapping->getTranslation(ciphertext);
        
        // Step 6c: Check for any fully translated words
        vector<string> words = tk->tokenize(newTranslation);
        bool fullyTranslatedNotFound = true;
        bool strFullyTranslated = true;

        for (int j = 0; j < words.size(); j++) {
            if (isFullyTranslated(words[j])) { // find all fully translated words
                if (!m_wordList.contains(words[j])) { // if fully translated word not part of word list
                    fullyTranslatedNotFound = false;
                    break;
                }
            } else {
                strFullyTranslated = false;
            }
        }
        
        if (!fullyTranslatedNotFound) { // incorrect mapping -> try the next candidate
            mapping->popMapping();
            continue;
        } else if (strFullyTranslated) { // every word translated -> potential solution
            possibilities->push_back(newTranslation); // so record it
            mapping->popMapping();
            continue; // then move to the next candidate
        } else { // message not completely translated -> continue recursion
            crackHelper(mapping, tk, tokens, ciphertext, possibilities);
        }
    }
    
    // Step 7: We tried all words in collection, so throw away current table and return to previous recursion call
    mapping->popMapping();
    return;

}

/**
 * Find the index of the string with the most untranslated characters.
 */
int DecrypterImpl::getMostUntranslated(Translator* tl, const vector<string>* tokens) const {
    int maxIndex = -1;
    int maxCount = 0;
    for (int i = 0; i < tokens->size(); i++) {
        string trans = tl->getTranslation((*tokens)[i]);
        int count = 0;
        for (int j = 0; j < trans.size(); j++) {
            if (trans[j] == '?') count++;
        }
        if (count >= maxCount) {
            maxCount = count;
            maxIndex = i;
        }
    }
    if (maxCount == 0) return -1; // if no question marks anywhere, return -1
    else return maxIndex;
}

/**
 * Returns true if the string is fully translated.
 */
bool DecrypterImpl::isFullyTranslated(const string& s) const {
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '?') return false;
    }
    return true;
}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
   return m_impl->crack(ciphertext);
}
