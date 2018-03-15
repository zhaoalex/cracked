#include "provided.h"
#include <string>
#include <vector>
#include <set>
using namespace std;

class DecrypterImpl
{
public:
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    void DecrypterImpl::crackHelper(Translator* mapping, vector<string> tokens);
    string getMostUntranslated(const vector<string>& tokens) const;

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
    
    set<string> possiblities;
    
    crackHelper(&tl, tokens);
    
    return vector<string>();  // This compiles, but may not be correct
}

void DecrypterImpl::crackHelper(Translator* mapping, Tokenizer* tk, vector<string> tokens, string ciphertext, set<string> possibilities) {
    // Step 2 cont. TODO put this in main fcn?
    string cipherWord = getMostUntranslated(tokens);
    
    // Step 3: Translate encrypted word
    string currTranslation = mapping->getTranslation(cipherWord);
    
    // Step 4: Find candidates
    vector<string> candidates = m_wordList.findCandidates(cipherWord, currTranslation);
    
    // Step 5: If empty, return to previous recursive call
    if (candidates.empty()) {
        mapping->popMapping();
        
        return; // TODO ????????
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
        bool isFullyDecoded = true;
        bool hasFullyTranslatedWord = false;
        for (int j = 0; j < words.size(); j++) {
            if (m_wordList.contains(words[i])) {
                hasFullyTranslatedWord = true;
            } else {
                isFullyDecoded = false;
            }
        }
        if (!hasFullyTranslatedWord) { // no fully translated words = incorrect mapping
            mapping->popMapping();
            continue; // so move to the next candidate
        } else if (isFullyDecoded) { // every word decoded = potential solution
            possibilities.insert(newTranslation); // so record it
            mapping->popMapping();
            continue; // then move to the next candidate
        } else { // message not completely translated
            crackHelper(mapping, tk, tokens, ciphertext, possibilities);
        }
    }
    
    // Step 7: We tried all words in collection, so throw away current table and recurse again
    mapping->popMapping();
    crackHelper(mapping, <#vector<string> tokens#>)

}

/**
 * Find the string with the most untranslated characters.
 */
string DecrypterImpl::getMostUntranslated(const vector<string>& tokens) const {
    int maxIndex = 0;
    int maxCount = 0;
    for (int i = 0; i < tokens.size(); i++) {
        int count = 0;
        for (int j = 0; j < tokens[i].size(); j++) {
            if (tokens[i][j] == '?') count++;
        }
        if (count > maxCount) {
            maxCount = count;
            maxIndex = i;
        }
    }
    return tokens[maxIndex];
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
