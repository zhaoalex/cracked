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

const string WORDLIST_FILE = "/Users/alexzhao/Documents/wordlist.txt";

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


/*
void f()
{
    Translator t; // Define a translator object
    string secret = "Hdqlx!";
    string translated = t.getTranslation(secret);
    cout << "The translated message is: " << translated;
    // writes The translated message is: ?????!
}
 */

/*
void f()
{
    Translator t;
    // Submit the mapping DàE, HàR, LàD
    t.pushMapping("DHL", "ERD");
    string secret = "Hdqlx!";
    cout << t.getTranslation(secret) << endl; // writes Re?d?!
}
 */

/*
void f()
{
    Translator t;
    // Submit the first collection of character mappings
    t.pushMapping("DHL", "ERD"); // DàE, HàR, LàD
    string secret = "Hdqlx!";
    cout << t.getTranslation(secret) << endl; // writes Re?d?!
    // Submit a second collection of character mappings
    t.pushMapping("QX", "AY"); // QàA, XàY
    cout << t.getTranslation(secret) << endl; // writes Ready!
}
 */

/*
void f()
{
    Translator t;
    // Submit the first collection of mappings
    t.pushMapping("DHL", "ERD"); // DàE, HàR, LàD
    string secret = "Hdqlx!";
    cout << t.getTranslation(secret) << endl; // writes Re?d?!
    // Submit a second collection of mappings
    t.pushMapping("QX", "AY"); // QàA, XàY
    cout << t.getTranslation(secret) << endl; // writes Ready!
    // Pop the most recently pushed collection
    t.popMapping();
    cout << t.getTranslation(secret) << endl; // writes Re?d?!
    // Pop again
    t.popMapping();
    cout << t.getTranslation(secret) << endl; // writes ?????!
}
*/

/*
void f()
{
    Translator t;
    t.pushMapping("DHL", "ERD"); // DàE, HàR, LàD
    if ( ! t.pushMapping("QX", "RY")) // QàR, XàY
        cout << "Both H and Q would map to R!" << endl;
    // The current mapping is still DàE, HàR, LàD with no
    // mapping for Q or X
    cout << t.getTranslation("HDX") << endl; // writes RE?
    if ( ! t.pushMapping("H", "S")) // HàS
        cout << "H would map to both R and S!" << endl;
}
 */

/*
int main() {
    f();
}
    */

int main() {
    //cout << encrypt("Please encrypt this message for me!!") << endl;
    //decrypt("y qook ra bdttook yqkook.");
    //decrypt("Vxgvab sovi jh pjhk cevc andi ngh iobnxdcjnh cn bdttook jb pnio jpfnicvhc cevh vha nceoi nho cejhy.");
    //decrypt("Lzdkgd dyrmjls shcg xdggkud fpm xd!!");
    //decrypt("Trcy oyc koon oz rweelycbb vmobcb, wyogrcn oecyb; hjg ozgcy tc moox bo moya wg grc vmobck koon grwg tc ko yog bcc grc oyc trlvr rwb hccy oecyck zon jb. -Rcmcy Xcmmcn");
    //decrypt("Jxwpjq qwrla glcu pcx qcn xkvv dw uclw ekarbbckpjwe dq jzw jzkpta jzrj qcn ekep'j ec jzrp dq jzw cpwa qcn eke ec. -Urls Jxrkp");
    //decrypt("Xjzwq gjz cuvq xz huri arwqvudiy fuk ufjrqoq svquxiy. -Lzjk Nqkkqcy");
    decrypt("Axevfvu lvnelvp bxqp mvpprjv rgl bvoop Grnxvgkvuj dqupb jvbp buvrbvl be lqggvu.");
}
