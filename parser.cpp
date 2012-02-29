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

#ifndef DEBUG
#define DEBUG
#endif

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

	// create local variables
	// stack to hold nonterminals as they're built
	stack<NonTerminal> newNTstack;
	// stack to hold nonterminals at they are completed
	stack<NonTerminal> doneNTstack;

	// initialize the static members of NonTerminal


/*	// build maps for symbol lookup - could be more efficient
	g_BuildGrammarSymbolMap();
	g_BuildTermTypeMap();*/

	cout << endl << endl
			<< ":::Welcome to the recursive descent parser:::" << endl
			<< endl;

	string strRule;
	string name;
	int numRules = 0;
	vector<string> tokenVector;
	const string delim = " ";
	int numTokens = 0;

	while (inFilePtr.good()) {
		getline(inFilePtr, strRule);
		if (inFilePtr.good()) { //TODO: why check inFIlePtr.good() twice??
			cout << "Line("<< numRules <<"):: \"" << strRule << "\"" <<endl;

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
			NonTerminal *nextNT = new NonTerminal(tokenVector,numRules);
			if(numRules == 0){
				nextNT->SetStartSymbol();
			}

			g_ruleMap[nextNT->GetName()] = *nextNT;
			newNTstack.push(*nextNT);
			cout << endl << endl;

			cout << "stack contains " << newNTstack.size() << " items." << endl << endl;
			++numRules;
		}
	}
	// close file before terminating program
	inFilePtr.close();

	cout << numRules << " rules found." << endl<< endl;

	// parse bck through rules from last to first, defining first/follow as you go
	while(!newNTstack.empty()){
		NonTerminal tempNT = newNTstack.top();
		newNTstack.pop();

		// analyze for First/Follow

		doneNTstack.push(tempNT);

	}
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
		//cout << "token: " << token << endl;
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
	cout << endl << endl << "ERROR CODE " << errCode << " :: " << message << endl;
	exit(1);
}

#ifdef DONTUSECLASSES
void BuildFirstSet(vector<string> tokenList){
	string token;
	bool validNT;
	GramSymbolType gramSym;
	TermSymbolType termSym;
	int tokenNum = 0;
	int moreNTs = 0;

	map <string, NonTerminal>::iterator rulePtr;
	string NTKey = "";

	cout<< ":::BuildFirstSet:::" << tokenList.size() << " tokens:: " << endl;
	// first, look for NonTerm @ vector[0]

	vector<string>::iterator it_ii = tokenList.begin();
	// this is the first token in the list, must be valid NT
	token = *it_ii;
	tokenNum++;
	gramSym = FindGrammarSymbol(token);
	termSym = FindTermType(token);

	cout << "Consider token ("<< tokenNum <<"): " << token << endl;
	if(gramSym != GS_NONE){
		PrintError(0);
	}else if(termSym != TS_NONE){
		PrintError(2);
	}else{
		validNT = isValidNonTerm(token);
		if(validNT){
			AddNonTermRule(token);
			NTKey = token;
			rulePtr = g_ruleMap.find(NTKey);
		}
	}

	// increment to point at 2nd token (should be a GS_DASH)
	it_ii++;
	tokenNum++;
	token = *it_ii;
	gramSym = FindGrammarSymbol(token);
	termSym = FindTermType(token);

	cout << "Consider token ("<< tokenNum <<"): " << token << endl;
	if(gramSym != GS_DASH){
		PrintError(0);
	}

	// point to next token (first term/nonterm in production)
	it_ii++;
	tokenNum++;
	while(it_ii != tokenList.end()){
		token = *it_ii;

		// recurse here??
		gramSym = FindGrammarSymbol(token);
		termSym = FindTermType(token);

		cout << "Consider token ("<< tokenNum <<"): " << token << endl;

		if(gramSym != GS_NONE){	// reserved symbol
			// make sure token is not a reserved symbol ( []{}- )
			// need some logic here
			cout << "Grammar found:\t\'" << gramSym <<"\', \'"<<  token << "\' "<< endl;
			if(gramSym == GS_LBRACKET || gramSym == GS_OR){
				// optional part, include the next token's FIRST set in this token's FIRST set
				// include the FIRST() from the symbol inside the LBrace, then skip past Rbrace

//				// add following terminals and terminals after RBrace


			}else if (gramSym == GS_LBRACE){
				// indicates repetition, important for FOLLOW, but not FIRST

			}
		}else{
			// not a grammar symbol, look for terminal
			if(termSym != TS_NONE){
				// this is a terminal symbol
				// add the terminal to the first set of NT
				AddTermToFirst(termSym, NTKey);

			}else {
				// this is a nonterminal, make a new NT object

				validNT = isValidNonTerm(token);
				if(validNT){
					AddNonTermRule(token);
					if(tokenNum != 0){
						// a nonterminal has been found in the production, come back
						moreNTs = 1;
					}
				}else{
					// malformed non-terminal
					PrintError(0);
				}
			}
		}

		// increment iterator to point to next token
		it_ii++;
		tokenNum++;
	}
	if(moreNTs){
		g_ruleMap[NTKey].SetComplete(false);
	}
	g_ruleMap[NTKey].PrintFirstSet();
}

void AddNonTermRule(string nonTermName){
	cout << "NonTerminal found:\t" << nonTermName << endl;
	if(g_ruleMap.find(nonTermName) == g_ruleMap.end()){
		// nonterm is not yet present in the map, add it
		g_ruleMap[nonTermName] = NonTerminal(nonTermName);
		if(g_ruleMap.size() == 1){
			// just added 1st NT ==> start symbol
			g_ruleMap[nonTermName].SetStartSymbol();
		}
	}
}

void AddTermToFirst(TermSymbolType terminal, string nonTermKey){
	cout << "Terminal found:\t" << terminal << ", adding to First(" << nonTermKey << ")" << endl;
	map<string, NonTerminal>::iterator it = g_ruleMap.find(nonTermKey);
	if(it != g_ruleMap.end()){
		// nonterm is present in the map, add Term to First(nonTermKey)
		it->second.AddToFirst(terminal);
	}
}

void AddTermToFollow(TermSymbolType terminal, string nonTermKey){
	cout << "Terminal found:\t" << terminal << ", adding to Follow(" << nonTermKey << ")" << endl;
	map<string, NonTerminal>::iterator it = g_ruleMap.find(nonTermKey);
	if(it != g_ruleMap.end()){
		// nonterm is present in the map, add Term to First(nonTermKey)
		it->second.AddToFollow(terminal);
	}
}



TermSymbolType FindTermType(string token){
	// token is the key value into the map
	TermSymbolType returnSymbol;

	//  declare iterator to find the key
	map<string, TermSymbolType>::iterator it;
	it = g_termMap.find(token);

	if(it == g_termMap.end()){
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
	it = g_grammarSymbolMap.find(token[0]);

	if(it == g_grammarSymbolMap.end()){
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

	// check to see if valid NonTerm
	if(isdigit(c)){
		return false;
	}else if(ispunct(c)){
		return false;
	}else if(isalnum(c)){
		// valid beginning to a NonTerm
		// check remaining characters for alphanumeric
		while(isalnum(c) && (numChar <= tokenLength)){
			numChar++;
			c = token[numChar];
		}
	}
	if(numChar == tokenLength)
		return true;
	else
		return false;
}


void g_BuildTermTypeMap(){
	// clear map
	g_termMap.clear();

	// fill the terminal map with terminal symbol types
	g_termMap["VAR"] 			= TS_VAR;
	g_termMap["BEGIN"] 		= TS_BEGIN;
	g_termMap["END"] 			= TS_END;
	g_termMap["ASSIGN"] 		= TS_ASSIGN;
	g_termMap["IF"] 			= TS_IF;
	g_termMap["WHILE"]		= TS_WHILE;
	g_termMap["DO"]			= TS_DO;
	g_termMap["THEN"] 		= TS_THEN;
	g_termMap["PRINT"] 		= TS_PRINT;
	g_termMap["INT"] 			= TS_INT;
	g_termMap["REAL"] 		= TS_REAL;
	g_termMap["STRING"] 		= TS_STRING;
	g_termMap["PLUS"] 		= TS_PLUS;
	g_termMap["MINUS"]		= TS_MINUS;
	g_termMap["UNDERSCORE"] 	= TS_UNDERSCORE;
	g_termMap["DIV"] 			= TS_DIV;
	g_termMap["MULT"] 		= TS_MULT;
	g_termMap["EQUAL"] 		= TS_EQUAL;
	g_termMap["COLON"] 		= TS_COLON;
	g_termMap["COMMA"] 		= TS_COMMA;
	g_termMap["SEMICOLON"] 	= TS_SEMICOLON;
	g_termMap["LBRAC"] 		= TS_LBRAC;
	g_termMap["RBRAC"] 		= TS_RBRAC;
	g_termMap["LPAREN"] 		= TS_LPAREN;
	g_termMap["RPAREN"] 		= TS_RPAREN;
	g_termMap["NOTEQUAL"] 	= TS_NOTEQUAL;
	g_termMap["GREATER"] 		= TS_GREATER;
	g_termMap["LESS"] 		= TS_LESS;
	g_termMap["LTEQ"] 		= TS_LTEQ;
	g_termMap["GTEQ"] 		= TS_GTEQ;
	g_termMap["DOT"] 			= TS_DOT;
	g_termMap["ID"] 			= TS_ID;
	g_termMap["NUM"] 			= TS_NUM;
	g_termMap["REALNUM"] 		= TS_REALNUM;
}

void g_BuildGrammarSymbolMap(){
	// clear map
	g_grammarSymbolMap.clear();

	// fill map
	g_grammarSymbolMap['-']	= GS_DASH;
	g_grammarSymbolMap['[']	= GS_LBRACKET;
	g_grammarSymbolMap[']']	= GS_RBRACKET;
	g_grammarSymbolMap['{']	= GS_LBRACE;
	g_grammarSymbolMap['}']	= GS_RBRACE;
	g_grammarSymbolMap['|']	= GS_OR;
}
#endif // DONTUSECLASSES
