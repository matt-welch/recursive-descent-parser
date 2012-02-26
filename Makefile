parser: parser.o
	g++ -o parser parser.o

parser.o: parser.cpp
	g++ -c $(CONFIG) parser.cpp
	
clean:
	rm -f parser *.o core core.*

tidy: clean
	rm -f *.*~ *~

DEBUG_FLAGS = -g3 -ggdb -O0 -Wall -pedantic -DDEBUG -std=c++0x
CONFIG		= -Wall -pedantic -std=c++0x

debug: CONFIG=$(DEBUG_FLAGS)
debug: parser

debugger: CONFIG=$(DEBUG_FLAGS)
debugger: parser
	gdb ./parser

test: debug
	./parser grammar.txt

#.PHONY: submit
#submit: tidy 
# 	cd ..
#	cp -r p2 James_Welch_Proj2
#	zip -r James_Welch_Proj2 James_Welch_Proj2

