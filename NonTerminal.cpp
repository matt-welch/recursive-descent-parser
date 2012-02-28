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

void NonTerminal::PrintFirstSet(){
	cout << "FIRST(" << _name <<") = {";
	string tempSymbol;

	for(vector<TermSymbolType>::iterator it = _firstSet.begin();
			it != _firstSet.end();
			++it){
		if(it != _firstSet.begin()){
			cout << ", ";
		}

//		cout << ::TermStrings[*it] << endl<< endl;

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
		cout << tempSymbol;
	}
	cout << "}" << endl;
}


void NonTerminal::PrintFollowSet(){

}
