export LD_LIBRARY_PATH = "$(pwd)/bin"

interpreter:
	g++ src/interpreter.cpp -fpic -shared -o lib/libinterpreter.so -I ./header/
%:
	g++ src/$@.cpp -o bin/$@ -I ./header -L ./lib -linterpreter
all:
	g++ src/interpreter.cpp -fpic -shared -o lib/libinterpreter.so -I ./header/
	g++ src/main.cpp -o bin/main -I ./header -L ./lib -linterpreter
test:
	./bin/interpreter < tests/test1.txt
test2:
	./bin/interpreter < tests/test2.txt
test3:
	./bin/interpreter < tests/test3.txt
test4:
	./bin/interpreter < tests/test4.txt
rm:
	rm bin/interpreter
subl:
	subl src/*
atom:
	atom src/*
fastcreate:
	g++ src/* -o bin/interpreter -I headers/ -std=c++11
