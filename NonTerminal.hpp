/*
 * NonTerminal.hpp
 *
 *  Created on: Feb 26, 2012
 *      Author: mwelch
 */

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
using namespace std;


/* ----------------------------- token types ------------------------------ */
typedef enum e_TermSymbols {
        // Terminal symbols:
	TS_NONE,
	TS_VAR, TS_BEGIN, TS_END, TS_ASSIGN, TS_IF, TS_WHILE, TS_DO, TS_THEN, TS_PRINT, TS_INT, TS_REAL, TS_STRING,
	TS_PLUS, TS_MINUS, TS_UNDERSCORE, TS_DIV, TS_MULT, TS_EQUAL, TS_COLON, TS_COMMA, TS_SEMICOLON,
	TS_LBRAC, TS_RBRAC, TS_LPAREN, TS_RPAREN, TS_NOTEQUAL, TS_GREATER, TS_LESS, TS_LTEQ, TS_GTEQ, TS_DOT,
	TS_ID, TS_NUM, TS_REALNUM
}TermSymbolType;

typedef enum e_GrammarSymbols {
	GS_NONE, GS_LBRACEOPT, GS_RBRACEOPT, GS_LBRACKET, GS_RBRACKET, GS_DASH, GS_OR, GS_EOF
}GramSymbolType;

extern map <TermSymbolType, string> symbolMap; // private data member of grammar - need bidirectional lookup

//build TermStrings array for enum-lookup - TermStrings[TS_TYPE] = TS_string
const string TermStrings[] = {"NONE", "VAR", "BEGIN", "END", "ASSIGN", "IF", "WHILE", "DO", "THEN", "PRINT", "INT", "REAL", "STRING",
		"PLUS", "MINUS", "UNDERSCORE", "DIV", "MULT", "EQUAL", "COLON", "COMMA", "SEMICOLON",
		"LBRAC", "RBRAC", "LPAREN", "RPAREN", "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ", "DOT",
		"ID", "NUM", "REALNUM"};


class NonTerminal {
public:
	NonTerminal();
	NonTerminal(string name);
	virtual ~NonTerminal();
	void AddToFirst(TermSymbolType newFirst);
	void AddNTtoFirst(NonTerminal *newFirst);
	void AddToFollow(TermSymbolType newFollow);
	void PrintFirstSet();
	void PrintFollowSet();
	void SetStartSymbol();
	void SetRuleNum(int num);
	int  GetRuleNum();
	void SetComplete(bool complete);
	bool GetComplete();
	map <TermSymbolType, string> symbolMap;
//	const static string TermStrings[];
private:
	string 					_name;

	int						_ruleNum;
	// one rule per each optional nonterm production (separated by GS_OR)
	vector<string> 			_rules;
	vector<TermSymbolType> 	_firstSet;
	vector<TermSymbolType> 	_followSet;
	vector<NonTerminal*>	_firstNonTerms;
	bool 					_complete;
	bool					_startSym;

};


/* end of __NONTERMINAL_HPP__ */
#endif
