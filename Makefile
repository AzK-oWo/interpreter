bin:
	g++ src/interpreter.cpp -o bin/interpreter -std=c++11 -fmax-errors=2
start:
	./bin/interpreter
rm:
	rm bin/interpreter
