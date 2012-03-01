/*******************************************************************************
 * FILENAME:	NonTerminal.cpp
 * DESCRIPTION:	NonTerminal Class method definitions
 * AUTHOR:		James Matthew Welch [JMW]
 * SCHOOL:		Arizona State University
 * CLASS:		CSE340: Principles of Programming Languages
 * INSTRUCTOR:	Dr. Toni Farley
 * SECTION:		27199
 * TERM:		Spring 2012
 ******************************************************************************/

#include "NonTerminal.hpp"


NonTerminal::NonTerminal() {
	// TODO Auto-generated constructor stub

}

NonTerminal::NonTerminal(string name){
	_name = name;
	_complete = 0;

}

NonTerminal::NonTerminal(vector<string> tokenList, int ruleNum, string ruleString){
	_ruleNum = ruleNum;
	_complete = false;
	_ruleTokens = tokenList;
	_ruleString = ruleString;
	// add _ruleTokens to _ruleSet
	_ruleSet.push_back(_ruleTokens);
	ParseTokenList(tokenList);
}

void NonTerminal::ParseTokenList(vector<string> tokenList){
	string token;
	GramSymbolType gramSym;
	TermSymbolType termSym;
	int tokenNum=0;

#ifdef DEBUG
	cout<< ":::ParseTokenList:::" << tokenList.size() << " tokens:: " << endl;
#endif
	// iterator to beginning of vector
	vector<string>::iterator it_ii = tokenList.begin();

	// this is the first token in the list, must be valid NT
	token = *it_ii;
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
			if(_name.compare(token) == 0){
				// this is a new rule for an existing NT
			}else{
				_name = token;
			}
		}else{
			PrintError(0);
		}
	}

	// increment to point at 2nd token (should be a GS_DASH)
	it_ii++; tokenNum++;
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
	it_ii++; tokenNum++;
	bool complete = false;
	bool newRule = true;
	bool bracketOpen = false;
	int firstCount = 0;
	while(it_ii != tokenList.end() && !complete){
		token = *it_ii;
		gramSym = FindGrammarType(token);
		termSym = FindTermType(token);

		// analyze tokens
		// this logic is ugly because it's approximating a functional paradigm
		if(gramSym == GS_LBRACKET){
			if(firstCount == 0){
				newRule = true;
			}else{
				newRule = false;
				complete = true;
			}
			bracketOpen = true;
		}else if(gramSym == GS_OR){
			if(firstCount == 0) PrintError(0);
			newRule = true;
		}else if(gramSym == GS_RBRACKET){
			// shouldn't see a '}' before anything in First()
			if(bracketOpen != true){
				PrintError(0);
			}
			bracketOpen = false;
			newRule = true;
		}else if(gramSym == GS_LBRACE){
			if(_firstSet.size() > 0){
			// done, no more tokens in first set
			bracketOpen = true;
			complete = true;
			continue;
			}
		}

		if(newRule == true){
			if(termSym != TS_NONE){
				_termTokens.push_back(token);
				AddToFirst(termSym);
				firstCount++;
				newRule = false;
			}else if(isValidNonTerm(token)){
				// found a NT, add to FIRST()
				_firstNonTerms.push_back(token);
				firstCount++;
				newRule = false;
			}
		}

		// increment iterator to point to next token
		it_ii++; tokenNum++;
	}
	// TODO: these calls should be inside main() at the end, when it's done
#ifdef DEBUG
	PrintFirstSet();
	PrintFirstNTs();
#endif
}

NonTerminal::~NonTerminal() {
	// TODO Auto-generated destructor stub
}

bool NonTerminal::UnionFirstSets( NonTerminal  other){
	_modified = false;
	for(vector<TermSymbolType>::iterator it = other._firstSet.begin();
		it != 	other._firstSet.end();
		++it){
		this->AddToFirst(*it);
	}
	return _modified;
}

bool NonTerminal::UnionFollowSets( NonTerminal  other){
	_modified = false;
	for(vector<TermSymbolType>::iterator it = other._followSet.begin();
		it != 	other._followSet.end();
		++it){
		this->AddToFollow(*it);
	}
	PrintFollowSet();
	return _modified;
}

void NonTerminal::AddToFirst(TermSymbolType newFirst){
	// make sure it's not already in the first set
	for(vector<TermSymbolType>::iterator it = _firstSet.begin();
			it != _firstSet.end();
			++it){
		if(*it == newFirst) return;
	}
	// add to first set if not present
	_modified = true;
	_firstSet.push_back(newFirst);
}

void NonTerminal::AddToFollow(TermSymbolType newFollow){
	// make sure it's not already in the follow set
		for(vector<TermSymbolType>::iterator it = _followSet.begin();
				it != _followSet.end();
				++it){
			if(*it == newFollow) return;
		}
		// add to first set if not present
		_modified = true;
		_followSet.push_back(newFollow);
#ifdef DEBUG
		cout<< "Symbol added to FOLLOW: " << newFollow << endl;
#endif
}

void NonTerminal::AddNTtoFollow(string newFollowNT){
	for(vector<string>::iterator it = _followNonTerms.begin();
			it != _followNonTerms.end();
			++it){
		if(this->FindGrammarType(newFollowNT) != GS_NONE){
			// stop here, there' something wrong
#ifdef DEBUG
			cout << "THIS IS NOT A GRAMMAR SYMBOL(" << newFollowNT <<")"<<endl;
#endif
			exit(1);
		}
		if(newFollowNT.compare(*it) == 0) return;
	}
	_modified = true;
	_followNonTerms.push_back(newFollowNT);
}

void NonTerminal::AddFirstToFollow(NonTerminal other){
	_modified = false;
	for(vector<TermSymbolType>::iterator it = other._firstSet.begin();
		it != 	other._firstSet.end();
		++it){
		this->AddToFollow(*it);
	}
}
void NonTerminal::SetStartSymbol(){
	// no start symbol has been set yet, set it to 1
	_startSym = true;
}

bool NonTerminal::IsStartSymbol(){
	return _startSym;
}

void NonTerminal::SetRuleNum(int num){
	_ruleNum = num;
}

int NonTerminal::GetRuleNum(){
	return _ruleNum;
}

vector< vector <string> > NonTerminal::GetRuleSet(){
	return _ruleSet;
}

vector<string> NonTerminal::GetFirstNTs(){
	return _firstNonTerms;
}

vector<string> NonTerminal::GetFollowNTs(){
	return _followNonTerms;
}

vector<string> NonTerminal::GetRule(){
	return _ruleTokens;
}
void NonTerminal::SetComplete(bool complete){
	_complete = complete;
}

bool NonTerminal::GetComplete(){
	return _complete;
}

string NonTerminal::GetName(){
	return _name;
}

GramSymbolType NonTerminal::FindGrammarType(string token)
{
	char c = token.c_str()[0];
	switch(c){
	case '-':
		return GS_DASH;
	case '[':
		return GS_LBRACKET;
	case ']':
		return GS_RBRACKET;
	case '{':
		return GS_LBRACE;
	case '}':
		return GS_RBRACE;
	case '|':
		return GS_OR;
	case '$':
		return GS_EOF;
	default:
		return GS_NONE;
	}
	return GS_NONE;
}

TermSymbolType NonTerminal::FindTermType(string token){
	for(int i = 0; i < g_NUM_TERMINALS; i++){
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

	if(FindGrammarType(token) > 0 || FindTermType(token) > 0)
		return false;

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

void NonTerminal::PrintRule(){
	cout << _ruleString.c_str() << endl;
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

		// use ENUM as index to strings vector
		tempSymbol = TermStrings[*it];

		cout << tempSymbol;
	}
	cout << "}" << endl;
}

void NonTerminal::PrintFirstNTs(){
	cout << "FIRSTnt(" << _name <<") = {";
	string tempSymbol;

	for(vector<string>::iterator it = _firstNonTerms.begin();
			it != _firstNonTerms.end();
			++it){
		if(it != _firstNonTerms.begin()){
			cout << ", ";
		}

		cout << *it;
	}
	cout << "}" << endl;
}

void NonTerminal::PrintFollowSet(){
		cout << "FOLLOW(" << _name <<") = {";
		string tempSymbol;

		for(vector<TermSymbolType>::iterator it = _followSet.begin();
				it != _followSet.end();
				++it){
			if(it != _followSet.begin()){
				cout << ", ";
			}
			// use ENUM as index to strings vector
			tempSymbol = TermStrings[*it];

			cout << tempSymbol;
		}
		cout << "}" << endl;
}

void NonTerminal::PrintFollowNTs(){
	cout << "FOLLOWnt(" << _name <<") = {";
	string tempSymbol;

	for(vector<string>::iterator it = _followNonTerms.begin();
			it != _followNonTerms.end();
			++it){
		if(it != _followNonTerms.begin()){
			cout << ", ";
		}

		cout << *it;
	}
	cout << "}" << endl;
}
