#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "interpreter.hpp"
#include "Number.hpp"
#include "Variable.hpp"
#include "Goto.hpp"

int main(int argc, char** argv) {
	for (int i = 1; i < argc; i++)
		if (strcmp(argv[i], "--verbose") == 0)
			DEBUG = true;
	// for (i = 1; i < argc; i++)
	//     if (string(argv[i]).substr(???) == ".txt") {
	//          INTERACTIV = 0;
	//     }
	
	std::string codeline;
	std::vector<std::vector<Lexem *>> infixlines, postfixlines;
//	int value;
	while (getline(std::cin, codeline)) {
		infixlines.push_back(parse_lexem(codeline));
	}
	init_loops(infixlines);
	for (int row = 0; row < infixlines.size(); ++row) {
		init_labels(infixlines[row], row);
	}
	if (DEBUG) {
		for (int i = 0; i < infixlines.size(); ++i) {
			std::cout << std::endl << "size of infix = " << infixlines[i].size() <<std::endl;
			for (auto &debug: infixlines[i]) {
				if (debug == nullptr) {
					continue;
				}
				if (Number *ptr = dynamic_cast<Number *>(debug)) {
					((Number *)debug) -> print();
					continue;
				}
				if (Variable *ptr = dynamic_cast<Variable *>(debug)) {
					((Variable *)debug) -> print();
					continue;
				} 
				if (Goto *ptr = dynamic_cast<Goto *>(debug)) {
					((Goto *)debug) -> print();
				} else {
					((Oper *)debug) -> print();
				}
			}
			std::cout << "\n----------------------------------\n";
		}
	}
	for (const auto &infix: infixlines) {
		postfixlines.push_back(build_postfix(infix));
	}
	if (DEBUG) {
		for (int i = 0; i < postfixlines.size(); ++i) {
			for (auto &debug: postfixlines[i]) {
				if (debug == nullptr) {
					continue;
				}
				if (Number *ptr = dynamic_cast<Number *>(debug)) {
					((Number *)debug) -> print();
					continue;
				}
				if (Variable *ptr = dynamic_cast<Variable *>(debug)) {
					((Variable *)debug) -> print();
					continue;
				} 
				if (Goto *ptr = dynamic_cast<Goto *>(debug)) {
					((Goto *)debug) -> print();
				} else {
					((Oper *)debug) -> print();
				}
			}
			std::cout << "\n----------------------------------\n";
		}
	}
	int row = 0;
	while (0 <= row && row < (int)postfixlines.size()) {
		row = evaluate_postfix(postfixlines[row], row);
	}
	for (auto &cl: Variable::vtable) {
		std::cout << cl.first << " = " << cl.second << std::endl;
	}
	for (int i = 0; i < infixlines.size(); i++) {
		for (auto &cl: infixlines[i]) {
			if (cl) {
				delete cl;
			}
		}
	}
	return 0;
}
