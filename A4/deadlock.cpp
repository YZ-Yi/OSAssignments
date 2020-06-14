/**********************************************
 * Last Name:   Yi
 * First Name:  Yuzhe
 * Student ID:  30105971
 * Course:      CPSC 457
 * Tutorial:    05
 * Assignment:  3
 *
 * File name: deadlock.cpp
 *********************************************/

#include <stdio.h>
#include <ctype.h>
#include <vector>
#include <string>
#include <cassert>
#include <unordered_map>
#include <set>
#include <list>
#include <array>


using namespace std;


typedef std::vector<std::string> vs_t;

enum Edge {
	Allocation, Request
};

// split string p_line into vector of strings (words)
// the delimiters are 1 or more whitespaces
vs_t
split( const std::string & p_line)
{
	auto line = p_line + " ";
	vs_t res;
	bool in_str = false;
	std::string curr_word = "";
	for( auto c : line) {
		if( isspace(c)) {
			if( in_str)
				res.push_back(curr_word);
			in_str = false;
			curr_word = "";
		}
		else {
			curr_word.push_back(c);
			in_str = true;
		}
	}
	return res;
}

// read in a line from stdin and parse it
// parsed results are passed back via parameter references
// return true on success, false on EOF
// aborts program on bad input (should throw exception)
bool
parseLine( std::string & proc_name, std::string & res_name, Edge & etype)
{
	char buff[4096];
	vs_t words;

	// read in a line and parse it into words
	while(1) { // loop to skip empty lines
		if( ! fgets( buff, sizeof(buff), stdin))
			return false;
		words = split(buff);
		if( words.size() != 0) break;
	}

	if( words.size() != 3) {
		printf("Bad line: %s\n", buff);
		exit( -1); // exception would be nicer
	}

	proc_name = words[0];
	res_name = words[2];
	etype = Edge::Request;

	if( words[1] == "->") {
		return true;
	}
	if( words[1] == "<-") {
		etype = Edge::Allocation;
		return true;
	}

	printf("Bad line: %s\n", buff);
	exit( -1); // exception would be nicer
};

// utility class you can use to convert words to unique integers
// get(word) returns the same number given the same word
//           the numbers will start at 0, then 1, 2, ...
//
// example:
//   Word2Int w2i
//   w2i.get("hello") = 0
//   w2i.get("world") = 1
//   w2i.get("hello") = 0
//
// HINT: this "may" help you get a bit more performance
// in your cycle finding algorithm, since indexed arrays are faster
// than hash tables... if you don't know what that means,
// safely ignore this hint :)
struct Word2Int {
	long
	get(const std::string & w) {
		auto f = myset.find(w);
		if( f == myset.end()) {
			myset[w] = counter;
			counter ++;
			return counter - 1;
		}
		return f-> second;
	}
 private:
	long counter = 0;
	std::unordered_map<std::string,long> myset;
};

struct Graph {

	// add your own members here
	// ...

	//  std::unordered_map<std::string, vs_t> adj_list;
	Word2Int w2i;
	// vector<list<int>> adj_list;
	// vector<int> out_counts;
	array<list<long>, 20000> adj_list;
	array<long, 20000> out_counts {0};
	// vector<list<long>> adj_list;
	// vector<long> out_counts;
	// std::unordered_map<std::string, std::list<std::string>> adj_list;
	// std::unordered_map<std::string, int> out_counts;
	std::set<std::string> all_nodes;
        vector<string> deadloc_pro;
        string cur_line;

	// Graph() : adj_list(20000), out_counts(20000) {};
	Graph(){
                deadlock_pro.reserve(100000);
	}
	// add edge n1 -> n2
	void add( std::string n1 , std::string n2) {
		long node1, node2;
		node1 = w2i.get(n1);
		node2 = w2i.get(n2);
		all_nodes.insert(n1);
		all_nodes.insert(n2);
		adj_list[node2].push_back(node1);
		out_counts[node1]++;
	}

	// run cycle detection
	bool deadlock() {	
		auto out_edges = out_counts;
		vector<long> nonout_nodes;
		nonout_nodes.reserve(100000);
	
		for(auto &n : all_nodes)
			if(out_edges[w2i.get(n)] == 0)
				nonout_nodes.push_back(w2i.get(n));
		
		while(!nonout_nodes.empty()){
			long node = nonout_nodes.back();
			nonout_nodes.pop_back();
			for(auto &n : adj_list[node]){
				--out_edges[n];

				if(out_edges[n] == 0)
					nonout_nodes.push_back(n);
			}
		}

		for(const auto &n : all_nodes)
			if(out_edges[w2i.get(n)] >=  1)
				if(n[0] == 'p')
					deadloc_pro.push_back(n);
		
		if(!deadloc_pro.empty())
			return true;
		
		return false;
	}

	// report cycle
	void report() {
        if(!deadloc_pro.empty()){
            printf("Deadlock on edge: %s\n", cur_line.c_str());
            printf("Deadlocked processes: ");

            for(long i = 0; i < deadloc_pro.size(); ++i){
                if(i == deadloc_pro.size() - 1)
                    printf("%s\n", &((deadloc_pro[i].c_str())[2]));
                else
                    printf("%s, ", &((deadloc_pro[i].c_str())[2]));
            }

            return ;
        }

        printf("No deadlock.\n");
        return ;
	}
};

int
main()
{

	Graph g;
	while(true) {
		std::string pname, rname;
		Edge e;
		if( ! parseLine(pname, rname, e)) break;

		if( e == Edge::Request){
			g.add( "p_" + pname, "r_" + rname);
            g.cur_line = pname + " -> " + rname;
        }
		else{
			g.add( "r_" + rname, "p_" + pname);
            g.cur_line = pname + " <- " + rname;
        }
		if( g.deadlock()) break;	
	}

	g.report();

	return 0;
}

