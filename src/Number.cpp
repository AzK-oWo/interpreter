#include "Number.hpp"
#include <iostream>

int Number::get_value() {
	return value;
}

void Number::print() {
		std::cout << '[' << value << "] ";
}