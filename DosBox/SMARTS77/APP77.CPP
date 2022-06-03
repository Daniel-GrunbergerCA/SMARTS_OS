
#include "smarts77.h"
/*
Zuf Newfield
Daniel Grunberger
*/
#define NUM_OF_LOOPS_A 5
#define NUM_OF_LOOPS_B 10
#define NUM_OF_LOOPS_C 15

// mutexx - priority inversion
// ioMutex - cin and cout
// Mutexy - new tasks (e, d, f)
Mutex ioMutex(0), Mutexx(0), Mutexy(0);

void a()
{
	ioMutex.Acquire();
	cout << "\n *************   A Start    *********************";
	ioMutex.Release();
	for (int j = 0; j < NUM_OF_LOOPS_A; j++)
	{
		for (long i = 0; i < 200000; i++);
		ioMutex.Acquire();
		cout << "A";
		ioMutex.Release();
	}
	ioMutex.Acquire();
	cout << "\n *************   A Finish   *********************";
	ioMutex.Release();
}

void b()
{
	ioMutex.Acquire();
	cout << "\n *************   B Start    *********************";
	ioMutex.Release();
	for (int j = 0; j < NUM_OF_LOOPS_B; j++)
	{
		for (long i = 0; i < 200000; i++);
		ioMutex.Acquire();
		cout << "B";
		ioMutex.Release();
	}
	ioMutex.Acquire();
	cout << "\n *************   B Finish   *********************";
	ioMutex.Release();
}

void c()
{
	ioMutex.Acquire();
	cout << "\n *************   C Start    *********************";
	ioMutex.Release();
	for (int j = 0; j < NUM_OF_LOOPS_C; j++)
	{
		for (long i = 0; i < 200000; i++);
		ioMutex.Acquire();
		cout << "C";
		ioMutex.Release();
	}
	ioMutex.Acquire();
	cout << "\n *************   C Finish   *********************";
	ioMutex.Release();
}

void main()
{
	clrscr();
	SMARTS.externalFunctions(timerInterruptHandler, scheduler, myTaskEnd, edf);
	SMARTS.declareTask(a, 'A', 100, 5);
	SMARTS.declareTask(b, 'B', 70, 4);
	SMARTS.declareTask(c, 'C', 50, 3);
	SMARTS.runTheTasks();
}

/*
The define statments are on the top of this file

Works for edf and for round robin:

	#define NUM_OF_LOOPS_A 5
	#define NUM_OF_LOOPS_B 10
	#define NUM_OF_LOOPS_C 15
    SMARTS.declareTask(a, 'A', 100, 5); 
	SMARTS.declareTask(b, 'B', 70, 4);
	SMARTS.declareTask(c, 'C', 50, 3);


---------------------------------------------------------------
Works for edf and does NOT work for round robin:

	#define NUM_OF_LOOPS_A 16
	#define NUM_OF_LOOPS_B 22
	#define NUM_OF_LOOPS_C 33
	SMARTS.declareTask(a, 'A', 100, 5);
	SMARTS.declareTask(b, 'B', 70, 4);
	SMARTS.declareTask(c, 'C', 50, 3);


 ---------------------------------------------------------------
 Does NOT work for edf and does NOT work for round robin:

	#define NUM_OF_LOOPS_A 33
	#define NUM_OF_LOOPS_B 33
	#define NUM_OF_LOOPS_C 33
	SMARTS.declareTask(a, 'A', 50, 5); 
	SMARTS.declareTask(b, 'B', 70, 4);
	SMARTS.declareTask(c, 'C', 100, 3);

*/



