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

test: debug grammar2.txt
	./parser grammar2.txt

.PHONY: submit
submit: tidy James_Welch_Proj2.zip NonTerminal.cpp NonTerminal.hpp parser.cpp Makefile
	
James_Welch_Proj2.zip:
	cp NonTerminal.cpp NonTerminal.hpp parser.cpp  Makefile ./James_Welch_Proj2
	zip -r James_Welch_Proj2.zip ./James_Welch_Proj2
