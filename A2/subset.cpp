/**********************************************
 * Last Name:   Yi
 * First Name:  Yuzhe
 * Student ID:  30105971
 * Course:      CPSC 457
 * Tutorial:    05
 * Assignment:  2
 *
 * File name: subset.cpp
 *********************************************/

#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <pthread.h>

std::vector<long> a;
long counts[32];            //because there are supposed to be 32 threads at most described in the question

struct arguments{
    long tid, from, to;
}


//using for debug
void print_sol(long comb){
    return ;                //commet to see results

    for(size_t i = 0; i < a.size(); ++i){
        if(comb & 1)
            printf("%d", a[i]);
        
        comb /= 2;
    }

    printf("\n");

    return ;
}

void test(long comb, long tid)
{
  long long sum = 0;
  long bits = comb;
  for(size_t i = 0 ; i < a.size() ; i ++ ) {
    if( bits & 1) // check lowest bit
      sum += a[i];
    bits >>= 1; // shift bits to the right
  }
  if( sum == 0) {
    print_sol(comb);
    ++counts[tid];
  }
}

// test all combinations using threads
void test_range(void *args)
{
  for( long i = args.from ; i < args.to ; i ++)
    test(i, args.tid);
}

int main(int argc, char ** argv){
    if(argc > 2){
        printf("Error: expected at most 1 command line argument.\n");
        return -1;
    }

    //convert argv[1] to 'nThread', report error if failed
    char *end = 0;
    errorn = 0;
    long nThreads = strol(arg[1], &end, 10);
    if(*end ! = 0 || errno != 0){
        printf("Expected integer, but you gave me '%s'\n", argv[1]);
        return -1;
    }

    //read all integers one by one abd put them into the array
    while(1){
        long n;
        if(1 != scanf("%ld, &n"))
            break;
        a.push_back(n);
    }

    //debug message
    printf("Using %ld thread(s) on %lu numbers.\n", nThreads, a.size());

    for(int i = 0; i < nThreads; ++i){
        int step = a.size() / nThreads;
        if(step = 0)
            step = 1;
        if()
    }
}