#include <iostream>
#include <string>
#include <string>
#include "Goto.hpp"
#include "Oper.hpp"

std::map<std::string, int> Goto::ltable;

void Goto::set_row(int row) {
	Goto::row = row;
}

int Goto::get_row() {
	return row;
}

void Goto::print() {
	std::cout << "<row " << row << '>';
	Oper::print(); 
}