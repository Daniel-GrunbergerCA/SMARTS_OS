
#include "smarts77.h"
/*
Zuf Newfield
Daniel Grunberger
*/
#define NUM_OF_LOOPS_A 5
#define NUM_OF_LOOPS_B 10
#define NUM_OF_LOOPS_C 15

void a()
{
	SMARTS.contextSwitchOff();
	cout << "\n *************   A Start    *********************";
	SMARTS.contextSwitchOn();
	for (int j = 0; j < NUM_OF_LOOPS_A; j++)
	{
		for (long i = 0; i < 200000; i++);
		SMARTS.contextSwitchOff();
		cout << "A";
		SMARTS.contextSwitchOn();
	}
	SMARTS.contextSwitchOff();
	cout << "\n *************   A Finish   *********************";
	SMARTS.contextSwitchOn();
}

void b()
{
	SMARTS.contextSwitchOff();
	cout << "\n *************   B Start    *********************";
	SMARTS.contextSwitchOn();
	for (int j = 0;  j < NUM_OF_LOOPS_B; j++)
	{
		for (long i = 0; i < 200000; i++);
		SMARTS.contextSwitchOff();
		cout << "B";
		SMARTS.contextSwitchOn();
	}
	SMARTS.contextSwitchOff();
	cout << "\n *************   B Finish   *********************";
	SMARTS.contextSwitchOn();
}

void c()
{
	SMARTS.contextSwitchOff();
	cout << "\n *************   C Start    *********************";
	SMARTS.contextSwitchOn();
	for (int j = 0; j < NUM_OF_LOOPS_C; j++)
	{
		for (long i = 0; i  < 200000; i++);
		SMARTS.contextSwitchOff();
		cout << "C";
		SMARTS.contextSwitchOn();
	}
	SMARTS.contextSwitchOff();
	cout << "\n *************   C Finish   *********************";
	SMARTS.contextSwitchOn();
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



