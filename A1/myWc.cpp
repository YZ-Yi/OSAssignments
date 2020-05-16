/**********************************************
 * Last Name:   Yi
 * First Name:  Yuzhe
 * Student ID:  30105971
 * Course:      CPSC 457
 * Tutorial:    05
 * Assignment:  1
 *
 * File name: myWc.cpp
 *********************************************/

#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <cstdint>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>

char buffer[256];
int counter = 0;

int my_getchar(int fd);

int main( int argc, char *argv[])
{
  std::string filepath = argv[1];
  int fd = open(filepath.c_str(), O_RDONLY); // input will be standard input

  // our boolean mini-state, true = we are in a middle of a word
  bool inside_word = false;
  // setup 3 counters
  uint64_t word_count = 0;
  uint64_t line_count = 0;
  uint64_t char_count = 0;
  // read file character by character and count words
   while(1) {
    // read in a single character
    int c = my_getchar(fd);
    if( c == EOF) break; // quit loop on EOF
    // count every character
    char_count ++;
    // count new lines
    if( c == '\n') {
      line_count ++;
    }
    // update our state
    if( isspace(c)) {
      inside_word = false;
    }
    else {
      // update word count if starting a new word
      if( ! inside_word) word_count ++;
      inside_word = true;
    }
  }

  // print results
  printf( "%7lu %7lu %7lu\n",
	  line_count,
	  word_count,
	  char_count
	  );

  close(fd);

  return 0;
}

int my_getchar(int fd){
  if(counter == 0 || counter == 256){
    memset(buffer, -1, sizeof(buffer));
    read(fd, buffer, sizeof(buffer));
    counter = 0;
  }

  return buffer[counter++];
}