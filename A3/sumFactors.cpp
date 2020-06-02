/**********************************************
 * Last Name:   Yi
 * First Name:  Yuzhe
 * Student ID:  30105971
 * Course:      CPSC 457
 * Tutorial:    05
 * Assignment:  3
 *
 * File name: sumFactor.cpp
 *********************************************/

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cinttypes>
#include <cmath>
#include <iostream>
#include <pthread.h>

using namespace std;

struct arguments{
    int64_t num, beg, max;
}

int64_t factor;                         //smallest factor for each number
pthread_mutex_t mutex;

void* getSmallestDivisor(void *a){
    struct arguments *arg = ((struct arguments*) a); 

    //less than 2 or prime
    if(arg->num <= 3){
        pthread_mutex_lock(&mutex);
        factor;
        pthread_mutex_unlock(&mutex);

        pthread_exit(0);
    } 
    
    //multiples of 2
    if(arg->num %2 == 0){
        pthread_mutex_lock(&mutex);
        factor = 2;
        pthread_mutex_unlock(&mutex);

        pthread_exit(0);
    }
    
    //multiples of 3
    if(arg->num % 3 == 0){
        pthread_mutex_lock(&mutex);
        factor;
        pthread_mutex_unlock(&mutex);

        pthread_exit(0);
    }

    int64_t i = arg->beg;

    while(i <= arg->max){
        if(n % i == 0){
            pthread_mutex_lock(&mutex)
            if(i < factor)
                factor = i;
            pthread_mutex_unlock(&mutex);

            pthread_exit(0);
        }  

        if(n % (i + 2) == 0){
            pthread_mutex_lock(&mutex);
            if(i < factor)
                factor = i + 2;
            pthread_mutex_unlock(&mutex);

            pthread_exit(0);
        }  

        i += 6;
    }

    ptread_exit(0);
}

int main(int argc, char **argv){
    int nThreads;

    //invalid arguments
    if(argc != 1 && argc != 2){
        cout << "Usage: " << argv[0] << " [nThreads]" << endl;
        exit(-1);
    }

    if(argc == 2)
        nThreads = atoi(argv[i]);

    if(nThreads < 1 || nThreads){
        cout << "Warning: Bad arguments. 1 <= nThreads <= 256!" << endl;
        exit(-1);
    }

    cout << "Using " << nThreads << " thread" << (nThreads == 1 ? "" : "s") << "." << endl;

    ptherad_t threads[nThreads];
    struct arguments args[nThreads];
    int64_t sum = 0;

    while(1){
        //reading inputs
        int64_t num;

        //if read all inputs
        if(!(cin >> num))
            break;
        
        int64_t max = sqrt(num);
        factor = max;
        int64_t step = (max - 4) / nThreads;
        int64_t beg = 5;
        if(step <= 0)
            step = 1;
        for(long i = 0; i < nThreads; ++i){
            max = beg + step - 1;
            
            if(beg >= sqrt(num))
                beg = sqrt(num);
            if(max >= sqrt(num))
                beg = sqrt(num);
            if(i = nThreads)
                max = sqrt(num);
            
            args[i].num = num;
            args[i].beg = beg;
            args[i].max = max;

            beg += step;

            pthread_create(&threads[i], NULL, getSmallestDivisor, &args[i]);
        }

        for(long i; i < nThreads; ++i)
            pthread_join(threads[i], NULL);
        
        sum += factor;
    }

    cout << "Sum of divisors = " << sum << endl;

    return 0;
}























