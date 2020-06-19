/**********************************************
 * Last Name:   Yi
 * First Name:  Yuzhe
 * Student ID:  30105971
 * Course:      CPSC 457
 * Tutorial:    05
 * Assignment:  3
 *
 * File name:   memsim.cpp
 *********************************************/
#include <stdio.h>
#include <string>
#include <vector>
#include <cassert>
#include <list>
#include <iostream>
#include <iterator>

using namespace std;

typedef vector<string> vs_t;

// split string p_line into a vector of strings (words)
// the delimiters are 1 or more whitespaces
vs_t split( const string & p_line){
	auto line = p_line + " ";
	vs_t res;
	bool in_str = false;
	string curr_word = "";
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

// convert string to long
// if successful, success = True, otherwise success = False
long str2long( const string & s, bool & success){
	char * end = 0;
	errno = 0;
	long res = strtol( s.c_str(), & end, 10);
	if( * end != 0 || errno != 0) {
	success = false;
	return -1;
	}
	success = true;
	return res;
}

// read in a line from stdin and parse it
// parsed results are passed back via parameter references
// returns true on EOF, otherwise false
// aborts program on bad input
bool parse_line( long & n1, long & n2){
	char buff[4096];
	vs_t words;
	static long line_no = 0;

	// read in a line and parse it into words
	while(1) { // loop to skip empty lines
	line_no ++;
	if( ! fgets( buff, sizeof(buff), stdin))
		return true; // eof
	words = split(buff);
	if( words.size() != 0) break; // continue if line not empty
	}

	if( words.size() > 2) {
	printf("Error on line %ld - too many words\n", line_no);
	exit(-1);
	}

	// convert first word into number
	bool ok;
	n1 = str2long( words[0], ok);
	if( ! ok) {
	printf("Error on line %ld - not an integer\n", line_no);
	exit(-1);
	}

	if( n1 < 0) {
	if( words.size() != 1) {
		printf("Error on line %ld - too many numbers\n", line_no);
		exit(-1);
	}
	}
	else {
	// convert 2nd number
	n2 = str2long( words[1].c_str(), ok);
	if( ! ok || n2 < 1) {
		printf("Error on line %ld - 2nd paramter not positive integer\n",
			 line_no);
		exit(-1);
	}
	}
	return false;
};

struct chunk{
	long tag, size;
	
	chunk(long tag, long size) : tag(-1), size(size) {} 
};

// all you need to do is to fill in the implementation of MemSim class
struct MemSim {
	// initializes the memory simulator with a page size
	MemSim(long size){
		chunks.push_front(chunk(-1, size));
		page_size = size;
	}

	// frees all blocks with the given tag and merges adjecent free blocks
	void free( long tag) {
		list<chunk>::iterator node = chunks.begin();
		while(node != chunks.end()){
			if(node->tag == tag)
				node->tag = -1;
			
			if(node->tag == -1)
				if(node != chunks.begin()){
					auto pre = prev(node);
					
					//merge 2 adjacent emtpty chunks
					if(pre->tag == -1){
						node->size += pre->size;
						chunks.erase(pre);
					}
				}
			++node;
		}

	}
	
	// allocates a chunk using best-fit
	// if no suitable chunk found, requests new page(s)
	void alloc( long tag, long size) {
		list<chunk>::iterator min = chunks.end();
		list<chunk>::iterator last = chunks.end();
		list<chunk>::iterator node = chunks.begin();
		

		while(node != chunks.end()){
			if(node->tag == -1){
				if(node->size == size){
					node->tag = tag;

					return ;
				}

				if(node->size > size){
					if(min == chunks.end())
						min = node;
					else if(min->size > node->size)
						min = node;
				}

				last = node;
			}
			
			++node;
		}

		if(min != chunks.end()){
			chunk c(-1, min->size - size);
			min->tag = tag;
			min->size = size;
			auto i = min;
			if(++i != chunks.end()){
				chunks.insert(++min, c);
			}
			else
				chunks.push_back(c);
		}
		else{
			if(last != chunks.end()){
				last->tag = tag;
				size -= last->size;
			}

			//request new pages
			while(size > 0){
				chunk page(-1, page_size);
				++request;

				if(size >= page_size){
					page.tag = tag;
					chunks.push_back(page);
					size -= page_size;
				}
				else{
					chunk new_chunk(-1, page_size - size);
					page.tag = tag;
					page.size = size;
					chunks.push_back(page);
					chunks.push_back(new_chunk);
					size = 0;
				}
			}
		}
	}
	
	// returns statistics about the simulation
	// - number of total page requests
	// - the largest available chunk
	void get_report( long & n_req, long & largest) {
		n_req = request;
		largest = 0;
		list<chunk>::iterator node = chunks.begin();
		list<chunk>::iterator i = chunks.begin();

		while(node != chunks.end()){
			if(node->tag == -1)
				if(node->size > largest)
					largest = node->size;
			
			++node;
		}
	}

 private:
	list<chunk> chunks;
	long page_size;
	long request = 1	;
};

int main(int argc, char **argv) {
	// parse command line arguments
	// ------------------------------
	long page_size = 1000;
	
	if( argc != 2) {
	printf("Assuming page size = %ld.\n", page_size);
	}
	else {
		bool ok;
		page_size = str2long( argv[1], ok);
		if( !ok || page_size < 1) {
			printf("Bad page size '%s'.\n", argv[1]);
			exit(-1);
		}
	}

	// process every line
	// ------------------------------
	MemSim ms(page_size);

	while(true){
		long n1, n2;
		if( parse_line( n1, n2)) 
			break;
		
		if( n1 < 0)
			ms.free( -n1);
		else
			ms.alloc( n1, n2);
	}

	// report results
	// ------------------------------
	long n_page_requests, largest_free_chunk_size;
	ms.get_report( n_page_requests, largest_free_chunk_size);
	printf("pages requested: %ld\n", n_page_requests);
	printf("largest free chunk: %ld\n", largest_free_chunk_size);

	return 0;
}
