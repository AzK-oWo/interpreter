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

enum CLASSTYPE {
	LEX,
	NUMBER,
	OPER,
};

class Lexem {
  public:
	Lexem() {};
	virtual ~Lexem() {};
	virtual CLASSTYPE getClasstype() {
		return LEX;
	};
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
	virtual CLASSTYPE getClasstype() {
		return NUMBER;
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
	int getValue (int left, int right);


	virtual CLASSTYPE getClasstype() {
		return OPER;
	};
	

	void print() {
		switch (opertype) {
			case PLUS:
				std::cout << "+ ";
				break;
			case MINUS:
				std::cout << "- ";
				break;
			case MULT:
				std::cout << "* ";
				break;
			case RBRACKET:
				std::cout << ") ";
				break;			
			case LBRACKET:
				std::cout << "( ";
				break;
			case DIV:
				std::cout << "/ ";
				break;
			case MOD:
				std::cout << "% ";
				break;
		}
	};


};

int Oper::getPriority() {
	return PRIORITY[(int)opertype];
}

int Oper::getValue(int left, int right) {
	switch (Oper::opertype) {
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
	}
	return 0;
}

std::vector<Lexem *> parseLexem(std::string codeline) {
	int line_size = codeline.size();
	std::vector<Lexem *> res;
	Lexem *lex;
	int number = 0;
	for (int i = 0, mode = 0; i < line_size; ++i)
	{
		if (codeline[i] >= '0' && codeline[i] <= '9') {
			mode = 1;
			number = number * 10 + codeline[i] - '0';
		} else {
			if (mode){	
				lex = new Number(number);
				res.push_back(lex);
				number = 0;
				mode = 0;
			}
			bool flag = true;
			switch (codeline[i]) {

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
/*				case ":=":
					lex = new Oper(ASSIGN);
					break;
				case '&':
					lex = new Oper(BITAND);
					break;
				case '|':
					lex = new Oper(BITOR);
					break;
				case '^':
					lex = new Oper(XOR);
					break;
				case "and":
					lex = new Oper(AND);
					break;
				case "or":
					lex = new Oper(OR);
					break;
				case "<=":
					lex = new Oper(LEQ);
					break;
				case '<':
					lex = new Oper(LT);
					break;
				case '>':
					lex = new Oper(GEQ);
					break;
				case ">=":
					lex = new Oper(GT);
					break;
				case "==":
					lex = new Oper(EQ);
					break;
				case "!=":
					lex = new Oper(NEQ);
					break;
				case ">>":
					lex = new Oper(SHR);
					break;
				case "<<":
					lex = new Oper(SHL);
					break;*/
				default:
					flag = false;
					break;
			}

			if (flag) {
				res.push_back(lex);
			}
		}
	}
	if (number != 0) {
		lex = new Number(number);
		res.push_back(lex);	
	}
	return res;
}

std::vector<Lexem *> buildPostfix (std::vector<Lexem *> infix) {
	std::vector<Lexem *> lexemStack;
	std::vector<Oper *> operStack;
	for (auto &elem: infix) {
		if (elem -> getClasstype() == NUMBER) {
			lexemStack.push_back(elem);
		} else {
			OPERATOR tmp_elem = ((Oper *)elem) -> getType();
			if (operStack.empty() || tmp_elem == LBRACKET) {
				operStack.push_back((Oper *)elem);
			} else {
				if (tmp_elem == RBRACKET) {
					while ((operStack.back()) -> getType() != LBRACKET) {
						lexemStack.push_back(operStack.back());
						operStack.pop_back();
					}
					operStack.pop_back();
				} else {
					if (tmp_elem == ASSIGN && ASSIGN == (operStack.back()) -> getType()) {
						operStack.push_back((Oper *)elem);
					} else {
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
				}
			}
		}
	}
	while (!operStack.empty()) {
		lexemStack.push_back(operStack.back());
		operStack.pop_back();
	}
	return lexemStack;
}

int evaluatePostfix(std::vector<Lexem *> postfix) {
	std::vector<int> estimation;
	int res;
	for (auto &label: postfix) {
		if (label -> getClasstype() == NUMBER) {
			estimation.push_back(((Number *)label) -> getValue());
		} else {
			res = estimation.back(),
			estimation.pop_back();
			res = ((Oper *)label) -> getValue(estimation.back(), res);
			estimation.pop_back();
			estimation.push_back(res);
		}
	}
	estimation.clear();
	return res;
}


int main(void) {
	std::string codeline;
	std::vector<Lexem *> infix;
	std::vector<Lexem *> postfix;
	int value;

	while (getline(std::cin, codeline)) {
		infix = parseLexem(codeline);
/*		std::cout << std::endl << infix.size() << std::endl;
		for (auto &p: infix) {
			if (p -> getClasstype() == NUMBER) {
				((Number *)p) -> print();
			}
			else {
				((Oper *)p) -> print();
			}
		}*/
		postfix = buildPostfix(infix);
/*		std::cout << std::endl << postfix.size() << std::endl;
		for (auto &p: postfix) {
			if (p -> getClasstype() == NUMBER) {
				((Number *)p) -> print();
			}
			else {
				((Oper *)p) -> print();
			}
		}
		std::cout << std::endl;*/
		value = evaluatePostfix(postfix);
		std::cout << ">>>" << value << std::endl;
		postfix.clear();
	} 
	return 0;
}