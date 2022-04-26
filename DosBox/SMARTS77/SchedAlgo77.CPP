
#include "smarts77.h"
#define MAXINT 2147483647

int roundRobin( )
// Round Robin short-term algorithm 
{
	int count;
	int nextTask = (SMARTS.getCurrentTask()+1)%SMARTS.getTotalTasks( );
	for (count=0;
	     SMARTS.getStatus(nextTask)!=READY && count<SMARTS.getTotalTasks( );
	     count++)
	     nextTask=++nextTask%SMARTS.getTotalTasks( );
	if (count == SMARTS.getTotalTasks( ))	// no found READY task to run
		nextTask = SMARTS.getTotalTasks( );
	return nextTask;
}


int edf()
{
	int count;
	int nextTask = SMARTS.getTotalTasks();
	int minTime = 1000;

	for (count = 0; count < SMARTS.getTotalTasks(); count++) {
		if (SMARTS.getStatus(count) == READY) {
			if (SMARTS.getRemainingTimeOfTask(count) < minTime) {
				minTime = SMARTS.getRemainingTimeOfTask(count);
				nextTask = count;
			}
		}
	}
//	cprintf("%d\n\r", nextTask);
	return nextTask;
}