/*******************************************************************************
 * FILENAME:	NonTeminal.hpp
 * DESCRIPTION:	Header file for NonTerminal Class
 * AUTHOR:		James Matthew Welch [JMW]
 * SCHOOL:		Arizona State University
 * CLASS:		CSE340: Principles of Programming Languages
 * INSTRUCTOR:	Dr. Toni Farley
 * SECTION:		27199
 * TERM:		Spring 2012
 ******************************************************************************/

#ifndef __NONTERMINAL_HPP__
#define __NONTERMINAL_HPP__

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
#include <map>
#include <stack>
#include <queue>
using namespace std;


/* ----------------------------- token types ------------------------------ */
typedef enum e_TermSymbols {
        // Terminal symbols:
	TS_NONE,
	TS_VAR, TS_BEGIN, TS_END, TS_ASSIGN, TS_IF, TS_WHILE, TS_DO, TS_THEN, TS_PRINT, TS_INT, TS_REAL, TS_STRING,
	TS_PLUS, TS_MINUS, TS_UNDERSCORE, TS_DIV, TS_MULT, TS_EQUAL, TS_COLON, TS_COMMA, TS_SEMICOLON,
	TS_LBRAC, TS_RBRAC, TS_LPAREN, TS_RPAREN, TS_NOTEQUAL, TS_GREATER, TS_LESS, TS_LTEQ, TS_GTEQ, TS_DOT,
	TS_ID, TS_NUM, TS_REALNUM, TS_EOF
}TermSymbolType;

typedef enum e_GrammarSymbols {
	GS_NONE, GS_LBRACKET, GS_RBRACKET, GS_LBRACE, GS_RBRACE, GS_DASH, GS_OR, GS_EOF
}GramSymbolType;

const int g_NUM_TERMINALS = 36;

const string TermStrings[36] = {
		"NONE", "VAR", "BEGIN", "END", "ASSIGN", "IF", "WHILE", "DO", "THEN", "PRINT",
		"INT",  "REAL", "STRING", "PLUS", "MINUS", "UNDERSCORE", "DIV", "MULT", "EQUAL", "COLON",
		"COMMA", "SEMICOLON", "LBRAC", "RBRAC", "LPAREN", "RPAREN", "NOTEQUAL", "GREATER", "LESS", "LTEQ",
		"GTEQ", "DOT", "ID", "NUM", "REALNUM", "$"};

const TermSymbolType TermSymbols[36] = {
		TS_NONE, TS_VAR, TS_BEGIN, TS_END, TS_ASSIGN, TS_IF, TS_WHILE, TS_DO, TS_THEN, TS_PRINT,
		TS_INT, TS_REAL, TS_STRING, TS_PLUS, TS_MINUS, TS_UNDERSCORE, TS_DIV, TS_MULT, TS_EQUAL, TS_COLON,
		TS_COMMA, TS_SEMICOLON,	TS_LBRAC, TS_RBRAC, TS_LPAREN, TS_RPAREN, TS_NOTEQUAL, TS_GREATER, TS_LESS,
		TS_LTEQ, TS_GTEQ, TS_DOT, TS_ID, TS_NUM, TS_REALNUM, TS_EOF};

const GramSymbolType GrammarSymbols[8] = {
		GS_NONE, GS_LBRACKET, GS_RBRACKET, GS_LBRACE, GS_RBRACE, GS_DASH, GS_OR, GS_EOF
};

class NonTerminal {
public:
	NonTerminal();
	NonTerminal(string name);
	NonTerminal(vector<string> tokenList, int ruleNum, string ruleString);
	virtual ~NonTerminal();
	void SetRuleNum(int num);
	int  GetRuleNum();
	void SetComplete(bool complete);
	bool GetComplete();
	void AddToFirst(TermSymbolType newFirst);
	void AddNTtoFirst(string token);
	void AddFirstToFollow(NonTerminal other);
	void AddToFollow(TermSymbolType newFollow);
	void AddNTtoFollow(string token);
	void PrintRule();
	void PrintFirstSet();
	void PrintFirstNTs();
	void PrintFollowNTs();
	void PrintFollowSet();
	void SetStartSymbol();
	bool IsStartSymbol();
	void ParseNewRule(vector<string> tokens);
	string GetName();
	vector<string> GetFirstNTs();
	vector<string> GetFollowNTs();
	vector<string> GetRule();
	void PrintError(int errCode);
	vector< vector <string> > GetRuleSet();
	bool UnionFirstSets(NonTerminal other);
	bool UnionFollowSets(NonTerminal other);
	void 			ParseTokenList(vector<string> tokenList);
	GramSymbolType 	FindGrammarType(string token);
	TermSymbolType 	FindTermType(string token);
	bool 			isValidNonTerm(string token);

private:
	// properties of the rule:
	bool 					_complete;
	bool					_modified;
	bool					_startSym;
	string 					_name;
	int						_ruleNum;
	// contents of the rule productions
	string					_ruleString;
	vector<string> 			_ruleTokens;
	vector< vector<string> > _ruleSet;
	vector<string>			_termTokens;
	vector<TermSymbolType> 	_firstSet;
	vector<TermSymbolType> 	_followSet;
	vector<string>			_firstNonTerms;
	vector<string>			_followNonTerms;
};


/* end of __NONTERMINAL_HPP__ */
#endif
