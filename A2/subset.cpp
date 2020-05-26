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
};


//using for debug
void print_sol(long comb){
    return ;                //commet to see results

    for(size_t i = 0; i < a.size(); ++i){
        if(comb & 1)
            printf("%ld", a[i]);
        
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

    return ;
}

// test all combinations using threads
void *test_range(void *args)
{
    struct arguments *arg = ((struct arguments *) args);

    for(long i = arg->from; i < arg->to; ++i)
        test(i, arg->tid);

    pthread_exit(0);
}

int main(int argc, char ** argv){
    long count = 0;

    if(argc > 2){
        printf("Error: expected at most 1 command line argument.\n");
        return -1;
    }

    //convert argv[1] to 'nThread', report error if failed
    char *end = 0;
    errno = 0;
    long nThreads = strtol(argv[1], &end, 10);
    if(*end != 0 || errno != 0){
        printf("Expected integer, but you gave me '%s'\n", argv[1]);
        return -1;
    }

    if(nThreads < 1 || nThreads > 32){
        printf("Error: the number of threads is out of range\n");
        return -1;
    }

    pthread_t threads[nThreads];

    //read all integers one by one abd put them into the array
    while(1){
        long n;
        if(1 != scanf("%ld", &n))
            break;
        a.push_back(n);
    }

    //debug message
    printf("Using %ld thread(s) on %lu numbers.\n", nThreads, a.size());

    long step = (long(1) << a.size()) / nThreads;
    int isMoreThreads = 0;
    //if there're more threads than the test range
    if(step < 1){
        step = 1;
        isMoreThreads = 1;
    }

    for(int i = 0; i < nThreads; ++i){
        if(isMoreThreads){
            if(i + 2 <= a.size()){
                struct arguments args{i, i + 1, i + 2};

                if(0 != pthread_create(&threads[i], NULL, test_range, &args)){
                    printf("Error: pthread_create failed\n");
                    
                    return -1;
                }
            }
            //may comment this out
            /*
            else{
                struct arguments args{i, 0, 0};

                if(0 != pthread_create(&threads[i], NULL, test_range, &args)){
                    printf("Error: pthread_create failed\n");
                    
                    return -1;
                }
            }
            */
        }
        else{
            //if it's the last thread
            if(i == nThreads - 1){
                struct arguments args{i, i * step + 1, long(1) << a.size()};
             
                if(0 != pthread_create(&threads[i], NULL, test_range, &args)){
                printf("Error: pthread_create failed\n");
                    
                return -1;
                }
            }
            else{
                struct arguments args{i, i * step + 1, (i + 1) * step};
             
                if(0 != pthread_create(&threads[i], NULL, test_range, &args)){
                printf("Error: pthread_create failed\n");
                    
                return -1;
                }
            }
        }
    }

    for(long i = 0; i < nThreads; ++i){
        count += counts[i];
        pthread_join(threads[i], NULL);
    }

    printf("Subsets found: %ld\n", count);

    return 0;
}