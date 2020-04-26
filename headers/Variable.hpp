#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <string>
#include <map>
#include "Lexem.hpp"

class Variable: public Lexem {
	std::string vName;
  public:
	Variable () {};
	Variable (std::string str) { vName = str; vtable[vName]; };
	~Variable () {};
	std::string get_name();
	int get_value();
	void set_value(int value);
	void print();
	static std::map<std::string,int> vtable;
};

#endif