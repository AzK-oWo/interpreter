bin: src/interpreter.cpp
	g++ src/interpreter.cpp -o bin/interpreter -std=c++11 -fmax-errors=2
start:
	./bin/interpreter < tests/test.txt
rm:
	rm bin/interpreter
subl:
	subl src/interpreter.cpp
atom:
	atom src/interpreter.cpp
