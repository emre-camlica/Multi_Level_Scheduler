/* @Author
Student Name: Emre Çamlıca
Student ID : 150210071
Date: 27.10.2022
*/
#include <iostream> 
#include <stdio.h>
#include <string>
#include "ProcessRep.h"


using namespace std;

ProcessRep::ProcessRep(string processType, int processID, int deliverTime, int processTime)
{   
    this -> mProcessType = processType;
    this -> mProcessID = processID;
    this -> mDeliverTime = deliverTime;
    this -> mProcessTime = processTime;
    this -> mpNext = NULL;
    this -> remainingTime = this -> mProcessTime;
    this -> startTime = -1;  // Required to pass the test cases.
    this -> endTime = -1;   // Required to pass the test cases.
}

ProcessRep::~ProcessRep()   // Deletes the pointer variable mpNext.
{   
    if(this -> mpNext != NULL)
    delete this -> mpNext;
}

ProcessRep* ProcessRep::getNext()
{   
    return this -> mpNext;
}

void ProcessRep::setNext(ProcessRep *next)
{
    this -> mpNext = next;
}

string ProcessRep::getProcessType()
{
    return this -> mProcessType;
}

void ProcessRep::setProcessType(string processType)
{
    this -> mProcessType = processType;
}

int ProcessRep::getDeliverTime()
{
    return this -> mDeliverTime;
}

void ProcessRep::setDeliverTime(int deliverTime)
{
    this -> mDeliverTime = deliverTime;
}

int ProcessRep::getProcessID()
{
    return this -> mProcessID;
}

void ProcessRep::setProcessID(int processID)
{
    this -> mProcessID = processID;
}

int ProcessRep::getProcessTime()
{
    return this -> mProcessTime;
}

void ProcessRep::setProcessTime(int ProcessTime)
{
    this -> mProcessTime = ProcessTime;
}