
#ifndef __GRAMMARRULE_HPP__
#define __GRAMMARRULE_HPP__

#include <map>

class GrammarRule{
public:
	GrammarRule(string name): _name(name)
	{

	}

	GrammarRule()
	{

	}

	virtual ~GrammarRule(){

	}

	void SetRawRule(string rule){
		_rawRule = rule;}

	string GetRawRule(){
		return _rawRule;}

	void SetName(string name){
		_name = name;
	}
	string GetName(){
		return _name;
	}

	void ShowFirst();
	void ShowFollow();

	void FindFirstSet();
	void FindFollowSet();

private:
	string _name;
	string _firstSet;
	string _followSet;
	string _rawRule;
    //map<const char*, string> ruleset;

};

#endif /*  __GRAMMARRULE_HPP__ inclusion guard */
