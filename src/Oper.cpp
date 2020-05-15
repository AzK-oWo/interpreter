#include <iostream>
#include <string>
#include "Lexem.hpp"
#include "Oper.hpp"
#include "Number.hpp"
#include "Variable.hpp"
#include "enum.hpp"

Oper::Oper() {}

Oper::Oper(OPERATOR opertype) {
	Oper::opertype = opertype;
}

Oper::~Oper() {}

void Oper::set_type(OPERATOR optype) {
	opertype = optype;
}

OPERATOR Oper::get_type() {
	return opertype;
};

int Oper::get_priority() {
	return PRIORITY[(int)opertype];
}

/*int Oper::get_value(Lexem *leftlex, Lexem *rightlex, Function *function) {
	int left, right;
	if (Number *ptr = dynamic_cast<Number *>(leftlex)) {
		left = ((Number *)leftlex) -> get_value();
	}
	if (Number *ptr = dynamic_cast<Number *>(rightlex)) {
		right = ((Number *)rightlex) -> get_value();
	}
	if (Variable *ptr = dynamic_cast<Variable *>(leftlex)) {
		left = ((Variable *)leftlex) -> get_value(function);
	}
	if (Variable *ptr = dynamic_cast<Variable *>(rightlex)) {
		right = ((Variable *)rightlex) -> get_value(function);
	}
	switch (Oper::opertype) {
			case ASSIGN:
				if (Variable *ptr = dynamic_cast<Variable *>(leftlex)) {
					int tmp;
					if (Number *ptr = dynamic_cast<Number *>(rightlex)) {
						tmp = ((Number *)rightlex) -> get_value();
					} else {
						tmp = ((Variable *)rightlex) -> get_value(function);
					}
					((Variable *)leftlex) -> set_value(function, tmp);
					return tmp;
				} else {
					throw(ERR_ASSIGN_PROBLEMS);
				}
			case PLUS:
				return left + right;
			case MINUS:
				return left - right;
			case MULT:
				return left * right;
			case DIV:
				return left / right;
			case MOD:
				return left % right;
			case OR:
				return left || right;
			case AND:
				return left && right;
			case BITOR:
				return left | right;
			case XOR:
				return left ^ right;
			case BITAND:
				return left & right;
			case EQ:
				return left == right;
			case NEQ:
				return left != right;
			case LEQ:
				return left <= right;
			case LT:
				return left < right;
			case GEQ:
				return left >= right;
			case GT:
				return left > right;
			case SHL:
				return left << right;
			case SHR:
				return left >> right;
	}
	return 0;
}*/

void Oper::print() {
	std::cout << '|' << OPERTEXT[(int)opertype] << "| ";
}