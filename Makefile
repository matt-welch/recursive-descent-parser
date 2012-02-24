parser: parser.o
	g++ -o parser parser.o

parser.o: parser.cpp
	g++ -c $(CONFIG) parser.cpp
	
clean:
	rm -f parser *.o core core.*

tidy: clean
	rm -f *.*~ *~

BINNAME = parser

DEBUG_FLAGS = -g3 -ggdb -O0 -Wall -pedantic -DDEBUG
CONFIG		= -Wall -pedantic

debug: CONFIG=$(DEBUG_FLAGS)
debug: BINNAME


#.PHONY: submit
#submit: tidy 
# 	cd ..
#	cp -r p2 James_Welch_Proj2
#	zip -r James_Welch_Proj2 James_Welch_Proj2

