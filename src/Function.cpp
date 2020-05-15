#include <iostream>
#include <string>
#include <map>
#include "Oper.hpp"
#include "Goto.hpp"
#include "Function.hpp"
#include "interpreter.hpp"

Function::Function() {}

Function::Function(std::string name, int row, int n) {
	((Oper *)this) -> set_type(FUNCTION);
	((Goto *)this) -> set_row(row);
	num_of_start_vars = n;
	f_name = name;
}

Function::~Function() {}

int Function::get_num_of_start_vars() {
	return num_of_start_vars;
}

std::string Function::get_name(){
	return f_name;
}

void Function::add_var(std::string var_name) {
	vtable[var_name];
}

void Function::set_start_var(int answer, int i) {
	int j = 0;
	for	(auto &q: vtable) {
		if (j == i) {
			q.second = answer;
		}
		j++;	
	}
}

void Function::print_function_vars() {
	for	(auto &q: vtable) {
		std::cout << q.first << " = " << q.second << std::endl;
	}
}

int Function::check_var(std::string var_name) {
	if (vtable.find(var_name) == vtable.end()) {
		return 1;
	}
	return 0;
}

void Function::set_value(std::string var_name, int value) {
	vtable[var_name] = value;
}

int Function::get_value(std::string var_name) {
	return vtable[var_name];
}

void Function::print() {
	Goto::print();
	std::cout << '@' << f_name << " %" << num_of_start_vars << "%";
	for (auto &var: vtable) {
		std::cout << "_" << var.first;
	}
	std::cout << "@ ";
}

std::map<std::string, int> Function::ftable;