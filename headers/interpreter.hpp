#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <vector>
#include <string>
#include "Lexem.hpp"

extern bool DEBUG;

enum OPERATOR {
	LBRACKET, RBRACKET,
	IF,
	ELSE,
	WHILE, ENDLOOP,
	GOTO, ASSIGN, COLON,
	OR,
	AND,
	BITOR,
	XOR,
	BITAND,
	EQ, NEQ,
	LEQ, LT,
	GEQ, GT,
	SHL, SHR,
	PLUS, MINUS,
	MULT, DIV, MOD
};

extern int PRIORITY[];

extern std::string OPERTEXT[];

Lexem *is_oper(std::string codeline, int &i, int size);

Lexem *is_number(std::string codeline, int &i, int size);

Lexem *is_variable(std::string codeline, int &i, int size);

std::vector<Lexem *> parse_lexem(std::string codeline);

void init_loops(std::vector<std::vector<Lexem *>> &infix);

void init_labels(std::vector<Lexem *> &infix, int row);

std::vector<Lexem *> build_postfix (std::vector<Lexem *> infix);

int evaluate_postfix(std::vector<Lexem *> postfix, int row);

#endif