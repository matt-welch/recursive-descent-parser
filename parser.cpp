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

#if 0
	// old method of tokenizing.  ignores punctuation
	size_t p0 = 0, p1 = string::npos;
	while (p0 != string::npos) {
		p1 = str.find_first_of(delim, p0);
		if (p1 != p0) {
			string token = str.substr(p0, p1 - p0);
			tokens.push_back(token);
		}
		p0 = str.find_first_not_of(delim, p1);
	}
#endif
	return tokens;
}


void BuildFirstSet(vector<string> tokenList){
	string token;
	bool validNT;
	GramSymbolType gramSym;
	TermSymbolType termSym;
	int ruleNum = 0;
	int tokenLength;
//	NonTerminal rule;

	cout<< ":::BuildFirstSet:::" << tokenList.size() << " tokens:: " << endl;
	for(vector<string>::iterator it_ii = tokenList.begin();
			it_ii != tokenList.end();
			++it_ii, ++ruleNum)
	{
		token = *it_ii;
		gramSym = FindGrammarSymbol(token);
		termSym = FindTermType(token);
		tokenLength = token.length();

		cout << "Consider token ("<< ruleNum <<"): " << token << endl;

		if(it_ii == tokenList.begin()){
			// this is the first token in the list, must be valid NT
			if(gramSym != GS_NONE){
				PrintError(0);
			}else if(termSym != TS_NONE){
				PrintError(2);
			}else{
				validNT = isValidNonTerm(token);
				if(validNT){
					AddNonTermRule(token);
				}
			}
			continue;
		}

//		The FIRST set of a terminal symbol is the symbol itself.
//		The FIRST set of an empty alternative is the empty set. The empty set is indicated by e and is considered an actual element of the FIRST set (So, a FIRST set could contain two elements: '+' and e).
//		If X has a production rule X: X1 X2 X3..., Xi, ...Xn, then initialize FIRST(X) to empty (i.e., not even holding e). Then, for each Xi (1..n):
//		add FIRST(Xi) to FIRST(X)
//		stop when FIRST(Xi) does not contain e
//		If FIRST(Xn) does not contain e remove e from FIRST(X) (unless analyzing another production rule) e is already part of FIRST(X).

		if(gramSym != GS_NONE){	// reserved symbol
			// make sure token is not a reserved symbol ( []{}- )
			// need some logic here
			cout << "Grammar found:\t\'" << gramSym <<"\', \'"<<  token << "\' "<< endl;
			if(gramSym == GS_LBRACEOPT || gramSym == GS_OR){
				// optional part, include the next token's FIRST set in this token's FIRST set
				// include the FIRST() from the symbol inside the Lbrace, then skip past Rbrace
				// if length > 1, there's something else hiding in this token
				if(tokenLength == 1){// the next token should be added to this FIRST()

				}else if(tokenLength> 1){
//					there's something else hiding inside the token, strip off the first char and
				}
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
					AddNonTermRule(token);
				}

			}

		}

	}
}

void AddNonTermRule(string name){
	cout << "NonTerminal found:\t" << name << endl;
	if(ruleMap.find(name) == ruleMap.end()){
		// rule is not yet present in the map, add it
		ruleMap[token] = NonTerminal(token);
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
#ifdef NOTNECESSARY
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
