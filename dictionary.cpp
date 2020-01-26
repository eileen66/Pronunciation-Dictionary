#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <climits>
#include <iomanip>
using namespace std;

bool validWord(string word){
    for (int i = 0; i < word.length(); i++) {
        if (!isalpha(word[i]) && word[i] != char(39))
            return false;
    }
    return true;
}

void splitOnSpace(string s, string & before, string & after) {
  // reset strings
    before = ""; 
    after = "";
  // accumulate before space
    int i = 0;
    while (i < s.size() && !isspace(s[i])) { 
        before = before + s[i]; 
        i++; 
    }
  // skip the space
    i++;
    i++;
    // accumulate after space
    while (i < s.size()) { 
        after = after + s[i]; 
        i++; 
    }  
}

void split(string s, string & before, string & after) {
  // reset strings
    before = ""; 
    after = "";
  // accumulate before space
    int i = 0;
    while (i < s.size() && !isspace(s[i])) { 
        before = before + s[i]; 
        i++; 
    }
  // skip the space
    i++;
    //i++;
    // accumulate after space
    while (i < s.size()) { 
        after = after + s[i]; 
        i++; 
    }
}

string pronunciation(string word, ifstream & fin){

    string pronounce = "";
    string line;
    string before = "";
    string after = "";
    
    while (getline(fin, line)) {
        splitOnSpace(line, before, after);
        if (before == word){
            pronounce = after;
            return pronounce;
        }
    }
    return pronounce;
}


void identical(string pronounce, ifstream & fin, string word) {

    fin.clear();
    fin.seekg(0);

    string line;
    string before = "";
    string after = "";

    for (int i = 0; i < 64; i++) {
        getline(fin, line);
    }

    while (getline(fin, line)) {
        splitOnSpace(line, before, after);         
        // checks if phonemes are the same and is a different word
        if (after == pronounce && before != word && validWord(before)) {    
            cout << before << " ";
        }
    }
    cout << endl;
} 
//counts number of phonemes
int numOfPhonemes (string s) {
    int count = 0;
    for (int i = 0; i < s.length() - 1; i++) {
        if (s[i] == ' ')
            count++;
    }
    return count + 1;
}
// checks if two phonemes are the same
bool comparePhonemes(string a, string b) {
    if (a == b) {
        return true;
    }
    else 
        return false;
}

void countDiff(string phoneme, string otherPhoneme, int numWord, string otherWord, string replaceOrAdd){

    string before = "";
    string after = "";
    split(phoneme, before, after);              // get first phoneme
    string otherB = "";
    string otherA = "";
    split(otherPhoneme, otherB , otherA);

    int count = 0;
    
    if (replaceOrAdd == "replace") {
        
        for (int i = 0; i < numWord; i++) {
            if (count > 1) {                        // if more than one is different then return
                return;
            }

            if (!comparePhonemes(before, otherB)) { 
            count++;                                // increase count if different
            }

            split(after, before, after);            // go to next phoneme for both words
            split(otherA, otherB, otherA);
        }
    }
    else if (replaceOrAdd == "add") {
        for (int i = 0; i < numWord + 1; i++) {
            if (count > 1) {                        // if more than one is different then return
                return;
            }

            if (!comparePhonemes(before, otherB)){  // if there is a difference
                split(otherA, otherB, otherA);      // then only change phonome of other word to compare with same phoneme of your  word
                count++;                            // and increase count.
            }
        
            else {                                  // phoneme are the same
                split(after, before, after);        // change phonome of both words
                split(otherA, otherB, otherA);
            }
        }
    }
    else if (replaceOrAdd == "remove") {
        for (int i = 0; i < numWord; i++) {
            if (count > 1) {                        // if more than one is different then return
                return;
            }

            if (!comparePhonemes(before, otherB)){  // if there is a difference
                split(after, before, after);        // then only change phonome of word to compare with same phoneme of other word
                count++;                            // and increase count.
            }
        
            else {                                  // phoneme are the same
                split(after, before, after);        // change phonome of both words
                split(otherA, otherB, otherA);
            }
        }
    }
    if (count == 1) 
        if(validWord(otherWord))                            // print out other word only if there is one difference
            cout << otherWord << " ";
}

//
void changePhoneme(ifstream & fin, string phoneme, string change) {
    
    fin.clear();
    fin.seekg(0);

    string line;

    for (int i = 0; i < 65; i++) {
        string junk;
        getline(fin, line);
    }
    while (getline(fin, line)) {

        string otherWord = "";
        string otherPhoneme = "";
        splitOnSpace(line, otherWord, otherPhoneme);

        int numWord = numOfPhonemes(phoneme);
        int numOther = numOfPhonemes(otherPhoneme);

        if(change == "replace"){
            if (numWord == numOther) {
                countDiff(phoneme, otherPhoneme, numWord, otherWord, change);
            }
        }
        else if (change == "add") {
            if (numWord == numOther - 1){
                countDiff(phoneme, otherPhoneme, numWord, otherWord, change);
            }
        }
        else if (change == "remove") {
            if (numWord == numOther + 1) {
                countDiff(phoneme, otherPhoneme, numWord, otherWord, change);
            }
        }
    }
}

string upper(string word) {
    string up;
    for (int i = 0; i < word.size(); i++) {
        up += toupper(word[i]);
    }
    return up;
}

int main() {
    
    ifstream fin("cmudict.0.7a.txt");
    if (fin.fail()) {
        cerr << "File cannot be opened for reading." << endl;
        exit(1); // exit if failed to open the file
    }
    
    string word;
    cout << "Enter word: ";
    cin >> word;
    if(!validWord(word)) {
        cout << "Not found" << endl;
        return 0;
    }
    word = upper(word);
    
    string pronounce = pronunciation(word, fin);
    if (pronounce != "")
        cout << left << setw(17) << "Pronunciation" << ": " << pronounce << endl << endl; 
    else {
        cout << "Not found" << endl;
        return 0;
    }  
  
    cout << left << setw(17) << "Identical" << ": ";
    identical(pronounce, fin, word);

    cout << left << setw(17) << "Replace Phoneme" << ": ";
    changePhoneme(fin, pronounce, "replace");
    cout << endl;

    cout << left << setw(17) << "Add Phoneme" << ": ";
    changePhoneme(fin, pronounce, "add");
    cout << endl;

    cout << left << setw(17) << "Remove Phoneme" << ": ";
    changePhoneme(fin, pronounce, "remove");
    cout << endl; 

    return 0; 
}
