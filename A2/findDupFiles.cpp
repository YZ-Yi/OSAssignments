/**********************************************
 * Last Name:   Yi
 * First Name:  Yuzhe
 * Student ID:  30105971
 * Course:      CPSC 457
 * Tutorial:    05
 * Assignment:  2
 *
 * File name: findDupFiles.cpp
 *********************************************/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct results {
    string dig, filename;
    int matchNum;               //-1 - cannot be digested, 0 - no matches
};

//read line from fp
bool get_line (string &line, FILE *fp = stdin){
    line.clear();

    while(1){
        int c = fgetc(fp);
        
        if(c == '\n')
            return true;
        
        if(c == EOF)
            return !line.empty();

        line.push_back(c);
    }
}

//returns SHA256 digest
string digest(const string &fname){
    string cmdline = "sha256sum " + fname + " 2> /dev/null";
    FILE *fp = popen(cmdline.c_str(), "r");

    if(fp == NULL)
        return "";

    string output;
    if(!get_line(output, fp))
        return "";

    string result;
    for(auto c : output)
        if(isspace(c))
            break;
        else
            result.push_back(c);

    return result;
}

int main(void){
    //read a list of filenames from stdin
    vector<string> fnames;
    string line;

    while(1){
        if(!get_line(line))
            break;
        
        fnames.push_back(line);
    }

    if(fnames.size() < 2) {
        printf("I could have worked if you gave me 2+ filenames... :(\n");
        return -1;
    }

    // int noDig = 0;
    vector<results> r, noDigest;

    for(int i = 0; i < fnames.size(); ++i){
        results temp;
        temp.matchNum = 0;
        temp.dig = digest(fnames[i]);
        temp.filename = fnames[i];

        if(temp.dig.empty()){
            temp.matchNum = -1;                 //cannot get digest
            noDigest.push_back(temp);
            ++noDig;
        }
        else
            r.push_back(temp);
    }

    int matches = 0;
  
    for(int i = 0; i < fnames.size() - 1; ++i){
        // printf("name: %s, match: %d, digest: %s\n", r[i].filename.c_str(), r[i].matchNum, r[i].dig.c_str());
        if(r[i].matchNum == 0){                 //if it has a digest and hasn't found matches yet
            for(int j = i + 1; j < fnames.size(); ++j){
                if(r[j].matchNum == 0){         //if it has a digest and hasn't found matches yet
                    if(r[i].dig == r[j].dig){
                        r[i].matchNum = matches + 1;
                        r[j].matchNum = matches + 1;
                    }
                }  
            }
            if(r[i].matchNum > 0)
                ++matches;
        }

        
    }

    for(int i = -1; i <= matches; ++i){
        if(i == -1 && noDig != 0)
            printf("Could not compute digests for files:\n");
        else if(i > 0)
            printf("Match %d:\n", i);

        for(auto &c : r){
            if(c.matchNum == i && i != 0)
                printf("%s\n", c.filename.c_str());
        }
    }

    return 0;
}