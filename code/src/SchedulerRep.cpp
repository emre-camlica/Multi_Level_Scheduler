/* @Author
Student Name: Emre Çamlıca
Student ID : 150210071
Date: 27.10.2022
*/
#include <iostream> 
#include <stdio.h>
#include <string.h>
#include "SchedulerRep.h"

using namespace std;

SchedulerRep::SchedulerRep()
{
    timeSliceCount = 0;
    totalTime = 0;
    this -> mpRunningProcess = NULL;
    this -> pCpuObj = new CPURep();                                              // Initializing the CPU is necessary.
    this -> mpProcessFIFO[0] = new FIFORep();                                    // Initializing each FIFO is necessary.   
    this -> mpProcessFIFO[1] = new FIFORep();
    this -> mpProcessFIFO[2] = new FIFORep();
}
SchedulerRep::~SchedulerRep()
{   
    if(this -> mpRunningProcess != NULL)
    delete this -> mpRunningProcess;
    if(this -> mpProcessFIFO[0] != NULL)
    delete this -> mpProcessFIFO[0];
    if(this -> mpProcessFIFO[1] != NULL)
    delete this -> mpProcessFIFO[1];
    if(this -> mpProcessFIFO[2] != NULL)
    delete this -> mpProcessFIFO[2];
}

FIFORep* SchedulerRep::getProcessFIFO(int index)
{
    return this-> mpProcessFIFO[index];
}

void SchedulerRep::setProcessFIFO(FIFORep* fifo, int index)
{
    this -> mpProcessFIFO[index] = fifo;
}

void SchedulerRep::setRunningProcess(ProcessRep* p)
{
    this -> mpRunningProcess = p;
}

ProcessRep* SchedulerRep::getRunningProcess()
{
    return this -> mpRunningProcess;
}

void SchedulerRep::pushProcess(ProcessRep* p)                               // Pushes a process to its FIFO according to its type.
{
    if(p -> getProcessType() == "A"){
        mpProcessFIFO[0] -> queue(p);
    }
    else if(p -> getProcessType() == "B"){
        mpProcessFIFO[1] -> queue(p);
    }
    else if(p -> getProcessType() == "C"){
        mpProcessFIFO[2] -> queue(p);
    }
}

ProcessRep* SchedulerRep::popProcess()                                      // Dequeues a process from the FIFO with the highest
{                                                                           // priority. If all FIFO's are empty, returns NULL
    if(mpProcessFIFO[0] -> getHead() != NULL ){                             // to keep the program going.
        return mpProcessFIFO[0] -> dequeue();
    }
    else if(mpProcessFIFO[1] -> getHead() != NULL ){
        return mpProcessFIFO[1] -> dequeue();
    }
    else if(mpProcessFIFO[2] -> getHead() != NULL ){
        return mpProcessFIFO[2] -> dequeue();
    }
    else return NULL;
}

bool SchedulerRep::checkTimeSlice()                                         // Returns true if the quantum time is reached for the 
{                                                                           // given process according to its type.
    if (mpRunningProcess == NULL)                                           // Required to prevent segmentation faults. 
    return 0;
    if(mpRunningProcess -> getProcessType() == "A"){
       return timeSliceCount == 2;
    }
    else if(mpRunningProcess -> getProcessType() == "B"){
        return timeSliceCount == 4;
    }
    else if(mpRunningProcess -> getProcessType() == "C"){
        return timeSliceCount == 8;
    } 
}
ProcessRep* SchedulerRep::sendProcessToCPU(ProcessRep* p){                  // Sends the process to CPU, runs the CPU and returns
                                                                            // the output of runCPU function, which is p if not
    return this -> pCpuObj -> runCPU(p, totalTime);                         // finished, NULL if p is finished.
}
void SchedulerRep::schedule(string type, int id, int arrivalTime, int processTime)
{   
    /*  
        The function is used to schedule the processes. If a process is reached the function it will be scheduled by the function
        according to the rules which is defined below.

            1) All process types have their own time slice (quantum). When running a process, If the scheduler reachs the time slice 
                (quantum) of the running process, the process will be preempted and put back to the queue.
            2) if a process that has higher priority comes, the running process will be preempted and put back to the queue.
            3) if a process that has less or same priority with running process comes, the running process will continue to run. 
                The new process will be put back to the queue.
            4) If running process and new process have same priority and they are put back to the queue, the new process will 
                be put back to the queue first.


    */
    ProcessRep *job = new ProcessRep(type, id, arrivalTime, processTime);   // Initializes the job with given parameters.
    pushProcess(job);                                                       // Sends the job to its FIFO.

    if(checkTimeSlice()){                                                   // Checks if current running process has reached
        pushProcess(getRunningProcess());                                   // its quantum time.
        setRunningProcess(NULL);                                            // If so, sends it back to its FIFO and restarts
        timeSliceCount = 0;                                                 // the quantum counter.
    }

    if(getRunningProcess() == NULL){                                        // If there is no current running process, dequeues 
        setRunningProcess(popProcess());                                    // one from the FIFO with the higher priority.

        if(getRunningProcess() == NULL){                                    // This step is used when there is no remaining
            totalTime ++;                                                   // processes waiting for running the CPU,
            return;                                                         // to keep the program going.
        }
    }

    if(getRunningProcess() -> getProcessType() == "A"){                     // Check if the job has higher priority than
        setRunningProcess(sendProcessToCPU(getRunningProcess()));           // the current running process. If so, queue 
    }                                                                       // the current running process, make job    
    else if(getRunningProcess() -> getProcessType() == "B"){                // as the current running process with restartimg 
        if(job -> getProcessType() == "A"){                                 // the quantum time and run the CPU with the job. 
            timeSliceCount = 0;                                             // If not, then run the CPU with the current one.
            pushProcess(getRunningProcess());                               
            setRunningProcess(popProcess());                                // Since A>B>C, logically it is enough to check:
            setRunningProcess(sendProcessToCPU(getRunningProcess()));       // (If B then if A) and (If C then if C' [A or B]).
        }                                                                  
        else{
        setRunningProcess(sendProcessToCPU(getRunningProcess()));
        }
    }
    else if(getRunningProcess() -> getProcessType() == "C"){
    if(job -> getProcessType() == "C"){
       setRunningProcess(sendProcessToCPU(getRunningProcess()));
    }

    else{
            timeSliceCount = 0;
            pushProcess(getRunningProcess());
            setRunningProcess(popProcess());
            setRunningProcess(sendProcessToCPU(getRunningProcess()));
    }
    
    }

    timeSliceCount ++;                                                       // Increases the quantum time after running the CPU. 

    if(getRunningProcess() == NULL){                                         // Checks the condition where the running process
        timeSliceCount = 0;                                                  // is finished. Then restarts the quantum time
        setRunningProcess(popProcess());                                     // with a new process running the CPU from the
    }                                                                        // FIFO with the higher priority.
    totalTime ++;                                                            // Increases the total time at the end.
    return;                                                                  // return is put to indicate the end of the function.
}

void SchedulerRep::schedule(ProcessRep* p)
{   
    /*  
        The function is used to schedule the processes. If a process is reached the function it will be scheduled by the function
        according to the rules which is defined below.

            1) All process types have their own time slice (quantum). When running a process, If the scheduler reachs the time slice 
                (quantum) of the running process, the process will be preempted and put back to the queue.
            2) if a process that has higher priority comes, the running process will be preempted and put back to the queue.
            3) if a process that has less or same priority with running process comes, the running process will continue to run. 
                The new process will be put back to the queue.
            4) If running process and new process have same priority and they are put back to the queue, the new process will 
                be put back to the queue first.


    */
    if(checkTimeSlice()){                                                       // Checks if current running process has reached
        pushProcess(getRunningProcess());                                       // its quantum time.
        setRunningProcess(NULL);                                                // If so, sends it back to its FIFO and restarts
        timeSliceCount = 0;                                                     // the quantum counter.
    }

    if(getRunningProcess() != NULL){                                            // The condition where the current running process is
    setRunningProcess(sendProcessToCPU(getRunningProcess()));                   // not yet finished or the quantum time not yet reached.
    timeSliceCount ++;                                                          // Runs the CPU with it and increases the quantum time.

    if(getRunningProcess() == NULL){                                            // If there is no current running process, dequeues                                
        timeSliceCount = 0;                                                     // one from the FIFO with the higher priority.
        setRunningProcess(popProcess());
    }
    totalTime ++;                                                               // Increases the total time at the end
    return;                                                                     // return is put to end the function.
    }

    if(getRunningProcess() == NULL){                                            // If there is no current running process, dequeues
        timeSliceCount = 0;                                                     // one from the FIFO with the higher priority.
        setRunningProcess(popProcess());

        if(getRunningProcess() == NULL){                                        // This step is used when there is no remaining
        totalTime ++;                                                           // processes waiting for running the CPU,
        return;                                                                 // to keep the program going.
        }

    setRunningProcess(sendProcessToCPU(getRunningProcess()));                   // Runs the CPU.
    timeSliceCount ++;                                                          // Increases the quantum time after running the CPU.

    if(getRunningProcess() == NULL){                                            // If there is no current running process, dequeues
                                                                                // one from the FIFO with the higher priority.
    setRunningProcess(popProcess());
    timeSliceCount = 0;
    }

    totalTime ++;                                                                // Increases the total time at the end.
    return;                                                                      // return is put to indicate the end of the function.
    }
}


