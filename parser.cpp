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

#include "NonTerminal.hpp"

#if 1
/*------------------- reserved words and token strings -----------------------*/
string reserved[] = { "", "VAR", "BEGIN", "END", "ASSIGN", "IF", "WHILE", "DO",
		"THEN", "PRINT", "INT", "REAL", "STRING", "+", "-", "_", "/", "*", "=",
		":", ",", ";", "[", "]", "(", ")", "<>", ">", "<", "<=", ">=", "<<",
		">>", ".", "ID", "NUM", "REALNUM", "ERROR" };

//string NonTerminal::TermStrings;

#endif

vector<string> 	tokenize(const string & str, const string & delim);
void 			PrintError(int errCode);

/*
// public methods for Grammar class

TermSymbolType 	FindTermType(string token);
void 			BuildFirstSet(vector<string> tokenList);
GramSymbolType 	FindGrammarSymbol(string token);
bool 			isValidNonTerm(string token);

void 			AddNonTermRule(string name);
void 			AddTermToFirst(TermSymbolType terminal, string nonTermKey);
void 			AddTermToFollow(TermSymbolType terminal, string nonTermKey);

// private methods for Grammar class
//void 			g_BuildTermTypeMap(); 							// private method
//void 			g_BuildGrammarSymbolMap(); 						// private method
*/


/*----------------------------------------------------------*/
/* Global Variables associated with the next input token*/
#define MAX_TOKEN_LENGTH 100

char token[MAX_TOKEN_LENGTH]; /* token string*/

int tokenLength;

/*
map <string, TermSymbolType> g_termMap; // private data member of Grammar
map <char, GramSymbolType> g_grammarSymbolMap; //private data member of Grammar

*/

/*
// Define bytecount in file scope.
map <string, TermSymbolType>  NonTerminal::termSymbolMap;
map <char, GramSymbolType> NonTerminal::grammarSymbolMap;
string NonTerminal::termStrings[] = {"NONE", "VAR", "BEGIN", "END", "ASSIGN", "IF", "WHILE", "DO", "THEN", "PRINT", "INT", "REAL", "STRING",
		"PLUS", "MINUS", "UNDERSCORE", "DIV", "MULT", "EQUAL", "COLON", "COMMA", "SEMICOLON",
		"LBRAC", "RBRAC", "LPAREN", "RPAREN", "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ", "DOT",
		"ID", "NUM", "REALNUM"};
*/
map <string, NonTerminal> g_ruleMap;

// beginning of recursive Descent Parser driver
int main ( int argc, char *argv[] )
{
	if (argc != 2){ // argc should be 2 for correct execution
		// We print argv[0] assuming it is the program name
		cout << "usage: " << argv[0] << " <filename>\n";
		exit(1);
	}

	// Assume argv[1] is a grammar filename to open
	ifstream inFilePtr(argv[1]);

	// check to see if file opening succeeded
	if (inFilePtr.fail()){
		cout << "Error opening file \"" << argv[1] << "\"\n";
		exit(1);
	}

	cout << endl << ":::Grammar First & Follow Sets:::" << endl;

	// create local variables
	// queue to hold nonterminals as they're built
	vector<NonTerminal> newNTvector;
	string strRule;
	string name;
	int numRules = 0;
	vector<string> tokenVector;
	const string delim = " ";
	int numTokens = 0;
	NonTerminal *nextNT;

	while (inFilePtr.good()) {
		getline(inFilePtr, strRule);
		if (inFilePtr.good()) { //TODO: why check inFilePtr.good() twice??
#ifdef DEBUG
			cout << "Line("<< numRules <<"):: \"" << strRule << "\"" <<endl;
#endif
			tokenVector = tokenize(strRule, delim);
			numTokens = tokenVector.size();

#ifdef DEBUG
			cout << "NumTokensFound: " << numTokens << endl;
#endif
			if (numTokens < 3) {
				// not enough tokens to build a rule (NonTerm,-,Term)
				PrintError(0);
			}

			// build a NonTerminal which may be pushed to the stack
			map<string, NonTerminal>::iterator it = g_ruleMap.find(tokenVector[0]);
			if(it == g_ruleMap.end()){// if no rule exists yet, make one
				nextNT = new NonTerminal(tokenVector,numRules);
			}else{// grab the existing rule from the map
				nextNT = &it->second;
			}
			if(numRules == 0){
				nextNT->SetStartSymbol();
			}

			g_ruleMap[nextNT->GetName()] = *nextNT;
			newNTvector.push_back(*nextNT);
#ifdef DEBUG
			cout << "stack contains " << newNTvector.size() << " items." << endl << endl;
#endif
			++numRules;
		}
	}
	// close file before terminating program
	inFilePtr.close();

#ifdef DEBUG
	cout << numRules << " rules found." << endl<< endl;
#endif

	// parse back through rules from first to last, unioning first sets of NTs
	// UNTIL no changes are made
	// use a vector and cycle through continuously until modified is false
	queue<NonTerminal> firstsDone;
	int modCount = 0;
	do{
		modCount = 0;
		for(vector<NonTerminal>::iterator thisNT = newNTvector.begin();
				thisNT != newNTvector.end();
				++thisNT)
		{
			vector<string> nonTermsInFirst = thisNT->GetFirstNTs();

#ifdef DEBUG
			cout << "reprocessing NT(" << thisNT->GetName() << "): mod:" << modCount << endl;
#endif

			// look at the nonTerminals listed in this NonTerminal's _nonTermTokens set
			for(vector<string>::iterator it = nonTermsInFirst.begin();
					it != nonTermsInFirst.end();
					++it){
				// *it is a string token from the list of NonTerminals in the FIRST()
				map <string, NonTerminal>::iterator it_nt= g_ruleMap.find(*it);
				if(it_nt == g_ruleMap.end()){
					// the nonTerminal in question was unfound==>error1
					PrintError(1);
				}else{
					// found a matching rule, add its FIRST() to this NT's FIRST()
					modCount += thisNT->UnionFirstSets(it_nt->second);
				}
			}
			if(modCount > 0){
				g_ruleMap[thisNT->GetName()] = *thisNT;
			}
		}
	}while(modCount > 0);

	queue<NonTerminal> followsDone;
	// parse for follow sets
	for(vector<NonTerminal>::iterator thisNT = newNTvector.begin();
			thisNT != newNTvector.end();
					++thisNT)
	{
		thisNT->GetFirstNTs();


		if(thisNT->IsStartSymbol()){
			thisNT->AddToFollow(TS_EOF);
		}


		followsDone.push(*thisNT);
	}

	// print FIRST sets
	queue<NonTerminal> firstsPrinted;
	while(!followsDone.empty()){
		NonTerminal tempNT = followsDone.front();
		followsDone.pop();
		tempNT.PrintFirstSet();
		firstsPrinted.push(tempNT);
	}

	// print FOLLOW sets
	while(!firstsPrinted.empty()){
		NonTerminal tempNT = firstsPrinted.front();
		firstsPrinted.pop();
		tempNT.PrintFollowSet();
	}

	// DONE!
	cout << endl;
	return(0);
}

vector<string> tokenize(const string & str, const string & delim) {
	vector<string> tokens;

	const char * line = str.c_str();
	const int lineLength = str.size();
	tokenLength = MAX_TOKEN_LENGTH;
	int pos = 0;
	char c = line[pos];

	while(pos < lineLength){// tokenize until end of line
		for(int i = 0;i < tokenLength; ++i){
			token[i]='\0';
		}
		tokenLength = 0;

		while(isspace(c)){
			pos++;
			c = line[pos];
		}
		if(ispunct(c)){
			token[tokenLength] = c;
			tokenLength++;// one past length so clear token
			pos++;
			c = line[pos];
			tokens.push_back(string(token));
		}else if(isdigit(c)){
			do{
				token[tokenLength] = c;
				tokenLength++;
				pos++;
				c = line[pos];
			}while(isalnum(c));
			tokens.push_back(string(token));
		}else if(isalpha(c)){
			do{
				token[tokenLength] = c;
				tokenLength++;
				pos++;
				c = line[pos];
			}while(isalnum(c));
			tokens.push_back(string(token));
		}
	}
	return tokens;
}

void PrintError(int errCode){
	string message;
	switch (errCode){
	case 0:
		message = "input not according to format";
		break;
	case 1:
		message = "non-terminal symbol listed in a rule but doesn't have a description";
		break;
	case 2:
		message = "terminal symbol appearing on LHS of a rule";
		break;
	default: //0
		message = "unknown error, kinda like a BSOD";
		break;
	}
	cout << endl << endl << "ERROR CODE " << errCode << " :: " << message << endl << endl;
	exit(1);
}
