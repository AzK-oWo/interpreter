interpreter:
	g++ src/interpreter.cpp -fpic -shared -o lib/libinterpreter.so -I ./header/ -std=c++11
%:
	g++ src/$@.cpp -o bin/$@ -I ./header -L ./lib -linterpreter
all:
	g++ src/interpreter.cpp -fpic -shared -o lib/libinterpreter.so -I ./header/
	g++ src/main.cpp -o bin/main -I ./header -L ./lib -linterpreter
test1:
	./bin/interpreter < tests/test1.txt --verbose
test2:
	./bin/interpreter < tests/test2.txt --verbose
test3:
	./bin/interpreter < tests/test3.txt --verbose
test4:
	./bin/interpreter < tests/test4.txt --verbose
test5:
	./bin/interpreter < tests/test5.txt --verbose
rm:
	rm bin/interpreter
subl:
	subl src/* headers/*
atom:
	atom src/* headers/*
fastcreate:
	g++ src/* -o bin/interpreter -I headers/ -std=c++11 -fmax-errors=2
