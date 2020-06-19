/**********************************************
 * Last Name:   Yi
 * First Name:  Yuzhe
 * Student ID:  30105971
 * Course:      CPSC 457
 * Tutorial:    05
 * Assignment:  3
 *
 * File name:   fatsim.cpp
 *********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <stack>
#include <array>
using namespace std;

long largest = 0;

//recursive DFS takes too much time
/*
void DFS(const vector<vector<long>> fat_table, long b, vector<int> &visited, long length){
	visited[b] = 1;

	if(++length > largest)
		largest = length;
	
	//for every block adjacent to b
	for(auto &u : fat_table[b + 1]){
		if(!visited[u]){
			cout << "from: " << u << endl; 

			DFS(fat_table, u, visited, length);
		}
	}

	return ;
}
*/
//iterative DFS is much more productibe than recursive DFS
void DFS(const vector<long> fat,const vector<vector<long>> fat_table, long b, vector<int> &visited, array<long, 1000000> &length){
	stack<long> stack;
	stack.push(b);

	while(!stack.empty()){
		b = stack.top();
		stack.pop();

		if(visited[b])
			continue;

		visited[b] = 1;
		if(fat[b] != -1)
			length[b] = 1 + length[fat[b]];
		
		if(length[b] > largest)
			largest = length[b];

		for(auto &i : fat_table[b + 1]){
			if(!visited[i])
				stack.push(i);
		}
		
	}
}

void
fatsim(
		long size,
		const vector<long> fat,
		const vector<vector<long>> fat_table,
		long & longest_file_blocks,
		long & unused_blocks
		){
	vector<int> visited(size, 0);
	array<long, 1000000> length {0};

	//for each block pointing to -1
	for(auto &i : fat_table[0]){
		cout << "end block: " << i << endl;
		length[i] = 1;
		DFS(fat, fat_table, i, visited, length);
	}

	long count = 0;
	for(auto &i  : visited)
		if(i == 0)
			++count;

	longest_file_blocks = largest;
	unused_blocks = count;
}

int main( int argc, char **)
{
	if( argc != 1) {
		printf("Thanks for all the command line arguments.\n"
					 "But I don't like any of them... Bye.\n");
		exit(-1);
	}

	vector<long> fat(1000000);
	vector<vector<long>> fat_table(1000001);
	long maxnum = 0;
	long i = 0;

	while ( 1) {
		long num;
		if( 1 != scanf("%ld", & num)) break;
		if( num < -1) {
			printf("I don't like your FAT, it's too negative.\n");
			exit(-1);
		}
		maxnum = max( maxnum, num);
		fat[i] = num;
		fat_table[num + 1].push_back(i);
		
		++i;
	}
	
	if( long(i) <= maxnum) {
		printf("Some of your FATs are too big.\n");
		exit(-1);
	}

	long count1, count2;
	fatsim(i, fat, fat_table, count1, count2);
	printf("blocks in largest file: %ld\n", count1);
	printf("blocks not in any file: %ld\n", count2);

	return 0;
}
