#ifndef NUMBER_HPP
#define NUMBER_HPP

#include "Lexem.hpp"

class Number: public Lexem {
	int value;
  public:
	Number();
	Number(int smthg);
	~Number();
	void set_value(int smthg);
	int get_value();
	void print();
};

#endif