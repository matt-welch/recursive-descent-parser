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
				nextNT = new NonTerminal(tokenVector,numRules, strRule);
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

#ifdef DEBUG
			cout << "reprocessing NT(" << thisNT->second.GetName() << "): mod:" << modCount << endl;
#endif

			vector<string> nonTermsInFirst = thisNT->second.GetFirstNTs();

			// look at the nonTerminals listed in this NonTerminal's _firstNonTerms set
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
			vector<string> rule = tempNT.GetRule();
			vector<string> tokens;

			const string ruleName = tempNT.GetName();
			const int ruleIDnum = tempNT.GetRuleNum();


			// fill arrays with symbol types for use in rules below
			int numTokens = rule.size() - 2; // adjust size for {NT,-}
			GramSymbolType 	gsTypes[numTokens];
			TermSymbolType 	tsTypes[numTokens];
			bool 			isValidNT[numTokens];


#ifdef DEBUG
			cout << "Processing FOLLOW(" << ruleName << ", "<< ruleIDnum <<"), numTokens("<< numTokens <<"): mod:" << modCount << endl;
#endif

			int i=0;
			for(vector<string>::iterator it = rule.begin();
				it != rule.end();
				++it){
				if(i>=2){ // start looking at 3rd token (i=2)
					string token = *it;
					tokens.push_back(token);
					gsTypes[i] = tempNT.FindGrammarType(token);
					tsTypes[i] = tempNT.FindTermType(token);
#ifdef DEBUG
					cout << TermStrings[tsTypes[i]] << " ";
#endif
					if(gsTypes[i] == GS_NONE){
						if( tsTypes[i] == TS_NONE){
//							isValidNT[i] = tempNT.isValidNonTerm(token);
							map<string,NonTerminal>::iterator it_inner = ruleNameMap.find(token);
							if(it_inner != ruleNameMap.end()){
								//token matches a rule in the map,
								isValidNT[i] = true;
							}else{
								isValidNT[i] = true;
							}
						}
					}
					else
						isValidNT[i] = false;
				}
				++i;
			}
#ifdef DEBUG
			cout << " numTokens in vector: " << tokens.size() << endl;
#endif
			// begin processing ways to the follow sets
			if(tempNT.IsStartSymbol()){
				tempNT.AddToFollow(TS_EOF);
				++modCount;
			}

			for(int i = 0; i<numTokens; i++){ //up to numTokens-1 so at least one token following this one
				string ntName = tokens[i];

				if(tempNT.isValidNonTerm(ntName) ){ // this is an NT, so it has a follow set
					// get an iterator to the NonTerminal in the map, matching the ntName in question

#ifdef DEBUG
					cout << "Consider NonTerminal:::" << ntName << endl;

#endif
					map<string, NonTerminal>::iterator it = ruleNameMap.find(ntName);

					if(it != ruleNameMap.end()){ // check to make sure iterator isn't null

						if(i < numTokens-4){ // NT,{,term,} or NT,[,term,] or NT,[,NT,]
							if(gsTypes[i+1] == GS_LBRACE){// followed by a repeating segment
								if(tsTypes[i+2] > TS_NONE){ // term inside repeat
									it->second.AddToFollow(tsTypes[i+2]);
									++modCount;
								}else if(isValidNT[i+2]){// NT inside repeat
									it->second.AddFirstToFollow(tokens[i+2]);
									++modCount;
								}
							}else if(gsTypes[i+1] == GS_LBRACKET){// followed by an optional segment
								// check inside the optional segment
								if(tsTypes[i+2] > TS_NONE){// terminal inside brackets
									it->second.AddToFollow(tsTypes[i+2]);
									++modCount;
								}else if(isValidNT[i+2]){//
									it->second.AddFirstToFollow(tokens[i+2]);
									++modCount;
								}
								for(int jj = i+3; jj<numTokens-1; ++jj){
									if(gsTypes[jj] == GS_RBRACKET){// found the closing bracket
										if(tsTypes[jj+1] > TS_NONE){// a terminal symbol follows the ]
											it->second.AddToFollow(tsTypes[jj+1]);
											++modCount;
										}else if(isValidNT[jj+1]){// a NT follows the ]
											it->second.AddFirstToFollow(tokens[jj+1]);
											++modCount;
										}
									}
								}
							}
						}else if (i < numTokens-1){
							// not enough tokens left to make a x[y] or x{y} construct
							// but at least one more, check it:
#ifdef DEBUG
							cout << "===> " << it->second.GetName() << endl;
							cout << "token(" << i-1 << ")===> " << TermStrings[tsTypes[i-1]] << endl;
							cout << "token(" << i << ")===> " << TermStrings[tsTypes[i]] << endl;
							cout << "token(" << i+1 << ")===> " << TermStrings[tsTypes[i+1]] << endl;
							cout << "token(" << i+2 << ")===> " << TermStrings[tsTypes[i+2]] << endl;
							cout << "token(" << i+3 << ")===> " << TermStrings[tsTypes[i+3]] << endl;
#endif
							if(tsTypes[i+1] > TS_NONE){// a terminal symbol follows this NT
								it->second.AddToFollow(tsTypes[i+1]);
								++modCount;
							}else if(isValidNT[i+1]){
								it->second.AddFirstToFollow(tokens[i+1]);
								++modCount;
							}
						}

						int otherRuleNum = it->second.GetRuleNum();
						ruleNumberMap[otherRuleNum] = it->second;
					}

				}
			}

/*===========================================================================*/
			// TODO START HERE  cycle through NTfollowers, pulling in follow sets
			vector<string> nonTermsInFollow = tempNT.GetFollowNTs();

#ifdef DEBUG
			tempNT.PrintFollowNTs();
#endif
			// look at the nonTerminals listed in this NonTerminal's _followNonTerms set
			for(vector<string>::iterator it = nonTermsInFollow.begin();
					it != nonTermsInFollow.end();
					++it){
				// *it is a string ntName from the list of NonTerminals in the FIRST()
				map <string, NonTerminal>::iterator it_nt= ruleNameMap.find(*it);
				if(it_nt == ruleNameMap.end()){
					// the nonTerminal in question was unfound==>error1
#ifdef DEBUG
					cout << "Bad TOKEN::: (" << *it << ")" << endl;
#endif
					PrintError(1);
				}else{
					// found a matching rule, add its FIRST() to this NT's FIRST()
					modCount += tempNT.UnionFollowSets(it_nt->second);
				}
			}


/*===========================================================================*/

			if(modCount > 0){
				ruleNameMap[ruleName] = tempNT;
				ruleNumberMap[ruleIDnum] = tempNT;
			}
		}
		modCount = 0;
#ifdef DEBUG
		cout << ":::FIRST & FOLLOW:::" << endl;
#endif
	}while(modCount > 0);//modCount > 0

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

// begin local functions
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
#ifdef DEBUGISNOTENOUGH
			cout << "tokenizer()::" << token << endl;
#endif
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
	cout << endl << endl << "Main() ERROR CODE " << errCode << " :: " << message << endl << endl;
	exit(1);
}
