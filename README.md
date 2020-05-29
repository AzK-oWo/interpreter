# Interpreter created by AzK

## About interpreter

I am sudent from MSU Kazakhstan branch and it my first experience to create an interpreter  

This interpreter has simular options like other programming languages

### There is a binary arithmetic:

Symbol | meaning
-------|--------
`+` | plus  
`-` | minus  
`*` | multyply  
`/` | division  
`%` | mod  
`and` | binary and  
`or` | binary or  
`|` | bitware or  
`&` | bitware and  
`> >= < <= != :=` | comparison signs  
`>> <<` | bit shift

### How to use goto:
	
	...
	goto L: 
	...
	L:
	...

### How to use if else and while:

	if condition:
		...
	else:
		...
	@


	while condition:
		...
	@

### How to declare function and use it:

	def function(arg1, arg2, arg3)
		...
		return value
	fed
	...
	x = function(a,b,c)	

	def function()
		...
		return 5 + 9
	fed
	...
	function()

	def function(arg1)
		...
		return
	fed
	...
	function(a)

### Code always have entry point:

	def main()
		...
		return 0
	fed

## Some examples

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

### Also it has error handler, which is working with try-catch blocks.

### Interpreter has debug options. You need just to use the flag **--verbose**.

### It would be better to add interactive mode.

## How to start:

### Compile:

	make fastcreate

### Start the program:

The code have to be written in the txt file in advance.

If you want to use some examples you can see in "tests" folder. Use the command below to run the program:
	
	make test$n # where $n := 1 - 5

Run the program with your txt file as follow:

	bin/interpreter << your_code.txt --verbose 