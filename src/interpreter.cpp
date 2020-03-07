#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <typeinfo>
#include <unistd.h>


enum OPERATOR {
	LBRACKET, RBRACKET,
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

int PRIORITY [] = { // TY[] = 
	-1, -1,
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

std::string OPERTEXT [] = {
	"(", ")",
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












class Lexem {
  public:
	Lexem() {};
	virtual ~Lexem() {};
};











class Number : public Lexem {
	int value;
  public:
	Number() {};
	Number(int smthg) {
		value = smthg;
	};
	virtual ~Number() {};
	int getValue();
	void print() {
		std::cout << value << ' ';
	};
};

int Number::getValue(){
	return value;
}











class Oper : public Lexem {
	OPERATOR opertype;
  public:
	Oper () {};
	Oper (OPERATOR opertype) {
		Oper::opertype = opertype;
	};
	virtual ~Oper () {};
	OPERATOR getType () {
		return opertype;
	};
	int getPriority ();
	int getValue (Lexem *leftlex, Lexem *rightlex);

	void print() {
		std::cout << OPERTEXT[(int)opertype] << ' ';
	};

};


int Oper::getPriority() { //get_priority()
	return PRIORITY[(int)opertype];
}








class Goto : public Oper { // Goto: public
  public:
	static std::map<std::string, int> lTable;
};

std::map<std::string, int> Goto::lTable;












class Variable : public Lexem { // Variable: public
	std::string vName;
  public:
	Variable() {};
	Variable(std::string str) {
		vName = str;
		vTable[vName];
	};
	~Variable() {};
	std::string getName();
	int getValue();
	void setValue(int value);
	static std::map<std::string,int> vTable;

	void print() {
			std::cout << vName << ' ';
		};
};

std::map<std::string,int> Variable::vTable;

std::string Variable::getName() {
	return vName;
}

void Variable::setValue(int value) {
	vTable[vName] = value;
}

int Variable::getValue() {
	return vTable[vName];
}



















int Oper::getValue(Lexem *leftlex, Lexem *rightlex) {
	int left, right;
	if (Number *ptr = dynamic_cast<Number *>(leftlex)) {
		left = ((Number *)leftlex) -> getValue();
	}
	if (Number *ptr = dynamic_cast<Number *>(rightlex)) {
		right = ((Number *)rightlex) -> getValue();
	}
	if (Variable *ptr = dynamic_cast<Variable *>(leftlex)) {
		left = ((Variable *)leftlex) -> getValue();
	}
	if (Variable *ptr = dynamic_cast<Variable *>(rightlex)) {
		right = ((Variable *)rightlex) -> getValue();
	}
	switch (Oper::opertype) { // tab
			case ASSIGN:
				int tmp;
				if (Number *ptr = dynamic_cast<Number *>(rightlex)) {
					tmp = ((Number *)rightlex) -> getValue();
				} else {
					tmp = ((Variable *)rightlex) -> getValue();
				}
				((Variable *)leftlex) -> setValue(tmp);
				return tmp;
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
}























Lexem *isOper(std::string codeline, int *i, int size) { //is_oper()
	int p = *i;
	Lexem *lex = nullptr;
	char tmp = codeline[p];
	while (p < size && (tmp == ' ' || tmp == '\t')) {
		++p;
		tmp = codeline[p];
	}
	*i = p;
	int n = sizeof(OPERTEXT) / sizeof(std::string);
	for (int op = 0; op < n; op++) {
		std::string subcodeline = codeline.substr(p, OPERTEXT[op].size());
		if (OPERTEXT[op] == subcodeline) {
			lex = new Oper((OPERATOR)op);
			*i += OPERTEXT[op].size();
			break;
		}
	}
	return lex;
}



Lexem *isNumber(std::string codeline, int *i, int size) { // int &
	int p = *i, number = 0;
	for ( ; p < size && codeline[p] >= '0' && codeline[p] <= '9'; p++) {
		number = number * 10 + codeline[p] - '0';
	}
	if (p == *i) {
		return nullptr;
	}
	*i = p;
	Lexem *lex = new Number(number);
	return lex;
}



Lexem *isVariable(std::string codeline, int *i, int size) {
	int p = *i;
	std::string vrbl;
	while (p < size && ((codeline[p] >= 'A' && codeline[p] <= 'Z') || (codeline[p] >= 'a' && codeline[p] <= 'z') || (codeline[p] >= '0' && codeline[p] <= '9'))) {
		vrbl += codeline[p]; //variable
		p++;
	}
	if (p == *i) { //if vrbl.isempty()
		return nullptr;
	}
	*i = p; 
	Lexem *lex = new Variable(vrbl);
	return lex;
}



std::vector<Lexem *> parseLexem(std::string codeline) {
	codeline = codeline + ' ';
	int size = codeline.size();
	std::vector<Lexem *> infix;
	Lexem *lex;
	int i = 0;
	while (i < size) {
		lex = isOper(codeline, &i, size);
		if (lex != nullptr) {
				infix.push_back(lex);
				continue;
		}
		lex = isNumber(codeline, &i, size);
		if (lex != nullptr) {
			infix.push_back(lex);
			continue;
		}
		lex = isVariable(codeline, &i, size);
		if (lex != nullptr) {
			infix.push_back(lex);
			continue;
		}
	}
	return infix;
}


void initLabels(std::vector<Lexem *> &infix, int row) {
	int size = infix.size();
	for (int i = 1; i < size; ++i) {
		Variable *ptr1;
		Oper *ptr2;
		if ((ptr1 = dynamic_cast<Variable *>(infix[i - 1])) && (ptr2 = dynamic_cast<Oper *>(infix[i]))) {
			Variable *lexemvar = (Variable *)infix[i - 1];
			Oper *lexemop = (Oper *)infix[i];
			if (lexemop -> getType() == COLON) {
				Goto::lTable.insert({lexemvar -> getName(), row});
				Variable::vTable.erase(lexemvar -> getName()); // r->g
				delete infix[i - 1];
				delete infix[i];
				infix[i - 1] = nullptr;
				infix[i] = nullptr;
				i++;
			}
		}
	}
}

std::vector<Lexem *> buildPostfix (std::vector<Lexem *> infix) {
	std::vector<Lexem *> lexemStack;
	std::vector<Oper *> operStack;
	for (auto &elem: infix) {
		if (elem == nullptr) {
			continue;
		}
		Number *ptr1;
		Variable *ptr2;
		if ((ptr1 = dynamic_cast<Number *>(elem)) || (ptr2 = dynamic_cast<Variable *>(elem))) {
			lexemStack.push_back(elem);
			continue;
		}
		OPERATOR tmp_elem = ((Oper *)elem) -> getType();
		if (operStack.empty() || tmp_elem == LBRACKET) {
			operStack.push_back((Oper *)elem);
			continue;
		}
		if (tmp_elem == RBRACKET) {
			while ((operStack.back()) -> getType() != LBRACKET) {
				lexemStack.push_back(operStack.back());
				operStack.pop_back();
			}
			operStack.pop_back();
			continue;
		}
		if (tmp_elem == ASSIGN && ASSIGN == (operStack.back()) -> getType()) {
			operStack.push_back((Oper *)elem);
			continue;
		}
		if (((Oper *)elem) -> getPriority() <= (operStack.back()) -> getPriority()) {
			while (!(operStack.empty()) && ((operStack.back()) -> getPriority() > 0)) {
				lexemStack.push_back(operStack.back());
				operStack.pop_back();
			}
			operStack.push_back((Oper *)elem);
		} else {
			operStack.push_back((Oper *)elem);
		}
	}
	while (!operStack.empty()) {
		lexemStack.push_back(operStack.back());
		operStack.pop_back();
	}
	return lexemStack;
}


int evaluatePostfix(std::vector<Lexem *> postfix, int row) {
	std::vector<Lexem *> estimation;
	std::vector<Number *> need_to_clear;
	Lexem *res;
	for (auto &elem: postfix) {
		if (Number *ptr = dynamic_cast<Number *>(elem)) {
			estimation.push_back(elem);
			continue;
		}
		if (Variable *ptr = dynamic_cast<Variable *>(elem)) {
			estimation.push_back(elem);
		} else {
			if (((Oper *)elem) -> getType() == GOTO) {
				Variable *label = (Variable *)estimation.back();
				estimation.pop_back();
				return Goto::lTable[label -> getName()];
			}
			res = estimation.back();
			estimation.pop_back();
			res = new Number(((Oper *)elem) -> getValue(estimation.back(), res));
			estimation.pop_back();
			estimation.push_back(res);
			need_to_clear.push_back((Number *)res);
		}
	}
	res = estimation.back();
	estimation.pop_back();
	for (auto &cl: need_to_clear) {
		delete cl;
	}
	return row + 1;
}

// ./interpreter filename.txt
// ./interpreter filename.txt --verbose
// ./interpreter --verbose


int main(void) { // main(int argc, char** argv)
	// for (i = 1; i < argc; i++)
	//     if (string(argv[i]) == "--verbose") {
	//          DEBUG = 1;
	//     }

	// for (i = 1; i < argc; i++)
	//     if (string(argv[i]).substr(???) == ".txt") {
	//          INTERACTIV = 0;
	//     }
	
	std::string codeline;
	std::vector<std::vector<Lexem *>> infixLines, postfixLines;
//	int value;
	while (getline(std::cin, codeline)) {
		
		if (codeline == "exit") {
			break;
		}

		infixLines.push_back(parseLexem(codeline));
/*		for (int i = 0; i < infixLines.size(); ++i) {
			std::cout << std::endl << "size of infix = " << infixLines[i].size() <<std::endl;
			for (auto &p: infixLines[i]) {
				if (Number *ptr = dynamic_cast<Number *>(p)) {
					((Number *)p) -> print();
					continue;
				}
				if (Variable *ptr = dynamic_cast<Variable *>(p)) {
					((Variable *)p) -> print();
				} else {
					((Oper *)p) ->print();
				}
			}
		}
		std::cout << "\n----------------------------------\n";*/
	}

	for (int row = 0; row < infixLines.size(); ++row) {
		initLabels(infixLines[row], row);
	}

/*	for (auto &i: Goto::lTable) {
		std::cout << i.first << " in " << i.second << std::endl;
	}*/

	for (const auto &infix: infixLines) {
		postfixLines.push_back(buildPostfix(infix));
	}
/*	for (int i = 0; i < postfixLines.size(); ++i) {
		for (auto &p: postfixLines[i]) {
			if (Number *ptr = dynamic_cast<Number *>(p)) {
				((Number *)p) -> print();
				continue;
			}
			if (Variable *ptr = dynamic_cast<Variable *>(p)) {
				((Variable *)p) -> print();
			} else {
				((Oper *)p) -> print();
			}
		}
		std::cout << "\n----------------------------------\n";
	}*/
	int row = 0;

	while (0 <= row && row < (int)postfixLines.size()) {
		row = evaluatePostfix(postfixLines[row], row);
	}


	for (auto &cl: Variable::vTable) {
		std::cout << cl.first << " = " << cl.second << std::endl;
	}


	for (int i = 0; i < infixLines.size(); i++) {
		for (auto &cl: infixLines[i]) {
			if (cl) {
				delete cl;
			}
		}
	}
	return 0;
}
