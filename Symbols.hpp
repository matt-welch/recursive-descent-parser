/*
 * Symbols.hpp
 *
 *  Created on: Feb 24, 2012
 *      Author: mwelch
 */

#ifndef __SYMBOLS_HPP__
#define __SYMBOLS_HPP__

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
	GS_NONE, GS_LBRACEOPT, GS_RBRACEOPT, GS_LBRACKET, GS_RBRACKET, GS_DASH, GS_OR
}GramSymbolType;

class Symbol{
public:
	Symbol(){

	}
	virtual ~Symbol(){

	}

protected:
	string _realName;
};

class GrammarSym : public Symbol{
	GramSymbolType _name;
};

class TerminalSym : public Symbol{
	TermSymbolType _name;
};

class NonTermSym : public Symbol{
public:
	NonTermSym(){

	}
	NonTermSym(string name){
		_name = name;
		_complete = 0;
	}
	virtual ~NonTermSym() {

	}
private:
	string 		_name;
	TerminalSym _firstSet[10];
	TerminalSym _followSet[10];
	bool 		_complete;
};


#if 1
/*------------------- reserved words and token strings -----------------------*/
string reserved[] = { "", "VAR", "BEGIN", "END", "ASSIGN", "IF", "WHILE", "DO",
		"THEN", "PRINT", "INT", "REAL", "STRING", "+", "-", "_", "/", "*", "=",
		":", ",", ";", "[", "]", "(", ")", "<>", ">", "<", "<=", ">=", "<<",
		">>", ".", "ID", "NUM", "REALNUM", "ERROR" };

#endif

#define KEYWORDS 12
#define RESERVED 36
#define VAR 1
#define BEGIN 2
#define END 3
#define ASSIGN 4
#define IF 5
#define WHILE 6
#define DO 7
#define THEN 8
#define PRINT 9
#define INT 10
#define REAL 11
#define STRING 12
#define PLUS 13
#define MINUS 14
#define UNDERSCORE 15
#define DIV 16
#define MULT 17
#define EQUAL 18
#define COLON 19
#define COMMA 20
#define SEMICOLON 21
#define LBRAC 22
#define RBRAC 23
#define LPAREN 24
#define RPAREN 25
#define NOTEQUAL 26
#define GREATER 27
#define LESS 28
#define LTEQ 29
#define GTEQ 30
#define LSHIFT 31
#define RSHIFT 32
#define DOT 33
#define ID 34
#define NUM 35
#define REALNUM 36
#define NONTERMINAL 37

#define ERROR 39


#endif /* __SYMBOLS_HPP__*/
