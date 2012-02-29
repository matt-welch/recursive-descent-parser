/*
 * NonTerminal.cpp
 *
 *  Created on: Feb 26, 2012
 *      Author: mwelch
 */

#include "NonTerminal.hpp"


NonTerminal::NonTerminal() {
	// TODO Auto-generated constructor stub

}

NonTerminal::NonTerminal(string name){
	_name = name;
	_complete = 0;

}

NonTerminal::NonTerminal(vector<string> tokenList, int ruleNum){
	_ruleNum = ruleNum;
	_complete = false;

	ParseTokenList(tokenList);
}



void NonTerminal::ParseTokenList(vector<string> tokenList){
	string token;
	GramSymbolType gramSym;
	TermSymbolType termSym;
	int tokenNum = 0;
	_ruleTokens = tokenList;

#ifdef DEBUG
	cout<< ":::ParseTokenList:::" << _ruleTokens.size() << " tokens:: " << endl;
#endif
	// first, look for NonTerm @ vector[0]

	vector<string>::iterator it_ii = _ruleTokens.begin();
	// this is the first token in the list, must be valid NT
	token = *it_ii;
	tokenNum++;
	gramSym = FindGrammarType(token);
	termSym = FindTermType(token);

#ifdef DEBUG
	cout << "Consider token ("<< tokenNum <<"): " << token << endl;
#endif
	if(gramSym != GS_NONE){
		PrintError(0);
	}else if(termSym != TS_NONE){
		PrintError(2);
	}else{
		if(isValidNonTerm(token)){
			_name = token;
		}else{
			PrintError(0);
		}
	}

	// increment to point at 2nd token (should be a GS_DASH)
	it_ii++;
	tokenNum++;
	token = *it_ii;
	gramSym = FindGrammarType(token);
	termSym = FindTermType(token);

#ifdef DEBUG
	cout << "Consider token ("<< tokenNum <<"): " << token << endl;
#endif
	if(gramSym != GS_DASH){
		PrintError(0);
	}
#ifdef DEBUG
	cout << "\tValid rule start, DASH (-) confirmed: " << endl;
#endif

	// point to next token (first term/nonterm in production)
	it_ii++;
	tokenNum++;
	while(it_ii != _ruleTokens.end()){
		token = *it_ii;

		gramSym = FindGrammarType(token);
		termSym = FindTermType(token);

#ifdef DEBUG
		cout << "Consider token ("<< tokenNum <<"): " << token << endl;
#endif
		if(gramSym != GS_NONE){	// reserved symbol
			// make sure token is not a reserved symbol ( []{}- )
			// need some logic here
#ifdef DEBUG
			cout << "Grammar found:\t\'" << gramSym <<"\', \'"<<  token << "\' "<< endl;
#endif
			if(gramSym == GS_LBRACEOPT || gramSym == GS_OR){
				// optional part, include the next token's FIRST set in this token's FIRST set
				// include the FIRST() from the symbol inside the LBrace, then skip past Rbrace

				// add following terminals and terminals after RBrace
/*				it_ii++;
				token = *it_ii;
				gramSym = FindGrammarType(token);
				termSym = FindTermType(token);
				_nonTermTokens.push_back(token);*/

			}else if (gramSym == GS_LBRACKET){
				// indicates repetition, important for FOLLOW, but not FIRST

			}
		}else{
			// not a grammar symbol, look for terminal
			if(termSym != TS_NONE){
				// this is a terminal symbol
				// add the terminal to the first set of NT
				//AddTermToFirst(termSym, _name);
				_termTokens.push_back(token);
				AddToFirst(termSym);
				// this is more subtle, not all terminals get added to first()

			}else {
				// this is a nonterminal, make a new NT object
				if(isValidNonTerm(token)){
					//AddNonTermRule(token);
					_nonTermTokens.push_back(token);

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
	// set _complete flag
	//	_complete = (_nonTermTokens.size() > 0);

		PrintFirstSet();
}

NonTerminal::~NonTerminal() {
	// TODO Auto-generated destructor stub
}

void NonTerminal::AddToFirst(TermSymbolType newFirst){
	_firstSet.push_back(newFirst);
}

void NonTerminal::AddToFollow(TermSymbolType newFollow){
	_followSet.push_back(newFollow);
}

void NonTerminal::SetStartSymbol(){
	// no start symbol has been set yet, set it to 1
	_startSym = 1;
}

void NonTerminal::SetRuleNum(int num){
	_ruleNum = num;
}

int NonTerminal::GetRuleNum(){
	return _ruleNum;
}

void NonTerminal::SetComplete(bool complete){
	_complete = complete;
}

bool NonTerminal::GetComplete(){
	return _complete;
}

GramSymbolType NonTerminal::FindGrammarType(string token)
{
	char c = token.c_str()[0];
	switch(c){
	case '-':
		return GS_DASH;
	case '[':
		return GS_LBRACEOPT;
	case ']':
		return GS_RBRACEOPT;
	case '{':
		return GS_LBRACKET;
	case '}':
		return GS_RBRACKET;
	case '|':
		return GS_OR;
	default:
		return GS_NONE;
	}
	return GS_NONE;
}

TermSymbolType NonTerminal::FindTermType(string token){
	for(int i = 0; i < 35; i++){
		if(token.compare(TermStrings[i]) == 0){
			return TermSymbols[i];
		}
	}
	return TS_NONE;
}

bool NonTerminal::isValidNonTerm(string token){

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

void NonTerminal::PrintError(int errCode){
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


void NonTerminal::PrintFirstSet(){
	cout << "FIRST(" << _name <<") = {";
	string tempSymbol;

	for(vector<TermSymbolType>::iterator it = _firstSet.begin();
			it != _firstSet.end();
			++it){
		if(it != _firstSet.begin()){
			cout << ", ";
		}

		tempSymbol = TermStrings[*it];

#if 0
		switch(*it){
		case TS_VAR:
			tempSymbol = "VAR";
			break;
		case TS_BEGIN:
			tempSymbol = "BEGIN";
			break;
		case TS_END:
			tempSymbol = "END";
			break;
		case TS_ASSIGN:
			tempSymbol = "ASSIGN";
			break;
		case TS_IF:
			tempSymbol = "IF";
			break;
		case TS_WHILE:
			tempSymbol = "WHILE";
			break;
		case TS_DO:
			tempSymbol = "DO";
			break;
		case TS_THEN:
			tempSymbol = "THEN";
			break;
		case TS_PRINT:
			tempSymbol = "PRINT";
			break;
		case TS_INT:
			tempSymbol = "INT";
			break;
		case TS_REAL:
			tempSymbol = "REAL";
			break;
		case TS_STRING:
			tempSymbol = "STRING";
			break;
		case TS_PLUS:
			tempSymbol = "PLUS";
			break;
		case TS_MINUS:
			tempSymbol = "MINUS";
			break;
		case TS_UNDERSCORE:
			tempSymbol = "UNDERSCORE";
			break;
		case TS_DIV:
			tempSymbol = "DIV";
			break;
		case TS_MULT:
			tempSymbol = "MULT";
			break;
		case TS_EQUAL:
			tempSymbol = "EQUAL";
			break;
		case TS_COLON:
			tempSymbol = "COLON";
			break;
		case TS_COMMA:
			tempSymbol = "COMMA";
			break;
		case TS_SEMICOLON:
			tempSymbol = "SEMICOLON";
			break;
		case TS_LBRAC:
			tempSymbol = "LBRAC";
			break;
		case TS_RBRAC:
			tempSymbol = "RBRAC";
			break;
		case TS_LPAREN:
			tempSymbol = "LPAREN";
			break;
		case TS_RPAREN:
			tempSymbol = "RPAREN";
			break;
		case TS_NOTEQUAL:
			tempSymbol = "NOTEQUAL";
			break;
		case TS_GREATER:
			tempSymbol = "GREATER";
			break;
		case TS_LESS:
			tempSymbol = "LESS";
			break;
		case TS_LTEQ:
			tempSymbol = "LTEQ";
			break;
		case TS_GTEQ:
			tempSymbol = "GTEQ";
			break;
		case TS_DOT:
			tempSymbol = "DOT";
			break;
		case TS_ID:
			tempSymbol = "ID";
			break;
		case TS_NUM:
			tempSymbol = "NUM";
			break;
		case TS_REALNUM:
			tempSymbol = "REALNUM";
			break;
		default:
			tempSymbol = "";
		}

#endif
		cout << tempSymbol;
	}
	cout << "}" << endl;
}


void NonTerminal::PrintFollowSet(){

}

void NonTerminal::BuildTerminalSymbolMap(){
	/*string TermStrings[35] = {
				"NONE", "VAR", "BEGIN", "END", "ASSIGN", "IF", "WHILE", "DO", "THEN", "PRINT",
				"INT",  "REAL", "STRING", "PLUS", "MINUS", "UNDERSCORE", "DIV", "MULT", "EQUAL", "COLON",
				"COMMA", "SEMICOLON", "LBRAC", "RBRAC", "LPAREN", "RPAREN", "NOTEQUAL", "GREATER", "LESS", "LTEQ",
				"GTEQ", "DOT", "ID", "NUM", "REALNUM"};

	TermSymbolType TermSymbols[35] = {
				TS_NONE, TS_VAR, TS_BEGIN, TS_END, TS_ASSIGN, TS_IF, TS_WHILE, TS_DO, TS_THEN, TS_PRINT,
				TS_INT, TS_REAL, TS_STRING, TS_PLUS, TS_MINUS, TS_UNDERSCORE, TS_DIV, TS_MULT, TS_EQUAL, TS_COLON,
				TS_COMMA, TS_SEMICOLON,	TS_LBRAC, TS_RBRAC, TS_LPAREN, TS_RPAREN, TS_NOTEQUAL, TS_GREATER, TS_LESS,
				TS_LTEQ, TS_GTEQ, TS_DOT, TS_ID, TS_NUM, TS_REALNUM};*/
}
