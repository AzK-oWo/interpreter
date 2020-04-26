#ifndef OPER_HPP
#define OPER_HPP

#include "Lexem.hpp"
#include "interpreter.hpp"

class Oper: public Lexem {
	OPERATOR opertype;
  public:
	Oper () {};
	Oper (OPERATOR opertype) {
		Oper::opertype = opertype;
	};
	virtual ~Oper () {};
	OPERATOR get_type();
	int get_priority();
	int get_value(Lexem *leftlex, Lexem *rightlex);
	virtual void print();
};

#endif