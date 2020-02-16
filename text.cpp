
#include "Query.h"
#include "TextQuery.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>

using std::set;
using std::string;
using std::map;
using std::vector;
using std::cerr;
using std::cout;
using std::cin;
using std::ifstream;
using std::endl;

//#1:TextQuery≤‚ ‘
/*void runQueries(ifstream &infile)
{
	// infile is an ifstream that is the file we want to query
	TextQuery tq(infile);  // store the file and build the query map
	// iterate with the user: prompt for a word to find and print results
	while (true) {
		cout << "enter word to look for, or q to quit: ";
		string s;
		// stop if we hit end-of-file on the input or if a 'q' is entered
		if (!(cin >> s) || s == "q") break;
		// run the query and print the results
		print(cout, tq.query(s)) << endl;
	}
}



// program takes single argument specifying the file to query
int main( )
{
	ifstream infile("data.txt");
	if (!infile)
	{
		cerr << " wrong";
		return 0;
	}

	//runQueries(infile);
	TextQuery tq(infile);
	tq.display_map();
	QueryResult ret = tq.query("they");
	print(cout, ret);
	return 0;
}*/



//#2:WordQuery≤‚ ‘
/*int main()
{
	//TextQuery file = get_file(argc, argv);
	ifstream infile("data.txt");
	TextQuery file(infile);

	// iterate with the user: prompt for a word to find and print results
	do {
		string sought;
		if (!get_word(sought)) break;

		// find all the occurrences of the requested string
		// define synonym for the line_no set 
		Query name(sought);
		const QueryResult results = name.eval(file);
		cout << "\nExecuting Query for: " << name << endl;

		// report no matches
		print(cout, results) << endl;
	} while (true);  // loop indefinitely; the exit is inside the loop
	return 0;
}*/


//#3:AndQuery≤‚ ‘
/*int main()
{
	// gets file to read and builds map to support queries
	//TextQuery file = get_file(argc, argv);
	ifstream infile("data.txt");
	TextQuery file(infile);

	do {
		string sought1, sought2;
		// stop if hit eof on input or a "q" is entered
		if (!get_words(sought1, sought2)) break;

		// find all the occurrences of the requested string
		Query andq = Query(sought1) & Query(sought2);
		cout << "\nExecuting query for: " << andq << endl;
		QueryResult results = andq.eval(file);
		// report matches
		print(cout, results);

		results = Query(sought1).eval(file);
		cout << "\nExecuted query: " << Query(sought1) << endl;
		// report matches
		print(cout, results);

		results = Query(sought2).eval(file);
		cout << "\nExecuted query: " << Query(sought2) << endl;
		// report matches
		print(cout, results);
	} while (true);

	return 0;
}*/


//#4.OrQuery
/*int main()
{
	// gets file to read and builds map to support queries
	//TextQuery file = get_file(argc, argv);
	ifstream infile("data.txt");
	TextQuery file(infile);

	do {
		string sought1, sought2;
		// stop if hit eof on input or a "q" is entered
		if (!get_words(sought1, sought2)) break;

		// find all the occurrences of the requested string
		Query andq = Query(sought1) | Query(sought2);
		cout << "\nExecuting query for: " << andq << endl;
		QueryResult results = andq.eval(file);
		// report matches
		print(cout, results);

		results = Query(sought1).eval(file);
		cout << "\nExecuted query: " << Query(sought1) << endl;
		// report matches
		print(cout, results);

		results = Query(sought2).eval(file);
		cout << "\nExecuted query: " << Query(sought2) << endl;
		// report matches
		print(cout, results);
	} while (true);

	return 0;
}*/


//#5.NotQuery
/*int main()
{
	//TextQuery file = get_file(argc, argv);
	ifstream infile("data.txt");
	TextQuery file(infile);

	// iterate with the user: prompt for a word to find and print results
	do {
		string sought;
		if (!get_word(sought)) break;

		Query notq = ~Query(sought);
	    QueryResult results = notq.eval(file);
		cout << "\nExecuting Query for: " << notq << endl;
		print(cout, results) << endl;

		Query name(sought);
		results = name.eval(file);
		cout << "\nExecuting Query for: " << name << endl;

		print(cout, results) << endl;
	} while (true);  // loop indefinitely; the exit is inside the loop
	return 0;
}*/


//#6.And_OrQuery≤‚ ‘
int main( )
{
	// gets file to read and builds map to support queries
	//TextQuery file = get_file(argc, argv);
	ifstream infile("data.txt");
	TextQuery file(infile);

	// iterate with the user: prompt for a word to find and print results
	while (true) {
		string sought1, sought2, sought3;
		if (!get_words(sought1, sought2)) break;
		cout << "\nenter third word: ";
		cin >> sought3;

		// find all the occurrences of the requested string
		Query q = Query(sought1) & Query(sought2)
			| Query(sought3);
		cout << "\nExecuting Query for: " << q << endl;
		const QueryResult results = q.eval(file);
		// report matches
		print(cout, results);
	}
	return 0;
}

