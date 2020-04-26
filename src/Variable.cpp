#include <string>
#include <map>
#include <iostream>
#include "Variable.hpp"

std::string Variable::get_name() {
	return vName;
}

void Variable::set_value(int value) {
	vtable[vName] = value;
}

int Variable::get_value() {
	return vtable[vName];
}
void Variable::print() {
	std::cout << '[' << vName << "] ";
}

std::map<std::string,int> Variable::vtable;