/*******************************************************************************
 * FILENAME:	parser.cpp
 * DESCRIPTION:	Create Recursive Descent Parser automatically from a grammar
 * AUTHOR:		James Matthew Welch [JMW]
 * SCHOOL:		Arizona State University
 * CLASS:		CSE340: Principles of Programming Languages
 * INSTRUCTOR:	Dr. Toni Farley
 * SECTION:		27199
 * TERM:		Spring 2012
 ******************************************************************************/

/* Recursive Descent Parser Main Program:  */

#include <stdlib.h> // atoi(), exit()
#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::endl;
using std::cin;

#include <fstream>
using std::ifstream;

//#define DEBUG


// beginning of recursive Descent Parser driver
int main ( int argc, char *argv[] )
{
  if ( argc != 2 ) // argc should be 2 for correct execution
    // We print argv[0] assuming it is the program name
    cout<<"usage: "<< argv[0] <<" <filename>\n";
  else {
    // Assume argv[1] is a grammar filename to open
    ifstream filePtr( argv[1] );

    // check to see if file opening succeeded
    if ( filePtr.fail() )
    	cout << "Error opening file \"" << argv[1] << "\"\n";
    else {



    	char x;
      // the_file.get(x) returns false if the end of the file
      //  is reached or an error occurs
      while ( filePtr.get(x) )
        cout<< x;
    }
  }
  return 0;
}

#if 0
int main(){
	ifstream infile;
	string inFileName = "grammar.txt";
	int neighbors;
	string token;

	// ifstream.open() requires a const char*
	infile.open((char*)inFileName.c_str()); //2nd arg: ifstream::in

	if(infile.fail()){
		cout << endl <<"An error occurred while reading from the file \""
			<< inFileName << "\"." << endl;
	}else{
		// read the nodes in from the graph
		infile >> numNodes; // read in the number of nodes from the first line

#ifdef VECTORARRAY
		// create a vector-2D array
		vector< vector<int> > array(numNodes);

		for(vector< vector<int> >::iterator x = array.begin();
				x != array.end();
				++x)
		{
			for(vector<int>::iterator cell = x->begin();
					cell != x->end();
					++cell)
			{
				*cell = 6;
				cout << "Cell contents:[" << *cell << "]" << endl;
			}
		}
#endif

		// create adjacency matrix here (one more row than needed so can ignore row-0
		graph = new int*[numNodes+1];

		for(int i=0; i < numNodes+1; ++i){
			graph[i] = new int[numNodes + 1];

			//initialize graph to zeroes
			for(int j = 0; j < numNodes+1; ++j){
				graph[i][j] = 0;
			}
		}

#ifdef DEBUG
		cout << "Initialized::" << endl;
		printMatrix(graph, numNodes);
#ifdef VECTORARRAY
		printVectorMatrix(array);
#endif
#endif


		for(int i=1; i<=numNodes; ++i){
			infile >> neighbors;

#ifdef DISABLE
			cout << neighbors << endl;
#endif
			for(int j=1; j <= neighbors; ++j){
				infile >> token;
				graph[i][atoi(token.c_str())] = 1;
			}

		}

#ifdef DEBUG
		cout << "Filled::" << endl;
		printMatrix(graph, numNodes);
#endif

		infile.close();
		cout << endl <<"A graph with "<< numNodes <<" nodes has been read in from the file \"" << inFileName << "\"." << endl;
	}

	// take graph file and determine the number of connected components
	// initialize sets
	list = new GraphNode*[numNodes+1];
	for(int i=1; i<=numNodes; ++i){
		// Makes new nodes for each vertex & MakeSet(this) on each
		list[i] = new GraphNode(i+1);
	}

	stringstream buffer;
	for(int i=1; i<=numNodes; ++i){
		// loop through each node's neighbors
		buffer << "{" ;
		for(int j=1; j<=numNodes; ++j){
			if(graph[i][j]==1){
				if(list[i]->FindSet() != list[j]->FindSet())
				{
					cout << "Parent: " << i << ", Child: " << j << endl;
					buffer << j << ", ";
					list[i]->Union(list[j]);
				}
			}
		}
		// add parent node here?
		buffer << "} \n";



		// count the number of sets and display that info before displaying buffer
		//buffer.str("");
	}
	// cout << buffer.str() << endl << endl;

	// determine Same-Components


	cout << endl << endl << "Microsoft Visual Studio 2010 is shite!!!" << endl << endl;

	return 0;
}
#endif
