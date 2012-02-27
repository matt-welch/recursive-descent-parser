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



class NonTerminal {
public:
	NonTerminal();
	NonTerminal(string name);
	virtual ~NonTerminal();
private:
	string 					_name;

	// one rule per each optional nonterm production (separated by GS_OR)
	vector<string> 			_rules;
	vector<TermSymbolType> 	_firstSet;
	vector<TermSymbolType> 	_followSet;


	bool 		_complete;
};



/* end of __NONTERMINAL_HPP__ */
#endif
