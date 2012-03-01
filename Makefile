parser: parser.o NonTerminal.o
	g++ -o parser parser.o NonTerminal.o
	
parser.o: parser.cpp 
	g++ -c $(CONFIG) parser.cpp
	
NonTerminal.o: NonTerminal.cpp NonTerminal.hpp
	g++ -c $(CONFIG) NonTerminal.cpp
	
clean:
	rm -f parser *.o core core.*

tidy: clean
	rm -f *.*~ *~

DEBUG_FLAGS = -g3 -ggdb -O0 -Wall -pedantic -DDEBUG
CONFIG		= -Wall -pedantic

debug: CONFIG=$(DEBUG_FLAGS)
debug: parser

debugger: CONFIG=$(DEBUG_FLAGS)
debugger: parser
	gdb ./parser

test: debug
	./parser grammar.txt

#.PHONY: submit
#submit: tidy 
# 	cd /..
#	cp -r p2 James_Welch_Proj2
#	zip -r James_Welch_Proj2 James_Welch_Proj2

