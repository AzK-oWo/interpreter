#include <vector>
#include <string>
#include <stack>
#include "interpreter.hpp"
#include "Lexem.hpp"
#include "Number.hpp"
#include "Variable.hpp"
#include "Oper.hpp"
#include "Goto.hpp"

bool DEBUG = false;

int PRIORITY[] = { 
	-1, -1,
	0,
	0,
	0, 0,
	0, 0, 0,
	1,
	2,
	3,
	4,
	5,
	6, 6,
	7, 7,
	7, 7,
	8, 8,
	9, 9,
	10, 10, 10
};

std::string OPERTEXT[] = {
	"(", ")",
	"if",
	"else",
	"while", "@",
	"goto", ":=", ":",
	"or",
	"and",
	"|",
	"^",
	"&",
	"==", "!=",
	"<=", "<",
	">=", ">",
	"<<", ">>",
	"+", "-",
	"*", "/", "%"
};

Lexem *is_oper(std::string codeline, int &i, int size) {
	Lexem *lex = nullptr;
	char tmp = codeline[i];
	while (i < size && (tmp == ' ' || tmp == '\t')) {
		++i;
		tmp = codeline[i];
	}
	int n = sizeof(OPERTEXT) / sizeof(std::string);
	for (int op = 0; op < n; op++) {
		std::string subcodeline = codeline.substr(i, OPERTEXT[op].size());
		if (OPERTEXT[op] == subcodeline) {
			if ((OPERATOR)op == IF || (OPERATOR)op == ELSE ||
				(OPERATOR)op == WHILE || (OPERATOR)op == ENDLOOP) {
				lex = new Goto((OPERATOR)op);
			} else {
				lex = new Oper((OPERATOR)op);
			}
			i += OPERTEXT[op].size();
			break;
		}
	}
	return lex;
}

Lexem *is_number(std::string codeline, int &i, int size) {
	int p = i, number = 0;
	for ( ; p < size && codeline[p] >= '0' && codeline[p] <= '9'; p++) {
		number = number * 10 + codeline[p] - '0';
	}
	if (p == i) {
		return nullptr;
	}
	i = p;
	Lexem *lex = new Number(number);
	return lex;
}

Lexem *is_variable(std::string codeline, int &i, int size) {
	std::string variable;
	while (i < size && ((codeline[i] >= 'A' && codeline[i] <= 'Z') || (codeline[i] >= 'a' && codeline[i] <= 'z')
		|| (codeline[i] >= '0' && codeline[i] <= '9') || codeline[i] == '_' || codeline[i] == '-')) {
		variable += codeline[i];
		i++;
	}
	if (variable.empty()) {
		return nullptr;
	}
	Lexem *lex = new Variable(variable);
	return lex;
}

std::vector<Lexem *> parse_lexem(std::string codeline) {
	codeline = codeline + ' ';
	int size = codeline.size();
	std::vector<Lexem *> infix;
	Lexem *lex;
	int i = 0;
	while (i < size) {
		lex = is_oper(codeline, i, size);
		if (lex != nullptr) {
				infix.push_back(lex);
				continue;
		}
		lex = is_number(codeline, i, size);
		if (lex != nullptr) {
			infix.push_back(lex);
			continue;
		}
		lex = is_variable(codeline, i, size);
		if (lex != nullptr) {
			infix.push_back(lex);
			continue;
		}
	}
	return infix;
}

void init_loops(std::vector<std::vector<Lexem *>> &infix) {
	bool is_if_statement;
	int size = infix.size();
	std::stack<Goto *> loop_stack;
	for (int row = 0; row < size; row++) {
		int length = infix[row].size();
		for (int i = 0; i < length; i++)
			if (Oper *ptr = dynamic_cast<Oper *>(infix [row][i])) {
				Oper *lexemoper = (Oper *)infix[row][i];
			if (lexemoper -> get_type() == IF) {
				if (Oper *ptr = dynamic_cast<Oper *>(infix[row].back())) {
					if ((((Oper *)infix[row].back()) -> get_type() == COLON)) {
						infix[row].pop_back();
					}
				}
				loop_stack.push((Goto *)lexemoper);
				is_if_statement = true;
			}
			if (lexemoper -> get_type() == WHILE) {
				if (Oper *ptr = dynamic_cast<Oper *>(infix[row].back())) {
					if ((((Oper *)infix[row].back()) -> get_type() == COLON)) {
						infix[row].pop_back();
					}
				}
				Goto *lexemgoto = (Goto *)lexemoper;
				lexemgoto -> set_row(row);
				loop_stack.push(lexemgoto);
				is_if_statement = false;
			}
			if (lexemoper -> get_type() == ELSE && is_if_statement) {
				loop_stack.top() -> set_row(row + 1);
				loop_stack.pop();
				loop_stack.push((Goto *)lexemoper);
			}
			if (lexemoper -> get_type() == ENDLOOP) {
				if (!is_if_statement) {
					Goto *lexemgoto = (Goto*)lexemoper;
					lexemgoto -> set_row(loop_stack.top() -> get_row());
				}
				loop_stack.top() -> set_row(row + 1);
				loop_stack.pop();
			}
		}
	}
}

void init_labels(std::vector<Lexem *> &infix, int row) {
	int size = infix.size();
	for (int i = 1; i < size; ++i) {
		if (infix[i - 1] == nullptr || infix[i] == nullptr) {
			continue;
		}
		Variable *ptr1;
		Oper *ptr2;
		if ((ptr1 = dynamic_cast<Variable *>(infix[i - 1])) && (ptr2 = dynamic_cast<Oper *>(infix[i]))) {
			Variable *lexemvar = (Variable *)infix[i - 1];
			Oper *lexemop = (Oper *)infix[i];
			if (lexemop -> get_type() == COLON) {
				Goto::ltable.insert({lexemvar -> get_name(), row});
				Variable::vtable.erase(lexemvar -> get_name()); // r->g
				delete infix[i - 1];
				delete infix[i];
				infix[i - 1] = nullptr;
				infix[i] = nullptr;
				i++;
			}
		}
	}
}

std::vector<Lexem *> build_postfix (std::vector<Lexem *> infix) {
	std::vector<Lexem *> lexemstack;
	std::vector<Oper *> operstack;
	for (auto &elem: infix) {
		if (elem == nullptr) {
			continue;
		}
		if (Number *ptr = dynamic_cast<Number *>(elem)) {
			lexemstack.push_back(elem);
			continue;
		}
		if (Variable *ptr = dynamic_cast<Variable *>(elem)) {
			lexemstack.push_back(elem);
			continue;
		}
		OPERATOR elemtype = ((Oper *)elem) -> get_type();
		if (operstack.empty() || elemtype == LBRACKET) {
			operstack.push_back((Oper *)elem);
			continue;
		}
		if (elemtype == RBRACKET) {
			while ((operstack.back()) -> get_type() != LBRACKET) {
				lexemstack.push_back(operstack.back());
				operstack.pop_back();
			}
			operstack.pop_back();
			continue;
		}
		if (elemtype == ASSIGN && ASSIGN == (operstack.back()) -> get_type()) {
			operstack.push_back((Oper *)elem);
			continue;
		}
		if (((Oper *)elem) -> get_priority() <= (operstack.back()) -> get_priority()) {
			while (!(operstack.empty()) && ((operstack.back()) -> get_priority() > 0)) {
				lexemstack.push_back(operstack.back());
				operstack.pop_back();
			}
			operstack.push_back((Oper *)elem);
		} else {
			operstack.push_back((Oper *)elem);
		}
	}
	while (!operstack.empty()) {
		lexemstack.push_back(operstack.back());
		operstack.pop_back();
	}
	return lexemstack;
}

int evaluate_postfix(std::vector<Lexem *> postfix, int row) {
	std::vector<Lexem *> stack;
	std::vector<Number *> need_to_clear;
	Lexem *res;
	for (auto &elem: postfix) {
		if (Number *ptr = dynamic_cast<Number *>(elem)) {
			stack.push_back(elem);
			continue;
		}
		if (Variable *ptr = dynamic_cast<Variable *>(elem)) {
			stack.push_back(elem);
		} else {
			if (((Oper *)elem) -> get_type() == GOTO) {
				Variable *label = (Variable *)stack.back();
				stack.pop_back();
				return Goto::ltable[label -> get_name()];
			}
			if (((Oper *)elem) -> get_type() == ELSE || ((Oper *)elem) -> get_type() == ENDLOOP) {
				return ((Goto *)elem) -> get_row();
			}
			if (((Oper *)elem) -> get_type() == IF || ((Oper *)elem) -> get_type() == WHILE) {
				Number *jump = (Number *)stack.back();
				if (!(jump -> get_value())) {
					return ((Goto *)elem) -> get_row();
				}
				stack.pop_back();
				for (auto &cl: need_to_clear) {
					delete cl;
				}
				return row + 1;
			}
			res = stack.back();
			stack.pop_back();
			res = new Number(((Oper *)elem) -> get_value(stack.back(), res));
			stack.pop_back();
			stack.push_back(res);
			need_to_clear.push_back((Number *)res);
		}
	}
//	res = stack.back();
	stack.pop_back();
	for (auto &cl: need_to_clear) {
		delete cl;
	}
	return row + 1;
}

// ./interpreter filename.txt
// ./interpreter filename.txt --verbose
// ./interpreter --verbose