#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <streambuf>
#include <cassert>
using namespace std;
// declaring all the functions to be used
void processBreaks(ostream& outf);
bool processWord(char wordB[], ostream& outf, bool& nextWord, bool& breaks, int length, int& count, bool& firstSpace, bool& isPunct, bool& lastWord, bool& nextWordAfterPunct);
bool processRender(char token[], int length, int& count, ostream& outf, bool& firstSpace, bool& isPunct, bool& lastWord);
bool renderOut(int& count, char pWord[], ostream& outf, int length, bool& firstPortion, bool& firstSpace, bool& isPunct, bool& lastWord);

int render(int lineLength, istream& inf, ostream& outf) {// main render function
    if (lineLength < 1) { //checks invalid lengths
        return 2;
    }

    // boolean variables used later as certain references for 
    int charCount = 0;
    char c;
    int overflowCount = 0;
    char characters[181];
    bool firstWord = false;
    bool nextWord = false;
    bool nextWordAfterPunct=false;
    bool breaks = false;
    bool success = true;
    bool check = true;
    bool firstSpace=false;
    bool isPunct=false;
    bool lastWord=false;
    
    while (inf.get(c)) {//getting all the characters

        if (!isspace(c)) { //takes in characters from a word excluding space
            characters[charCount] = c;
            charCount++;
        }
        else {// if is space, sets null character to charCount (the size of the word)
            characters[charCount] = '\0';
            if (characters[0] != '\0') {
                if (firstWord == false) {//only processes if the firstWord of a file is a word and not a break
                    if (strcmp(characters, "@P@") != 0) {
                        firstWord = true;
                     
                        check = processWord(characters, outf, nextWord, breaks, lineLength, overflowCount,firstSpace, isPunct,lastWord, nextWordAfterPunct);
                        if (!check) {// if failure of word to fit in length, success is set false
                            success = false;
                        }

                    }
                }
                else {// for all words that come after first word, they are processed, and if not sucessful word fit, saves it in boolean variable
                
                    check = processWord(characters, outf, nextWord, breaks, lineLength, overflowCount, firstSpace, isPunct, lastWord, nextWordAfterPunct);
                    if (!check) {
                        success = false;
                    }

                }
                charCount = 0; //sets charcount to 0 so new word can be processed
            }
        }


    }


    characters[charCount] = '\0'; //for last words, last words can't be processed inside the function as there may be no space after them, and thus this processes the lastword
    lastWord=true; //this variable tells functions not to produce any spaces after the word, even if there is a punctation
    check = processWord(characters, outf, nextWord, breaks, lineLength, overflowCount, firstSpace, isPunct, lastWord, nextWordAfterPunct);
    if (overflowCount!=0){// if the processWord function already produced an endline, don't produce, otherwise produce a break
    outf<<endl;
    }
    // determines the return of the function based on if success was false
    if (!check) {
        success = false;
    }

    if (success) { 
    return 0; 
    }
    else { 
    return 1; 
    }
}

void processBreaks(ostream& outf) {// when called, outputs two breaks
    outf << endl << endl;

    return;
}


bool processWord(char wordB[], ostream& outf, bool& nextWord, bool& breaks, int length, int& count, bool& firstSpace, bool& isPunct, bool& lastWord, bool& nextWordAfterPunct) {
    int wbLength=strlen(wordB); //checks word length
    bool check = true;  //sets variable check to true, used later as method to determine success of word
    
    //if found @P@ sets boolean variable breaks to true
    if (strcmp(wordB, "@P@") == 0) { 
        breaks = true;
    }
    // as we have found a break word and waits until the next word that arrives is not an @P@ 
    if (strcmp(wordB, "@P@") != 0 && breaks == true) {

        nextWord = true;
    }

    if (strcmp(wordB, "@P@") != 0 && breaks == false) {
        check = processRender(wordB, length, count, outf, firstSpace, isPunct, lastWord);
    }

    // sets isPunct to true to tell the processRender function to produce two spaces
    if ((lastWord == false) && (wordB[(wbLength - 1)] == '.' || wordB[(wbLength - 1)] == '?' || wordB[(wbLength - 1)] == '!' || wordB[(wbLength - 1)] == ':')) {
        isPunct = true;
        
    }
   
    // as we have found a break word and the new word and then processes it

    if (nextWord == true && breaks == true) {
        processBreaks(outf);
        count = 0;
        
        check = processRender(wordB, length, count, outf, firstSpace, isPunct, lastWord);

        nextWord = false;
        breaks = false;
    }
    
    return check; //returns the check based on success of the processRender function
}

bool processRender(char token[], int length, int& count, ostream& outf, bool& firstSpace, bool& isPunct, bool& lastWord) {
    bool check=true; // determines what the actual renderOut function produced
    bool success=true; //if success false as check is false, lets this function return false
    int wordSize = strlen(token);// takes size of the given word
    int portionCount = 0; //counter variable representing size for each word portions
    char portionArr[180]; //array to break word into portions
    bool firstPortion = false;
    for (int i = 0; i != wordSize; i++) { //loops though whole word
    //the only way word portions can end is if the word has ended or the dash is present. And thus...
        if (token[i] == '-') { //if dash present,save dash in array, set array of index portionCount+1 to null, and process

            portionArr[portionCount] = token[i];
            portionArr[portionCount + 1] = '\0';
            check=renderOut(count, portionArr, outf, length, firstPortion, firstSpace, isPunct,lastWord);
            if(!check){ // if check was false, lets the function know through success being set to false
            success=false;
            }
            portionCount = 0;// set index of counter to 0 to process new portion again in next loop iteration
        }
        else {

            portionArr[portionCount] = token[i]; // put characters into wordportion array
            portionCount++;
            if (token[i] != '-' && (i + 1) == wordSize) {// if word has ended, process word
                portionArr[portionCount] = '\0';
                check=renderOut(count, portionArr, outf, length, firstPortion, firstSpace, isPunct, lastWord);
                if (!check) {// same as previous code above
                    success = false;
                }
            }
        }

    }
    
   
    return success; //return the success of the text fitting
}

bool renderOut(int& count, char pWord[], ostream& outf, int length, bool& firstPortion, bool& firstSpace, bool& isPunct, bool& lastWord) {

    int pWordSize = strlen(pWord); //takes size of word portion
  
    if (!firstSpace){// if this is first word, if so, doesn't output spaces before the word
    firstSpace=true;
    } else{ // if word not a first word of input file, check if this word is the first portion of the word to output spaces
    if (!firstPortion ){
        if (isPunct == true && (!lastWord)) {// if there is a function and the spaces can fit within given length, produces two output spaces

                if ((count + 2 + pWordSize) <= length) {
                    if (count != 0) {
                   outf << ' '<<' ';
                    count += 2;
                    }
                }
                else{ // if spaces can't fit, newline produces with count set to 0
                    outf << endl;
                    count = 0;
                }
            
            isPunct = false; //sets isPunct to false
           
        }else{ // if not a punctation and is a normal space, check if space an fit and produce a space, if not endline
        if ((count + pWordSize + 1) <= length) {
            if(count!=0){
            outf << ' ';
            count++;
            }
        }
        else {
            outf << endl;
            count = 0;
        }
        }
        firstPortion = true;
        
    }
  
    }

    bool checkIfNoOverflow = true; // boolean variable to tell the function that it doesn't fit
    if (pWordSize + count <= length) {// if word fits within length
        outf << pWord;
        count += pWordSize;
       
    }
    else {// if not
        if (count != 0) {// if not already on a new line, produce new line
            outf << endl;
            count = 0;
        }
        if (pWordSize + count <= length) {// checks if word first in the empty line
            outf << pWord;
            count += pWordSize;
        }
        else {// if not, boolean variable sets to false indiciating word to big for given length
            checkIfNoOverflow = false;
            for (int i = 0; i != pWordSize; i++) {//iterates through the word and wraps text accordingly
                if (count != length) {
                    outf << pWord[i];
                    count++;
                }
                else {
                    outf << endl;
                    count = 0;
                    outf << pWord[i];
                    count++;
                }
            }
        }
    }
    return checkIfNoOverflow; // returns whether the portion fits or not
}

int render(int lineLength, istream& inf, ostream& outf);

void testRender(int lineLength, const char input[], const char expectedOutput[], int expectedReturnValue)
{
    istringstream iss(input);
    ostringstream oss;
    ostringstream dummy;
    streambuf* origCout = cout.rdbuf(dummy.rdbuf());
    int retval = render(lineLength, iss, oss);
    cout.rdbuf(origCout);
    if (!dummy.str().empty())
        cerr << "WROTE TO COUT INSTEAD OF THIRD PARAMETER FOR: " << input << endl;
    else if (retval != expectedReturnValue)
        cerr << "WRONG RETURN VALUE FOR: " << input << endl;
    else if (retval == 2)
    {
        if (!oss.str().empty())
            cerr << "WROTE OUTPUT WHEN LINELENGTH IS " << lineLength << endl;
    }
    else if (oss.str() != expectedOutput)
        cerr << "WRONG RESULT FOR: " << input << endl;
}


int main() {
   
    
    
       
     
    
       
     const int MAX_FILENAME_LENGTH = 100;
        for (;;)
        {
            cout << "Enter input file name (or q to quit): ";
            char filename[MAX_FILENAME_LENGTH];
            cin.getline(filename, MAX_FILENAME_LENGTH);
            if (strcmp(filename, "q") == 0)
                break;
            ifstream infile(filename);
            if (!infile)
            {
                cerr << "Cannot open " << filename << "!" << endl;
                continue;
            }
            cout << "Enter maximum line length: ";
            int len;
            cin >> len;
            cin.ignore(10000, '\n');
            int returnCode = render(len, infile, cout);
            cout << "Return code is " << returnCode << endl;
        }
    
       
       
      

    
   
}