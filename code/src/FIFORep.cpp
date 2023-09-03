/* @Author
Student Name: Emre Çamlıca
Student ID : 150210071
Date: 27.10.2022
*/
#include <iostream> 
#include <stdio.h>
#include <string.h>

#include "ProcessRep.h"
#include "FIFORep.h"

using namespace std;

FIFORep::FIFORep()
{
    this -> mpHead = NULL;
    this -> mpTail = NULL;
}

FIFORep::FIFORep(ProcessRep* head)  // Initializes both the head and the tail since the first
{                                   // element is both the head and the tail.
    this -> mpHead = head;
    this -> mpTail = head;
}


FIFORep::~FIFORep()
{   
    if(this -> mpHead != NULL){                                  // Checks if the FIFO is empty. If not, deletes it starting from
        ProcessRep *temp = this -> mpHead;
        while(this -> mpHead -> getNext() != NULL){              // the head. At the end, deletes the last remaining element. 
            temp = this -> mpHead;
            this -> mpHead -> setNext(this -> mpHead -> getNext());
            temp = NULL;
        }
        this -> mpHead = NULL;
        this -> mpTail = NULL;
        delete this -> mpHead;
        delete this -> mpTail;
        delete temp;
    }
    delete this -> mpHead;
    delete this -> mpTail;
}

void FIFORep::setHead(ProcessRep* head)
{
    this -> mpHead = head;
}

ProcessRep* FIFORep::getHead()
{
    return this -> mpHead;
}

void FIFORep::setTail(ProcessRep* tail)
{
    this -> mpTail = tail;
}

ProcessRep* FIFORep::getTail()
{
    return this -> mpTail;
}

void FIFORep::queue(ProcessRep* p)
{
    /*
        The function add a process to the tail of the queue.
    */
   if(mpTail == NULL){  // Required to prevent segmentation faults.
   setTail(p);          // Sets both head and tail to p since there are no other elements.
   setHead(p);
   return;
   }

   mpTail -> setNext(p); // Adds the element to tail.
   mpTail = p;           // Since p is the last element, now it is the tail.
}

ProcessRep* FIFORep::dequeue()
{   
    /*
        The function remove a process from the head of the queue and returns it.
    */   
    if(mpHead != NULL){         // Required to prevent segmentation faults.
        ProcessRep *temp = mpHead;
        mpHead = mpHead -> getNext();    
        temp -> setNext(NULL);  // Required since we return temp and it will be used in other FIFO's.
        if(mpHead == NULL)      // If the head is NULL, tail should also be NULL, since there can   
        mpTail = NULL;          // not be an end if there is no beginning.
        return temp;            
    }
   return NULL;         
}

ProcessRep* FIFORep::searchID(int id)
{   
    /*
        The function search a process by id and returns it if it exist otherwise returns NULL.
    */ 
    ProcessRep *temp = mpHead;
    while(temp != NULL){        // Search until reaching next of the tail, which is NULL.
        if(temp -> getProcessID() == id)
        return temp;
        temp = temp -> getNext();
    }
    delete temp;
    return NULL;                
}

void FIFORep::printFIFO()
{
    /*
        The function prints the proceses in the queue starting from head to tail.
    */
    ProcessRep *temp = mpHead;
    while(temp != NULL){        // Print until reaching next of the tail, which is NULL.
        cout << temp -> getProcessType() << temp -> getProcessID() << " " << temp -> startTime << " " << temp -> endTime << " ";
        temp = temp -> getNext();
    }
    delete temp;                
}