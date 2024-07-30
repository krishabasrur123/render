#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


int countLines(istream& inf)   // inf is a name of our choosing
{
	/*int charCount = 0;
	char c;
	char characters[1] = { '\0' };
	cout << characters << endl;
	while (inf.get(c)) {
		char temp[2];
		temp[0] = c;
		temp[1] = '\0';
		if (!(isspace(c))) {
			strcat(characters, temp);
		}
	}
	cout << characters << endl;*/

	int charCount = 0;
	char c;
	char characters[250]="\0";
	
	int count=-1;
	while (inf.get(c)) {
		char temp[2];
		temp[0] = c;
		temp[1]='\0';
		strcat(characters, temp);
	}
	cout << characters<< endl;


	

	/*int lineCount = 0;
	string line;
	while (getline(inf, line))
		lineCount++;
	*/

	return 5;
}

int main()
{
	ifstream infile("Project5Phase1.txt");
	if (!infile)
	{
		cerr << "Error: Cannot open data.txt!" << endl;
		return 1;
	}
	int fileLines = countLines(infile);  // reads from the file data.txt
	/*cout << "Project5Phase1.txt has " << fileLines << " lines." << endl;
	cout << "Type lines, then ctrl-Z (Windows) or ctrl-D (macOS/Linux):" << endl;
	int keyboardLines = countLines(cin); // reads from keyboard
	cout << "You typed " << keyboardLines << " lines." << endl;*/
}