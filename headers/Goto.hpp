#ifndef GOTO_HPP
#define GOTO_HPP

#include <map>
#include <string>
#include "Oper.hpp"

class Goto: public Oper {
	int row;
  public:
  	enum {UNDEFINED = -INT32_MAX};
  	Goto ();
  	Goto (OPERATOR optype): Oper (optype) { row = UNDEFINED; }
  	~Goto () {};
  	void set_row(int row);
  	int get_row();
  	virtual void print();
	static std::map<std::string, int> ltable;
};

#endif