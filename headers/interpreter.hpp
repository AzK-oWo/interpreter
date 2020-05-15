#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <vector>
#include <string>
#include <map>
#include "Lexem.hpp"
#include "Number.hpp"
#include "Function.hpp"

extern bool DEBUG;

extern bool INTERACTIVE;

extern int OPER_OR_VAR[];

extern std::string ERRORTEXT[];

Lexem *is_oper(std::string codeline, int &i, int size);

Lexem *is_number(std::string codeline, int &i, int size);

Lexem *is_variable(std::string codeline, int &i, int size);

std::vector<Lexem *> parse_lexem(std::string codeline);

void init_labels(std::vector<Lexem *> &infix, int row);

void init_loops(std::vector<std::vector<Lexem *>> &infix);

void init_functions(std::vector<std::vector<Lexem *>> &infix);

std::vector<Lexem *> build_postfix(std::vector<Lexem *> infix);

int evaluate_postfix(std::vector<std::vector<Lexem *>> &postfix, int row, Function **function_field, Number **res);

#endif