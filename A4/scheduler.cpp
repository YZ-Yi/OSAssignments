/**********************************************
 * Last Name:   Yi
 * First Name:  Yuzhe
 * Student ID:  30105971
 * Course:      CPSC 457
 * Tutorial:    05
 * Assignment:  3
 *
 * File name: scheduler.cpp
 *********************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Process{
    long arrival, burst, id;
    
    //for sorting
    bool operator < (const Process &a) const {
        return burst > a.burst;
    }
};

void roundRobin(vector<Process> p, int slice){
    queue<Process> jobs;
    int remaining = p.size();
    int idealing = 0;
    int i = 0;
    int cpu = 0;
    long current = 0;
    long time = 0;
    int last_id = -1;
    Process cur_job;
    cout << "Seq: ";

    while(1){
        if(remaining == 0)
            break;
        //if process in cpu is done
        if(cpu == 1 && cur_job.burst <= 0){
            --remaining;
            cpu = 0;
            time = 0;
            if(cur_job.id != last_id){
                cout << "P" << cur_job.id << " ";
                last_id = cur_job.id;
            }

            continue;
        }

        //if a new process arriving
        if(p[i].arrival <= current && i < p.size()){
            jobs.push(p[i++]);

            continue;
        }

        //if have been executing slice time
        if(cpu && time == slice){
            cpu = 0;
            time = 0;
            jobs.push(cur_job);
            if(last_id != cur_job.id){
                cout << "P" << cur_job.id << " ";
                last_id = cur_job.id;
            }

            continue;
        }

        //if cpu is idle and RQ not empty
        if(cpu == 0 && !jobs.empty()){
            cpu = 1;
            cur_job = jobs.front();
            jobs.pop();

            continue;
        }

        //execute one burst of job on cpu
        if(cpu){
            --cur_job.burst;
            ++time;
            idealing = 0;
        }
        else{
            if(idealing == 0){
                cout << "- ";
                idealing = 1; 
            }
        }

        ++current;
    }

    cout << endl;
}

void shortestJobFirst(const vector<Process> &p){
    long current = 0;
    int cpu = 0;                    //0 - idel, 1 - in process
    int i = 0;
    int remaining = p.size();
    int ideling = 0;
    Process cur_job;
    priority_queue<Process> jobs;
    cout << "Seq: ";

    while(1){
        if(remaining == 0)
            break;
        
        //if process in cpu is done
        if(cpu == 1 && cur_job.burst <= 0){
            cout << "P" << cur_job.id  << " ";
            --remaining;
            cpu = 0;

            continue;
        }

        //if a new process is arriving
        if(p[i].arrival <= current && i < p.size()){
            jobs.push(p[i++]);

            continue;
        }

        //if cpu is idel and RQ not empty
        if(cpu == 0 && !jobs.empty()){
            cpu = 1;
            cur_job = jobs.top();
            jobs.pop();

            continue;
        }
        
        //execute one burst of the job on cpu
        if(cpu){
            --cur_job.burst;
            ideling = 0;
        }
        else{
            if(ideling == 0){
                cout << "- ";
                ideling = 1;
            }
        }

        ++current;
    }

    cout << endl;
}

int main(int agrv, char **agrc){
    if(agrv < 3 || agrv > 4){
        cout << "Error, command format: " << endl 
            << "./scheduler filename RR slice(> 0)" << endl
            << "or" << endl
            << "./schedule filename SJF" << endl;

        exit(-1);
    }

    string fileName(agrc[1]);
    string sche_type(agrc[2]);
    long slice = -1;
    
    if(agrv > 3){
        if(atoi(agrc[3]) > 0)
            slice = atol(agrc[3]);
        else
        {
            cout << "Error, command format: " << endl 
            << "./scheduler filename RR slice(> 0)" << endl
            << "or" << endl
            << "./schedule filename SJF" << endl;

            exit(-1);
        }
        
    }

    //wrong command line format
    if((agrv == 3 && sche_type == "RR") || (agrv == 4 && sche_type == "SJF") ||(sche_type != "SJF" && sche_type != "RR")){
        cout << "Error, command format: " << endl 
            << "./scheduler filename RR slice(> 0)" << endl
            << "or" << endl
            << "./schedule filename SJF" << endl;

        exit(-1);
    }

    ifstream fin(fileName);
    vector<Process> p;

    //reading from input file
    long n;
    int i = 0;
    while(fin >> n){
        Process pro;
        pro.arrival = n;
        fin >> pro.burst;
        pro.id = i;
        ++i;
        p.push_back(pro);
    }

    if(sche_type == "SJF")
        shortestJobFirst(p);
    else
        roundRobin(p, slice);


    return 0;
}