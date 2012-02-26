/*******************************************************************************
 * FILENAME:	parser.cpp
 * DESCRIPTION:	Create Recursive Descent Parser automatically from a grammar
 * AUTHOR:		James Matthew Welch [JMW]
 * SCHOOL:		Arizona State University
 * CLASS:		CSE340: Principles of Programming Languages
 * INSTRUCTOR:	Dr. Toni Farley
 * SECTION:		27199
 * TERM:		Spring 2012
 ******************************************************************************/
/* TODO name turn in based on rules from Syllabus*/
/* TODO test on general.asu.edu*/
/* Recursive Descent Parser Main Program:  */

#include <cstdlib> // atoi(), exit()
#include <stdio.h>
#include <ctype.h> 	/* isspace(), isdigit(), isalpha(), isalnum() */
#include <string.h> /* strcmp() */

#include <string>
using std::string;
using std::getline;

#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include <fstream>
using std::ifstream;

#include <sstream>
using std::stringstream;

#include <iterator>
#include <vector>
#include <istream>
using namespace std;

#include "GrammarRule.hpp"
#include "Symbols.hpp"

//#define DEBUG

void PrintError(int errCode); 						// public method
void BuildTermTypeMap(); 							// private method
void BuildGrammarSymbolMap(); 						// private method
TermSymbolType FindTermType(string token); 			// public method
TermSymbolType FindFirst(vector<string> tokenList); // public method
GramSymbolType FindGrammarSymbol(string token);		// public method
bool isValidNonTerm(string token); 					// public method
vector<string> tokenize(const string & str, const string & delim); // public method
void AddNonTermRule(string name); 					// public method of Grammar

map <string, TermSymbolType> termMap; // private data member of Grammar
map <char, GramSymbolType> grammarSymbolMap; //private data member of Grammar
map <string, NonTermSym> ruleMap; // private data member of Grammar


/*----------------------------------------------------------*/
/* Global Variables associated with the next input token*/
#define MAX_TOKEN_LENGTH 100

char token[MAX_TOKEN_LENGTH]; /* token string*/
int ttype; /* token type*/

int tokenLength;

int line_no = 1;

NonTermSym *rules;

/*----------------------------------------------------------*/

// beginning of recursive Descent Parser driver
int main ( int argc, char *argv[] )
{
	if (argc != 2) // argc should be 2 for correct execution
		// We print argv[0] assuming it is the program name
		cout << "usage: " << argv[0] << " <filename>\n";
	else {
		// Assume argv[1] is a grammar filename to open
		ifstream inFilePtr(argv[1]);

		// check to see if file opening succeeded
		if (inFilePtr.fail())
			cout << "Error opening file \"" << argv[1] << "\"\n";
		else {

			cout << endl << endl
					<< ":::Welcome to the recursive descent parser:::" << endl
					<< endl;

			string strRule;
			string name;
			int numRules = 0;

			while (inFilePtr.good()) {
				getline(inFilePtr, strRule);
				if (inFilePtr.good()) {
					cout << strRule << endl;
					++numRules;
				}
			}
			cout << numRules << " rules found." << endl<< endl;

			char proceed;

			inFilePtr.clear(); // forget we hit the end of file
			inFilePtr.seekg(0, ios::beg); // move to the start of the file

			BuildGrammarSymbolMap();
			BuildTermTypeMap();

			map< string, vector <string> > ruleSet;
			const string delim = " ";

			rules = new NonTermSym[numRules];

			for (int i = 0; i < numRules; ++i) {
				getline(inFilePtr, strRule);

				vector< string > ruleVector;

				// get vector of tokens from line

				stringstream ruleStream(strRule);
				cout << "Whole Line:: \"" << ruleStream.str() << "\"" <<endl;

				ruleVector = tokenize(strRule, delim);
#ifdef DEBUG
				cout << "NumTOkensFOund: " << ruleVector.size() << endl;
#endif


				TermSymbolType termType = FindFirst(ruleVector);

				cout << endl << endl << endl;
			}

			// close file before terminating program
			inFilePtr.close();
		}

	}
	cout << endl << endl;
	return 0;
}

vector<string> tokenize(const string & str, const string & delim) {
	// this function borrowed form the following page:
	// http://www.gamedev.net/topic/320087-c-string-parsing-with-stringstream/
	vector<string> tokens;

	size_t p0 = 0, p1 = string::npos;
	while (p0 != string::npos) {
		p1 = str.find_first_of(delim, p0);
		if (p1 != p0) {
			string token = str.substr(p0, p1 - p0);
			tokens.push_back(token);
		}
		p0 = str.find_first_not_of(delim, p1);
	}

	return tokens;
}


TermSymbolType FindFirst(vector<string> tokenList){
	string token;
	bool validNT;
	int tokenLen;
	GramSymbolType gramSym;
	TermSymbolType termSym;
	int ruleNum = 0;
	NonTermSym rule;

	cout<< "Fcn: FindFirst::" << endl;
	for(vector<string>::iterator it_ii = tokenList.begin();
			it_ii != tokenList.end();
			++it_ii, ++ruleNum)
	{
		token = *it_ii;
		tokenLen = token.length();

		gramSym = FindGrammarSymbol(token);
		termSym = FindTermType(token);

		if(it_ii == tokenList.begin()){
			// this is the first token in the list, must be valid NT
			if(gramSym != GS_NONE){
				PrintError(0);
			}else if(termSym != TS_NONE){
				PrintError(2);
			}else{
				validNT = isValidNonTerm(token);
				if(validNT){
					cout << "NonTerminal found:\t" << token << endl;
					AddNonTermRule(token);

				}
			}
			continue;
		}


		cout << "Consider token: " << token << endl;

		if(gramSym != GS_NONE){	// reserved symbol
			// make sure token is not a reserved symbol ( []{}- )
			// need some logic here
			cout << "Grammar found:\t" << gramSym <<", "<<  token << endl;
			if(gramSym == GS_LBRACEOPT || gramSym == GS_OR){
				// optional part, include the next token's FIRST set in this token's FIRST set
				// include the FIRST() from the symbol inside the Lbrace, then skip past Rbrace
				// if length > 1, there's something else hiding in this token

			}else if (gramSym == GS_LBRACKET){
				// repetition, important for FOLLOW, but not FIRST

			}
		}else{
			// not a grammar symbol, look for terminal
			;
			if(termSym != TS_NONE){
				// this is a terminal symbol
				// add the terminal to the first set of NT
				cout << "Terminal found:\t" << token << endl;
			}else {
				// this is a nonterminal, make a new NT object

				validNT = isValidNonTerm(token);
				if(validNT){
					cout << "NonTerminal found:\t" << token << endl;
				}

			}

		}

	}
	return TS_NONE;
}

void AddNonTermRule(string name){
	if(ruleMap.find(name) == ruleMap.end()){
		// rule is not yet present in the map, add it
		ruleMap[token] = NonTermSym(token);
	}
}

TermSymbolType FindTermType(string token){
	// token is the key value into the map
	TermSymbolType returnSymbol;

	//  declare iterator to find the key
	map<string, TermSymbolType>::iterator it;
	it = termMap.find(token);

	if(it == termMap.end()){
		// symbol is unfound, return TS_NONE
		returnSymbol = TS_NONE;
	}else{
		returnSymbol = it->second;
	}

	// return the TermSymbol from the map
	return returnSymbol;
}

GramSymbolType FindGrammarSymbol(string token)
{
	GramSymbolType returnSymbol;

	//  declare iterator to find the key
	map<char,GramSymbolType>::iterator it;
	it = grammarSymbolMap.find(token[0]);

	if(it == grammarSymbolMap.end()){
		// not a grammar symbol
		returnSymbol = GS_NONE;
	} else {
		returnSymbol = it->second;
	}
	return returnSymbol;
}


bool isValidNonTerm(string token){

	char c = token[0];
	int numChar = 0;
	int tokenLength = token.length();

	if(isdigit(c)){
		return false;
	}else if(isalnum(c)){
		// valid beginning to a NonTerm
		// check remaining characters for alphanumeric
		while(isalnum(c) && (numChar <= tokenLength)){
			numChar++;
			c = token[numChar];
		}
	}
#ifdef DEBUG
	cout << " numchar: " << numChar << "::: tokenLength: " << tokenLength << endl;
#endif
	if(numChar == tokenLength)
		return true;
	else
		return false;
}

void PrintError(int errCode){
	string message;
	switch (errCode){
	case 1:
		message = "non-terminal symbol listed in a rule but doesn't have a description";
		break;
	case 2:
		message = "terminal symbol appearing on LHS of a rule";
		break;
	default: //0
		message = "input not according to format";
		break;
	}
	cout << endl << endl << "ERROR CODE " << errCode << " :: " << message << endl;
	exit(1);
}

void BuildTermTypeMap(){
	// clear map
	termMap.clear();

	// refill the map with terminal symbol types
	termMap["VAR"] 			= TS_VAR;
	termMap["BEGIN"] 		= TS_BEGIN;
	termMap["END"] 			= TS_END;
	termMap["ASSIGN"] 		= TS_ASSIGN;
	termMap["IF"] 			= TS_IF;
	termMap["WHILE"]		= TS_WHILE;
	termMap["DO"]			= TS_DO;
	termMap["THEN"] 		= TS_THEN;
	termMap["PRINT"] 		= TS_PRINT;
	termMap["INT"] 			= TS_INT;
	termMap["REAL"] 		= TS_REAL;
	termMap["STRING"] 		= TS_STRING;
	termMap["PLUS"] 		= TS_PLUS;
	termMap["MINUS"]		= TS_MINUS;
	termMap["UNDERSCORE"] 	= TS_UNDERSCORE;
	termMap["DIV"] 			= TS_DIV;
	termMap["MULT"] 		= TS_MULT;
	termMap["EQUAL"] 		= TS_EQUAL;
	termMap["COLON"] 		= TS_COLON;
	termMap["COMMA"] 		= TS_COMMA;
	termMap["SEMICOLON"] 	= TS_SEMICOLON;
	termMap["LBRAC"] 		= TS_LBRAC;
	termMap["RBRAC"] 		= TS_RBRAC;
	termMap["LPAREN"] 		= TS_LPAREN;
	termMap["RPAREN"] 		= TS_RPAREN;
	termMap["NOTEQUAL"] 	= TS_NOTEQUAL;
	termMap["GREATER"] 		= TS_GREATER;
	termMap["LESS"] 		= TS_LESS;
	termMap["LTEQ"] 		= TS_LTEQ;
	termMap["GTEQ"] 		= TS_GTEQ;
	termMap["DOT"] 			= TS_DOT;
	termMap["ID"] 			= TS_ID;
	termMap["NUM"] 			= TS_NUM;
	termMap["REALNUM"] 		= TS_REALNUM;
}

void BuildGrammarSymbolMap(){
	// clear map
	termMap.clear();

	// fill map
	grammarSymbolMap['-']	= GS_DASH;
	grammarSymbolMap['[']	= GS_LBRACEOPT;
	grammarSymbolMap[']']	= GS_RBRACEOPT;
	grammarSymbolMap['{']	= GS_LBRACKET;
	grammarSymbolMap['}']	= GS_RBRACKET;
	grammarSymbolMap['|']	= GS_OR;
}
