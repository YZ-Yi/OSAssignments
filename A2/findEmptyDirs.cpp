/**********************************************
 * Last Name:   Yi
 * First Name:  Yuzhe
 * Student ID:  30105971
 * Course:      CPSC 457
 * Tutorial:    05
 * Assignment:  2
 *
 * File name: findEmptyDirs.cpp
 *********************************************/

/*
 **************POSIX Programmer's Manual******************* 
 https://www.man7.org/linux/man-pages/man0/dirent.h.0p.html
 https://www.man7.org/linux/man-pages/man3/readdir.3.html
 https://www.man7.org/linux/man-pages/man3/fdopendir.3p.html  
*/


#include <stdio.h>
#include <string>
#include <vector>
#include <sys/types.h>
#include <dirent.h>

using namespace std;

int main(void){
    vector<string> stack;                   //stack to store directries
    stack.push_back(".");                   //push in ther current directry, which is '.'

    while(!stack.empty()){
        auto dirname = stack.back();
        stack.pop_back();

        DIR *dir = opendir(dirname.c_str());
        if(dir){
            int numOfPath = 0;              //to record the number of files or directories in the curreny directory
            while(1){
                dirent *de = readdir(dir);
                if(!de)
                    break;

                string name = de->d_name;
                if(name == "." || name == "..")
                    continue;

                ++numOfPath;                    //if anything in the current directory
                //if it is not a directory
                if(de->d_type != DT_DIR)
                    continue;
                
                string path = dirname + "/" + de->d_name;
                stack.push_back(path);
            }
            
            //if the directory is empty, print it
            if(numOfPath == 0)
                printf("%s\n", dirname.c_str());

            closedir(dir);
        }
    }

    return 0;
}