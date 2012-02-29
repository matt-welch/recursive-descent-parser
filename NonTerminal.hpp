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
#include <queue>
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
	GS_NONE, GS_LBRACKET, GS_RBRACKET, GS_LBRACE, GS_RBRACE, GS_DASH, GS_OR, GS_EOF
}GramSymbolType;

const string TermStrings[35] = {
		"NONE", "VAR", "BEGIN", "END", "ASSIGN", "IF", "WHILE", "DO", "THEN", "PRINT",
		"INT",  "REAL", "STRING", "PLUS", "MINUS", "UNDERSCORE", "DIV", "MULT", "EQUAL", "COLON",
		"COMMA", "SEMICOLON", "LBRAC", "RBRAC", "LPAREN", "RPAREN", "NOTEQUAL", "GREATER", "LESS", "LTEQ",
		"GTEQ", "DOT", "ID", "NUM", "REALNUM"};

const TermSymbolType TermSymbols[35] = {
		TS_NONE, TS_VAR, TS_BEGIN, TS_END, TS_ASSIGN, TS_IF, TS_WHILE, TS_DO, TS_THEN, TS_PRINT,
		TS_INT, TS_REAL, TS_STRING, TS_PLUS, TS_MINUS, TS_UNDERSCORE, TS_DIV, TS_MULT, TS_EQUAL, TS_COLON,
		TS_COMMA, TS_SEMICOLON,	TS_LBRAC, TS_RBRAC, TS_LPAREN, TS_RPAREN, TS_NOTEQUAL, TS_GREATER, TS_LESS,
		TS_LTEQ, TS_GTEQ, TS_DOT, TS_ID, TS_NUM, TS_REALNUM};

const GramSymbolType GrammarSymbols[8] = {
		GS_NONE, GS_LBRACKET, GS_RBRACKET, GS_LBRACE, GS_RBRACE, GS_DASH, GS_OR, GS_EOF
};

class NonTerminal {
public:
	NonTerminal();
	NonTerminal(string name);
	NonTerminal(vector<string> tokenList, int ruleNum);
	virtual ~NonTerminal();
	void AddToFirst(TermSymbolType newFirst);
	void AddNTtoFirst(NonTerminal *newFirst);
	void AddToFollow(TermSymbolType newFollow);
	void PrintFirstSet();
	void PrintFirstNTs();
	void PrintFollowSet();
	void SetStartSymbol();
	void SetRuleNum(int num);
	int  GetRuleNum();
	void SetComplete(bool complete);
	bool GetComplete();
	string GetName();
	vector<string> GetFirstNTs();
	void PrintError(int errCode);
	void UnionFirstSets(NonTerminal other);
//	vector<string> tokenize(const string & str, const string & delim);
/*
	static 	map <string, TermSymbolType> t*ermSymbolMap; // private data member of Grammar
	static 	map <char, GramSymbolType> grammarSymbolMap; //private data member of Grammar
	static string termStrings[];
*/

//	const static string TermStrings[];
private:

	GramSymbolType 	FindGrammarType(string token);
	TermSymbolType 	FindTermType(string token);
	bool 			isValidNonTerm(string token);
	void 			ParseTokenList();
	void			BuildTerminalSymbolMap();

	string 					_name;
	int						_ruleNum;
	vector<string> 			_ruleTokens;
	vector<string>			_nonTermTokens;
	vector<string>			_termTokens;
	vector<TermSymbolType> 	_firstSet;
	vector<TermSymbolType> 	_followSet;
	bool 					_complete;
	bool					_startSym;
};


/* end of __NONTERMINAL_HPP__ */
#endif
