#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <typeinfo>

enum OPERATOR {
	LBRACKET, RBRACKET,
	ASSIGN,
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

int PRIORITY [] = {
	-1, -1,
	0,
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
	":= ",
	"or ",
	"and ",
	"|",
	"^",
	"&",
	"== ", "!= ",
	"<= ", "<",
	">= ", ">",
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
  public :
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

int Oper::getPriority() {
	return PRIORITY[(int)opertype];
}

class Variable : public Lexem {
	std::string vName;
  public:
	Variable() {};
	Variable(std::string str) {
		vName = str;
		vTable.insert({vName, 0});
	};
	~Variable() {};
	int getValue();
	void setValue(int value);
	static std::map<std::string,int> vTable;

	void print() {
			std::cout << vName << ' ';
		};
};

std::map<std::string,int> Variable::vTable;

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
	switch (Oper::opertype) {
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

static std::map<std::string, int> vTable;


Lexem *isOper(std::string codeline, int *i, int size) {
	int p = *i;
	Lexem *lex = nullptr;
	char tmp = codeline[p];
	while (p < size && (tmp == ' ' || tmp == '\t')) {
		++p;
		tmp = codeline[p];
	}
	*i = p;
	if (p != size) {
		//сделать без кейсов презентация
		switch (codeline[p]) {
				case '+':
					lex = new Oper(PLUS);
					break;
				case '-':
					lex = new Oper(MINUS);
					break;
				case '*':
					lex = new Oper(MULT);
					break;
				case '(':
					lex = new Oper(LBRACKET);
					break;
				case ')':
					lex = new Oper(RBRACKET);
					break;
				case '/':
					lex = new Oper(DIV);
					break;
				case '%':
					lex = new Oper(MOD);
					break;
				case '^':
					lex = new Oper(XOR);
					break;
				case ':':
					if (codeline[p + 1] == '=') {
						lex = new Oper(ASSIGN);
						p++;
					}
					break;
				case '!':
					if(codeline[p + 1] == '=') {
						lex = new Oper(NEQ);
						p++;
					}
					break;
				case '=':
					if (codeline[p + 1] == '=') {
						lex = new Oper(EQ);
						p++;
					}
					break;
				case '&':
					if (codeline[p + 1] == '&') {
						lex = new Oper(AND);
						p++;
					} else {
						lex = new Oper(BITAND);
					}
					break;
				case '|':
					if (codeline[p + 1] == '|') {
						lex = new Oper(OR);
						p++;
					} else {
						lex = new Oper(BITOR);
					}
					break;
				case '<':
					if (codeline[p + 1] == '=') {
						lex = new Oper(LEQ);
						p++;
					} else {
						if (codeline[p + 1] == '<') {
							lex = new Oper(SHL);
							p++;
						} else {
							lex = new Oper(LT);
						}
					}
					break;
				case '>':
					if (codeline[p + 1] == '=') {
						lex = new Oper(GEQ);
						p++;
					} else {
						if (codeline[p + 1] == '>') {
							lex = new Oper(SHR);
							p++;
						} else {
							lex = new Oper(GT);
						}
					}
					break;
				default:
					return nullptr;
					break;
			}
			*i = p + 1;
	}
	return lex;
}

Lexem *isNumber(std::string codeline, int *i, int size) {
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
		vrbl += codeline[p];
		p++;
	}
	if (p == *i) {
		return nullptr;
	}
	*i = p;
	Lexem *lex = new Variable(vrbl);
	return lex;
}

std::vector<Lexem *> parseLexem(std::string codeline) {
	codeline = codeline + ' ';
	int size = codeline.size();
	std::vector<Lexem *> res;
	Lexem *lex;
	int i = 0;
	while (i < size) {
		lex = isOper(codeline, &i, size);
		if (lex != nullptr) {
				res.push_back(lex);
				continue;
		}
		lex = isNumber(codeline, &i, size);
		if (lex != nullptr) {
			res.push_back(lex);
			continue;
		}
		lex = isVariable(codeline, &i, size);
		if (lex != nullptr) {
			res.push_back(lex);
			continue;
		}
	}
	return res;
}


std::vector<Lexem *> buildPostfix (std::vector<Lexem *> infix) {
	std::vector<Lexem *> lexemStack;
	std::vector<Oper *> operStack;
	for (auto &elem: infix) {
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
			while (!(operStack.empty()) && ((operStack.back()) -> getType()) != LBRACKET) {
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

int evaluatePostfix(std::vector<Lexem *> postfix) {
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
	int result;
	if (Number *ptr = dynamic_cast<Number *>(res)) {
		result = ((Number *)res) -> getValue();
	}
	if (Variable *ptr = dynamic_cast<Variable *>(res)) {
		result = ((Variable *)res) -> getValue();
	}
	for (auto &cl: need_to_clear) {
		delete cl;
	}
	return result;
}

int main(void) {
	std::string codeline;
	std::vector<Lexem *> infix;
	std::vector<Lexem *> postfix;
	int value;
	while (getline(std::cin, codeline)) {
		if (codeline == "exit") {
			break;
		}
		infix = parseLexem(codeline);
/*		std::cout << std::endl << "size of infix = " << infix.size() <<std::endl;
		for (auto &p: infix) {
			if (Number *ptr = dynamic_cast<Number *>(p)) {
				((Number *)p) -> print();
				continue;
			}
			if (Variable *ptr = dynamic_cast<Variable *>(p)) {
				((Variable *)p) -> print();
			} else {
				((Oper *)p) ->print();
			}
		}*/
		postfix = buildPostfix(infix);
/*		std::cout << std::endl << postfix.size() << std::endl;
		for (auto &p: postfix) {
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
		std::cout << std::endl;*/
		value = evaluatePostfix(postfix);
		std::cout << ">>>" << value << std::endl;
		for (auto &cl: infix) {
			delete cl;
		}
	}
	return 0;
}
