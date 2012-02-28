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

#endif

void PrintError(int errCode); 						// public method
void BuildTermTypeMap(); 							// private method
void BuildGrammarSymbolMap(); 						// private method
TermSymbolType FindTermType(string token); 			// public method
void BuildFirstSet(vector<string> tokenList); // public method
GramSymbolType FindGrammarSymbol(string token);		// public method
bool isValidNonTerm(string token); 					// public method
vector<string> tokenize(const string & str, const string & delim); // public method
void AddNonTermRule(string name); 					// public method of Grammar
void AddTermToFirst(TermSymbolType terminal, string nonTermKey);
void AddTermToFollow(TermSymbolType terminal, string nonTermKey);

/*----------------------------------------------------------*/
/* Global Variables associated with the next input token*/
#define MAX_TOKEN_LENGTH 100

char token[MAX_TOKEN_LENGTH]; /* token string*/
int ttype; /* token type*/

int tokenLength;

int line_no = 1;

NonTerminal *rules;
map <string, TermSymbolType> termMap; // private data member of Grammar
map <char, GramSymbolType> grammarSymbolMap; //private data member of Grammar
map <string, NonTerminal> ruleMap; // private data member of Grammar
stack<NonTerminal> 	_unprocessed; // private data member of Grammar

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

#ifdef DEBUG
			cout << "Reading in File first time, counting rules:::" << endl;
#endif
			while (inFilePtr.good()) {
				getline(inFilePtr, strRule);
				if (inFilePtr.good()) {
					cout << strRule << endl;
					++numRules;
				}
			}
			cout << numRules << " rules found." << endl<< endl;

			//char proceed;

			inFilePtr.clear(); // forget we hit the end of file
			inFilePtr.seekg(0, ios::beg); // move to the start of the file

			BuildGrammarSymbolMap();
			BuildTermTypeMap();

			map< string, vector <string> > ruleSet;
			const string delim = " ";

			rules = new NonTerminal[numRules];

			for (int i = 0; i < numRules; ++i) {
				getline(inFilePtr, strRule);

				vector< string > ruleVector;

				// get vector of tokens from line

				stringstream ruleStream(strRule);
				cout << "Whole Line:: \"" << ruleStream.str() << "\"" <<endl;

				ruleVector = tokenize(strRule, delim);
#ifdef DEBUG
				cout << "NumTokensFound: " << ruleVector.size() << endl;
#endif
				if(ruleVector.size() < 3){
					// not enough tokens to build a rule (NonTerm,-,Term)
					PrintError(0);
				}

				BuildFirstSet(ruleVector);


				cout << endl << endl << endl;
			}

			// close file before terminating program
			inFilePtr.close();
		}

	}
	cout << endl << endl;
	return 0;
}


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
			rulePtr = ruleMap.find(NTKey);
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
			if(gramSym == GS_LBRACEOPT || gramSym == GS_OR){
				// optional part, include the next token's FIRST set in this token's FIRST set
				// include the FIRST() from the symbol inside the LBrace, then skip past Rbrace

				// add following terminals and terminals after RBrace
				it_ii++;
				tokenNum++;
				token = *it_ii;
				gramSym = FindGrammarSymbol(token);
				termSym = FindTermType(token);


			}else if (gramSym == GS_LBRACKET){
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
		ruleMap[NTKey].SetComplete(false);
	}
	ruleMap[NTKey].PrintFirstSet();
}




void AddNonTermRule(string nonTermName){
	cout << "NonTerminal found:\t" << nonTermName << endl;
	if(ruleMap.find(nonTermName) == ruleMap.end()){
		// nonterm is not yet present in the map, add it
		ruleMap[nonTermName] = NonTerminal(nonTermName);
		if(ruleMap.size() == 1){
			// just added 1st NT ==> start symbol
			ruleMap[nonTermName].SetStartSymbol();
		}
	}
}

void AddTermToFirst(TermSymbolType terminal, string nonTermKey){
	cout << "Terminal found:\t" << terminal << ", adding to First(" << nonTermKey << ")" << endl;
	map<string, NonTerminal>::iterator it = ruleMap.find(nonTermKey);
	if(it != ruleMap.end()){
		// nonterm is present in the map, add Term to First(nonTermKey)
		it->second.AddToFirst(terminal);
	}
}

void AddTermToFollow(TermSymbolType terminal, string nonTermKey){
	cout << "Terminal found:\t" << terminal << ", adding to Follow(" << nonTermKey << ")" << endl;
	map<string, NonTerminal>::iterator it = ruleMap.find(nonTermKey);
	if(it != ruleMap.end()){
		// nonterm is present in the map, add Term to First(nonTermKey)
		it->second.AddToFollow(terminal);
	}
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

	// fill the terminal map with terminal symbol types
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
	grammarSymbolMap.clear();

	// fill map
	grammarSymbolMap['-']	= GS_DASH;
	grammarSymbolMap['[']	= GS_LBRACEOPT;
	grammarSymbolMap[']']	= GS_RBRACEOPT;
	grammarSymbolMap['{']	= GS_LBRACKET;
	grammarSymbolMap['}']	= GS_RBRACKET;
	grammarSymbolMap['|']	= GS_OR;
}
