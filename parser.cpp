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

vector<string> 	tokenize(const string & str, const string & delim);
void 			PrintError(int errCode);

/*----------------------------------------------------------*/
/* Global Variables associated with the next input token*/
#define MAX_TOKEN_LENGTH 100
char token[MAX_TOKEN_LENGTH]; /* token string*/
int tokenLength;


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

	cout << endl << ":::Grammar First & Follow Sets (from \"" << argv[1] << "\"):::" << endl;

	// create local variables
	// queue to hold nonterminals as they're built
	map<string, NonTerminal> ruleNameMap;
	map<int, NonTerminal> ruleNumberMap;
	string strRule;
	string name;
	int numRules = 0;
	int currentRuleNum = 0;
	vector<string> tokenVector;
	const string delim = " ";
	int numTokens = 0;
	NonTerminal *nextNT;

	while (inFilePtr.good()) {
		getline(inFilePtr, strRule);
		if (inFilePtr.good()) { // check inFilePtr.good() twice to ensure no empty lines
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
			// assume first token is a valid NT - it will exit if not
			map<string, NonTerminal>::iterator it = ruleNameMap.find(tokenVector[0]);

			if(it == ruleNameMap.end()){// if no rule exists yet, make one
				nextNT = new NonTerminal(tokenVector,numRules);
			}else{// grab the existing rule from the map
				nextNT = &it->second;
				currentRuleNum = nextNT->GetRuleNum();
				nextNT->ParseTokenList(tokenVector);
			}
			if(numRules == 0){ nextNT->SetStartSymbol(); }

			string ruleName = nextNT->GetName();
			ruleNameMap[ruleName] = *nextNT;
			ruleNumberMap[currentRuleNum] = *nextNT;

#ifdef DEBUG
			cout << "ruleMap contains " << ruleNumberMap.size() << " items." << endl << endl;
#endif
			++numRules;
			currentRuleNum = numRules;
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
		for(map<int,NonTerminal>::iterator thisNT = ruleNumberMap.begin();
				thisNT != ruleNumberMap.end();
				++thisNT)
		{
			vector<string> nonTermsInFirst = thisNT->second.GetFirstNTs();

#ifdef DEBUG
			cout << "reprocessing NT(" << thisNT->second.GetName() << "): mod:" << modCount << endl;
#endif

			// look at the nonTerminals listed in this NonTerminal's _nonTermTokens set
			for(vector<string>::iterator it = nonTermsInFirst.begin();
					it != nonTermsInFirst.end();
					++it){
				// *it is a string token from the list of NonTerminals in the FIRST()
				map <string, NonTerminal>::iterator it_nt= ruleNameMap.find(*it);
				if(it_nt == ruleNameMap.end()){
					// the nonTerminal in question was unfound==>error1
					PrintError(1);
				}else{
					// found a matching rule, add its FIRST() to this NT's FIRST()
					modCount += thisNT->second.UnionFirstSets(it_nt->second);
				}
			}
			if(modCount > 0){
				ruleNameMap[thisNT->second.GetName()] = thisNT->second;
				ruleNumberMap[thisNT->second.GetRuleNum()] = thisNT->second;
			}
		}
	}while(modCount > 0);

	// parse for follow sets
/*===============================================*/
	modCount = 0;
	string token;
//	int lastSymbol = 0; // 0=GS, 1=TS, 2=NT
	do{
		modCount = 0;
		for(map<int,NonTerminal>::iterator thisNT = ruleNumberMap.begin();
				thisNT != ruleNumberMap.end();
				++thisNT)
		{
			NonTerminal tempNT = thisNT->second;

			if(tempNT.IsStartSymbol()){
				tempNT.AddToFollow(TS_EOF);
			}

			vector<string> rule = tempNT.GetRule();

			for(vector<string>::reverse_iterator it = rule.rbegin();
					it != rule.rend();
					++it){
				string token = *it;
				map<string, NonTerminal>::iterator NTptr = ruleNameMap.find(token);

				if(NTptr != ruleNameMap.end()){
					//the token in question is a nonterm in the map
					if( it == rule.rbegin()+1 ){
						// the token in question is also the last one in the
					}
					//the last
				}
			}


#ifdef DEBUG
			cout << "Processing FOLLOW(" << thisNT->second.GetName() << "): mod:" << modCount << endl;
#endif

			// for each token in this NT's rule
			//		if(ruleNameMap.find == ruleMap.end()
			//		if eachNT is followed by another NT, eachNT.AddToFollow(anotherNT.First)
			//		else if eachNT is followed by a term, eachNT.addToFollow(a term)
			//
/*			vector< vector <string> > *ruleSet = thisNT->second.GetRuleSet;
			for(vector< vector<string> >::iterator it_outer = ruleSet.begin();
					it_outer != ruleSet.end();
					++it_outer){
				for(vector<string>::iterator it_inner = it_outer->begin()+2; // start 2 past to skip {NT,-}
						it_inner != it_outer->end();
						++it_inner){
					token = *it_inner;

					// put the "getnexttoken() code into GetxtToken()
					GramSymbolType gramSym = thisNT->second.FindGrammarType(token);
					TermSymbolType termSym = thisNT->second.FindTermType(token);
					map<string, NonTerminal>::iterator it_NT = ruleNameMap.find(token);
					/////

					if(FindGrammarType(token) != GS_NONE){
						// grammar symbol fn, skip it
						lastSymbol = gramSym;
					}
					else if(thisNT->second.FindTermType(token) > 0){
						// a valid terminal has been found
					}

					if( it_NT != ruleNameMap.end()){
						// the token is a
					}
				}
			}*/
		}
#ifdef DEBUG
		modCount = 0;
#endif
	}while(modCount > 0);
	/*===============================================*/

	// print FIRST sets
	for(map<int,NonTerminal>::iterator thisNT = ruleNumberMap.begin();
					thisNT != ruleNumberMap.end();
					++thisNT)
	{
		thisNT->second.PrintFirstSet();
	}

	// print FOLLOW sets
	for(map<int,NonTerminal>::iterator thisNT = ruleNumberMap.begin();
			thisNT != ruleNumberMap.end();
			++thisNT)
	{
		thisNT->second.PrintFollowSet();
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
