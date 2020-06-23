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
#include <set>
#include <unordered_map>
#include <cstdint>

using namespace std;
int64_t count = 0;
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

// convert string to int64_t
// if successful, success = True, otherwise success = False
int64_t str2int64_t( const string & s, bool & success){
	char * end = 0;
	errno = 0;
	int64_t res = strtol( s.c_str(), & end, 10);
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
bool parse_line( int64_t & n1, int64_t & n2){
	char buff[4096];
	vs_t words;
	static int64_t line_no = 0;

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
	n1 = str2int64_t( words[0], ok);
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
	n2 = str2int64_t( words[1].c_str(), ok);
	if( ! ok || n2 < 1) {
		printf("Error on line %ld - 2nd paramter not positive integer\n",
			 line_no);
		exit(-1);
	}
	}
	return false;
};

struct chunk{
	// int64_t tag, size,;
	int64_t addr, tag, size;
	
	chunk(int64_t tag, int64_t size, int64_t addr) : tag(tag), size(size), addr(addr) {} 
};

typedef list<chunk>::iterator ChunkRef;

struct scmp {
  bool operator()( const ChunkRef & c1, const ChunkRef & c2) const {
    if( c1-> size == c2-> size)
      return c1-> addr < c2-> addr;
    else
      return c1-> size < c2-> size;
  }
};


// all you need to do is to fill in the implementation of MemSim class
struct MemSim {
	// initializes the memory simulator with a page size
	MemSim(int64_t size){
		page_size = size;
	}

	// frees all blocks with the given tag and merges adjecent free blocks
	void free( int64_t tag) {
		// cout << "Free tag " << tag << endl;
		for(auto it = tagged_blocks[tag].begin(); it != tagged_blocks[tag].end();){
			(*it)->tag = -1;

			if((*it) != chunks.begin()){
				ChunkRef pre = prev(*it);
				if(pre->tag == -1){

					(*it)->addr = pre->addr;
					(*it)->size += pre->size;
					free_blocks.erase(pre);
					chunks.erase(pre);
				}
			}
			if(next(*it) != chunks.end()){
				ChunkRef ne = next(*it);
				if(ne->tag == -1){
					(*it)->size += ne->size;
					free_blocks.erase(ne);
					chunks.erase(ne);
				}
			}

			free_blocks.insert(*it);
			++it;
		}

		tagged_blocks.erase(tag);
		
	}
	
	// allocates a chunk using best-fit
	// if no suitable chunk found, requests new page(s)
	void alloc( int64_t tag, int64_t size) {
		//request the first page
		if(chunks.empty()){
			chunks.push_front(chunk(-1, page_size, 0));
			++request;
			free_blocks.insert(chunks.begin());
		}

		list<chunk> dummy  {chunk(-1, size, 0)};
		auto sbesti = free_blocks.lower_bound(dummy.begin());
		ChunkRef best = chunks.end();
		if(sbesti != free_blocks.end()){
			best = *sbesti;
		}

		//request new pages
		if(best == chunks.end()){
			if(!free_blocks.empty()){
				// auto last_chunk = *(--free_blocks.end());
				
				
				if(chunks.back().tag == -1){
					ChunkRef last_chunk = --chunks.end();
					size -= last_chunk->size;
				
					last_chunk->tag = tag;

					free_blocks.erase(last_chunk);

					tagged_blocks[tag].push_back(last_chunk);
				}

				
			}

			while(size > 0){
				chunk page(-1, page_size, (int64_t)chunks.back().addr + chunks.back().size);
				// cout << chunks.back().addr << " + " << page_size <<" = " << page.addr << endl;
				++request;

				if(size >= page_size){
					page.tag = tag;
					chunks.push_back(page);
					size -= page_size;
					tagged_blocks[tag].push_back(--chunks.end());
				}
				else{
					page.tag = tag;
					page.size = size;
					chunks.push_back(page);
					tagged_blocks[tag].push_back(--chunks.end());


					chunk new_chunk(-1, page_size - size, (int64_t)chunks.back().addr + chunks.back().size);
					chunks.push_back(new_chunk);
					free_blocks.insert(--chunks.end());
					// cout << "New chunk page addr: " << new_chunk.addr << endl;
					size = 0;

					
				}
			}
		}
		else{
			best->tag = tag;
			if(best->size > size){
				chunk new_chunk(-1, best->size - size, (int64_t)best->addr + size);
				free_blocks.erase(sbesti);
				best->size = size;
				tagged_blocks[tag].push_back(best);
				chunks.insert(next(best), new_chunk);
				free_blocks.insert(next(best));
			}
			else{
				free_blocks.erase(sbesti);
				tagged_blocks[tag].push_back(best);
			}
		}

		
	}
	
	// returns statistics about the simulation
	// - number of total page requests
	// - the largest available chunk
	void get_report( int64_t & n_req, int64_t & largest) {
		n_req = request;
		largest = 0;
		for(auto &node : free_blocks){
			if(node->size > largest)
				largest = node->size;
			// cout << "size:" << node->size << endl;
		}

		
	}

 private:
	list<chunk> chunks;
	set<ChunkRef, scmp> free_blocks;
	unordered_map<int64_t, vector<ChunkRef>> tagged_blocks;
	int64_t page_size;
	int64_t request = 0;
};

int main(int argc, char **argv) {
	// parse command line arguments
	// ------------------------------
	int64_t page_size = 1000;
	
	if( argc != 2) {
	printf("Assuming page size = %ld.\n", page_size);
	}
	else {
		bool ok;
		page_size = str2int64_t( argv[1], ok);
		if( !ok || page_size < 1) {
			printf("Bad page size '%s'.\n", argv[1]);
			exit(-1);
		}
	}

	// process every line
	// ------------------------------
	MemSim ms(page_size);

	while(true){
		int64_t n1, n2;
		if( parse_line( n1, n2)) 
			break;
		
		if( n1 < 0)
			ms.free( -n1);
		else
			ms.alloc( n1, n2);
	}


	// report results
	// ------------------------------
	int64_t n_page_requests, largest_free_chunk_size;
	ms.get_report( n_page_requests, largest_free_chunk_size);
	printf("pages requested: %ld\n", n_page_requests);
	printf("largest free chunk: %ld\n", largest_free_chunk_size);

	return 0;
}
