#include "provided.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cctype>
#include <random>
#include <algorithm>
#include <numeric>
using namespace std;

const string WORDLIST_FILE = "wordlist.txt";

string encrypt(string plaintext)
{
	  // Generate a to z
    char plaintextAlphabet[26+1];
	iota(plaintextAlphabet, plaintextAlphabet+26, 'a');
	plaintextAlphabet[26] = '\0';

	  // Generate permutation
	string ciphertextAlphabet(plaintextAlphabet);
	default_random_engine e((random_device()()));
	shuffle(ciphertextAlphabet.begin(), ciphertextAlphabet.end(), e);

	  // Run translator (opposite to the intended direction)
	Translator t;
	t.pushMapping(plaintextAlphabet, ciphertextAlphabet);
	return t.getTranslation(plaintext);
}

bool decrypt(string ciphertext)
{
	Decrypter d;
	if ( ! d.load(WORDLIST_FILE))
	{
		cout << "Unable to load word list file " << WORDLIST_FILE << endl;
		return false;
	}
	for (const auto& s : d.crack(ciphertext))
		cout << s << endl;
	return true;
}

/*
int main(int argc, char* argv[])
{
	if (argc == 3  &&  argv[1][0] == '-')
	{
		switch (tolower(argv[1][1]))
		{
		  case 'e':
			cout << encrypt(argv[2]) << endl;
			return 0;
		  case 'd':
			if (decrypt(argv[2]))
				return 0;
			return 1;
		}
	}

	cout << "Usage to encrypt:  " << argv[0] << " -e \"Your message here.\"" << endl;
	cout << "Usage to decrypt:  " << argv[0] << " -d \"Uwey tirrboi miyi.\"" << endl;
	return 1;
}
*/

/*
int main() {
    Tokenizer t(" ,.!");
    vector<string> v = t.tokenize("...a");
    for (int i = 0; i < v.size(); i++) {
        cerr << v[i] << endl;
    }
}
*/

/*
#include "MyHash.h"

int main() {
    MyHash<string, int> test;
    test.associate("1", 1);
    test.associate("3", 3); // 2
    
    test.associate("2", 2);
    
    cout << *test.find("1") << endl << *test.find("3") << endl << *test.find("2") << endl;
    
}
*/

/*
string c(string s) {
    char patternCounter = 'A'; // will keep track of which letters to associate
    for (int i = 0; i < s.size(); i++) {
        char currentChar = s[i];
        
        // ignore apostrophes and uppercase letters
        // (we guarantee that the input will be lowercase)
        if (currentChar == '\'' || isupper(currentChar)) continue;
        
        if (currentChar == '\'') continue; // ignore apostrophes
        for (int j = 0; j < s.size(); j++) {
            if (s[j] == currentChar) {
                s[j] = patternCounter;
            }
        }
        patternCounter++;
    }
    return s;
}

int main() {
    string s = "google";
    cout << c(s);
}
 */

void f()
{
    WordList wl;
    if ( ! wl.loadWordList("wordlist.txt"))
    {
        cout << "Unable to load word list" << endl;
        return;
    }
    if (wl.contains("onomatopoeia"))
        cout << "I found onomatopoeia!" << endl;
    else
        cout << "Onomatopoeia is not in the word list!" << endl;
    string cipher = "xyqbbq";
    string decodedSoFar = "?r????";
    vector<string> v = wl.findCandidates(cipher, decodedSoFar);
    if (v.empty())
        cout << "No matches found" << endl;
    else
    {
        cout << "Found these matches:" << endl;
        for (size_t k = 0; k < v.size(); k++)
            cout << v[k] << endl; // writes grotto and troppo
    }
}

int main() {
    f();
}
    
