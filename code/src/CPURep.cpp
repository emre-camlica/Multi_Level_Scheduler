/* @Author
Student Name: Emre Çamlıca
Student ID : 150210071
Date: 27.10.2022
*/
#include <iostream>
#include <stdio.h>
#include <string.h>
// #include "node.h"
#include "ProcessRep.h"
#include "FIFORep.h"
#include "CPURep.h"

using namespace std;

CPURep::CPURep()
{
    this -> mFinishedProcess = new FIFORep();   // Need to allocate memory for the finished process FIFO.
}

CPURep::~CPURep()
{
    if(this -> mFinishedProcess != NULL)        // Deletes the finished process FIFO.
    delete this -> mFinishedProcess;
}

ProcessRep* CPURep::runCPU(ProcessRep* p, int time)
{   
    if(p -> remainingTime == p -> getProcessTime())     // Used when the CPU first runs with p where 
        p -> startTime = time;                          // remaining time = processtime
    p -> remainingTime --;
    if(p -> remainingTime == 0){                        // Checks if p is finished.
        p -> endTime = time + 1;                        // +1 is used as I increased the total time at the end of the Scheduler function.
        this -> mFinishedProcess -> queue(p);           
        return NULL;                                    // If p goes to the Finished Process FIFO, we should return NULL.
    }                                                   
    return p;
}

FIFORep* CPURep::getFinishedProcess()
{
    return this -> mFinishedProcess;
}

void CPURep::setFinishedProcess(FIFORep* finishedProcess)
{
    this -> mFinishedProcess = finishedProcess;
}