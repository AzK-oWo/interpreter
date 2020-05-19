# Interpreter created by AzK

## About interpreter

I am sudent from KB MSU and it my first experience to create an interpreter  

This interpreter has simular options like other programming languages

### It has only binary arithmetic:

*+* - plus  
*-* - minus  
_*_ - multyply  
*/* - division  
*%* - mod  
*and* - binary and  
*or* - binary or  
*|* - bitware or  
*&* - bitware and  
*>, >=, <, <=, !=, :=* - comparison signs  
*>>, <<* - bit shift

### How to use goto:
	
	...
	goto L: 
	...
	L: # label always have to exist
	_code_

### How to use if else and while:

	if _condition_:
		_code_
	else:
		_code_
	@



	while condition:
		_code_
	else:
		_code_
	@ # this is the end of contion. It must always be used

### How to declare function and use it:

	def function(arg1, arg2, ...)
		_code_
		return nothing_or_value
	fed # this is the end of function. It must always be used
	...
	function(a,b,c)	

### Code always have to have main function(where code execution starts)

	def main()
		_code_
		return 0
	fed

## What iterpreter is capable of

### Arithmetic operation:

	def main()
		1 + 2 * 3
		(1 + 2) * 3
		1 and 0
		2 >> 1
		(6 - 9) % 2
		return 0
	fed

### Working with variables:

	def main()
		x := 1
		y := x + 2
		z := 3 * 4 + 5 + y
		return 0
	fed

### Using goto and labels:

	def main()
		x := 1
		y := x + 2
		z:=3*4+5
		goto L
		y := 2
		return 1
	L: 
		x := 3
		return 0
	fed

### If-else and while conditions:

	def main()
		i := 1
		s := 0
		while i < 4:
			s := s + i
			i := i + 1
		@
		i := i + 1
		return 0
	fed

### Functions with arguments:

	def sqrt(a)
		return a * a
	fed

	def foo()
		a := 5
		c := 4
		r := 8
		return
	fed

	def qwerty(a, b, c)
		c := b - a
		return c
	fed

	def sum(a, b)
		return a + b
	fed

	def main()
		a := 5
		b := 6
		definition := sum(a, b) + 1 - qwerty(a, b, sum(a, b - 3))
		i:= 2
		foo()
		definition := definition + i
		if (i > 0) and (definition) > 0:
			sqrt(definition)
		@
		return
	fed

### Also it has error handler, which is working by try-catch blocks.

### Interpreter has debug options. You just need to use the flag **--verbose**.

### doesn't has interactive form, so the code have to be inside txt file.

## How to start:

Firstable, program needs to be complied by command **make fastcreate**

Secondly, code have to be written in the txt file in advance

If you want to use some example codes you can check it in "tests" folder. Use command **make test n _(n from 1 to 5)_** to run the program by one of example.

Final, run the program with your txt file like *bin/interpreter << my_example.txt --verbose* and enjoy 