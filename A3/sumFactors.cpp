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
};

int64_t factor;                         //smallest factor for each number
pthread_mutex_t mutex;

void* getSmallestDivisor(void *a){
    struct arguments *arg = ((struct arguments*) a); 
/*
    //less than 2 or prime
    if(arg->num <= 3){
        pthread_mutex_lock(&mutex);
        factor = 0;
        pthread_mutex_unlock(&mutex);

        pthread_exit(0);
    } 
    
    //multiples of 2
    if(arg->num %2 == 0){
        pthread_mutex_lock(&mutex);
        if(factor > 2)
            factor = 2;
        pthread_mutex_unlock(&mutex);

        pthread_exit(0);
    }
    
    //multiples of 3
    if(arg->num % 3 == 0){
        pthread_mutex_lock(&mutex);
        if(factor > 3)
            factor = 3;
        pthread_mutex_unlock(&mutex);

        pthread_exit(0);
    }
*/
    int64_t i = arg->beg;
    while(i <= arg->max){
        if(arg->num % i == 0){
            if(i < factor){
                pthread_mutex_lock(&mutex);
                factor = i;
                pthread_mutex_unlock(&mutex);
            }

            pthread_exit(0);
        }  

        if(arg->num % (i + 2) == 0){
            if(i + 2 < factor){
                pthread_mutex_lock(&mutex);
                factor = i + 2;
                pthread_mutex_unlock(&mutex);
            }

            pthread_exit(0);
        }  

        i += 6;
    }

    //if it's prime
    if(factor == (int64_t)sqrt(arg->num) && arg->num % factor != 0){
        pthread_mutex_lock(&mutex);
        factor = 0;
        pthread_mutex_unlock(&mutex);
    }
    

    pthread_exit(0);
}

int main(int argc, char **argv){
    int nThreads  = 1;

    //invalid arguments
    if(argc != 1 && argc != 2){
        cout << "Usage: " << argv[0] << " [nThreads]" << endl;
        exit(-1);
    }

    if(argc == 2)
        nThreads = atoi(argv[1]);

    if(nThreads < 1 || nThreads > 256){
        cout << "Warning: Bad arguments. 1 <= nThreads <= 256!" << endl;
        exit(-1);
    }

    cout << "Using " << nThreads << " thread" << (nThreads == 1 ? "" : "s") << "." << endl;

    pthread_t threads[nThreads];
    struct arguments args[nThreads];
    int64_t sum = 0;

    while(1){
        //reading inputs
        int64_t num;
        int64_t count = nThreads;

        //if read all inputs
        if(!(cin >> num))
            break;

/*      
        int64_t max = sqrt(num);
        factor = max;

        int64_t step = max / nThreads;
        max = step;
        int64_t beg = 5;
        if(step <= 0)
            step = 1;
        for(long i = 0; i < nThreads; ++i){

            if(max >= sqrt(num))
                max = sqrt(num);
            if(i == nThreads)
                max = sqrt(num);
            
            args[i].num = num;
            args[i].beg = beg;
            cout << "While max :" << max << endl;
            args[i].max = max;

            beg += step;
            max += step;

            pthread_create(&threads[i], NULL, getSmallestDivisor, &args[i]);
        }


        for(long i; i < nThreads; ++i)
            pthread_join(threads[i], NULL);
 */
        factor = sqrt(num);
        if(num <= 3)
            factor = 0;
        else if(num % 2 == 0)
            factor = 2;
        else if (num % 3 == 0)
            factor = 3;
        else{
            int64_t beg = 5;
            int64_t max = sqrt(num);
            int64_t step = max / nThreads;
            if(step <= 1)
                step = 1;
            max = step;
            
            for(int64_t i = 0; i < nThreads; ++i){
                if(max >= (int)sqrt(num))
                    max = sqrt(num);
                if(i == nThreads)
                    max = sqrt(num);
                
                args[i].num = num;
                args[i].beg = beg;
                args[i].max = max;

                beg += step;
                max += step;

                pthread_create(&threads[i], NULL, getSmallestDivisor, &args[i]);
            }

            for(int64_t i = 0; i < nThreads; ++i)
                pthread_join(threads[i], NULL);
        }

        cout << "Num: " << num << " Div: " << factor << endl; 
        sum += factor;
    }

    cout << "Sum of divisors = " << sum << endl;

    return 0;
}























