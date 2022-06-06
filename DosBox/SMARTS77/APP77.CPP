
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
Event evBToA, evDToC, evBToE, evDToF;

void a()
{
	ioMutex.Acquire();
	cout << "\n *************   A Start    *********************";
	ioMutex.Release();
	for (int j = 0; j < 15; j++)
	{
		for (long i = 0; i < 200000; i++);
		ioMutex.Acquire();
		cout << "A";
		ioMutex.Release();
	}

	ioMutex.acquire();
	cout << "\n *************   A Waiting to event from B  ********************* \n";
	ioMutex.release();


	char s;
	clock_t before = clock();
	evBToA.wait(s);
	clock_t after = clock();

	ioMutex.acquire();
	cout << "\n               ------ A wait " << ((float)(after - before) / 1000000.0F) * 1000 << " milliSeconds ------ \n";

	float num = ((float)(after - before) / 1000000.0F) * 1000;
	char snum[50];
	gcvt(num, 15, snum);

	ioMutex.release();

	ioMutex.acquire();
	cout << "\n *************   A get event from B  ********************* \n";
	fprintf(myOutput, "\n *************   A get event from B   ********************* \n");
	ioMutex.release();

	for (int k = 0; k < 15; k++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "A";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   A acquire the mutex   ********************* \n";
	fprintf(myOutput, "\n *************   A acquire the mutex    ********************* \n");
	ioMutex.release();

	aMutex.acquire();

	for (int n = 0; n < 15; n++)
	{
		for (long j = 0; j < 200000; j++);
		outMutex.acquire();
		cout << "A";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   A release the mutex   ********************* \n";
	ioMutex.release();

	aMutex.release();

	ioMutex.acquire();
	cout << "\n *************   A Finish   ********************* \n";
	ioMutex.release();

}

void b()
{
	ioMutex.acquire();
	cout << "\n *************   B Start    ********************* \n";
	ioMutex.release();

	for (int i = 0; i < 10; i++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "B";
		fprintf(myOutput, "B");
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   B acquire the mutex   ********************* \n";
	ioMutex.release();

	Mutexx.acquire();

	for (int k = 0; k < 10; k++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "B";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   B send event to A   ********************* \n";
	cout << "\n *************   B send event to E   ********************* \n";
	ioMutex.release();

	evBToA.send('A', NULL, 1);
	evBToE.send('E', NULL, 1);

	for (int n = 0; n < 10; n++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "B";
		fprintf(myOutput, "B");
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   B release the mutex   ********************* \n";
	ioMutex.release();

	Mutexx.release();

	for (int l = 0; l < 10; l++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "B";
		fprintf(myOutput, "B");
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   B Finish   ********************* \n";
	fprintf(myOutput, "\n *************   B Finish    ********************* \n");
	ioMutex.release();
}


void c()
{
	ioMutex.acquire();
	cout << "\n *************   C Start    ********************* \n";
	ioMutex.release();

	for (int i = 0; i < 15; i++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "C";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   C Waiting to event from D  ********************* \n";
	ioMutex.release();

	char w;
	clock_t before = clock();
	evDToC.wait(w);
	clock_t after = clock();

	ioMutex.acquire();
	cout << "\n               ------ C wait " << ((float)(after - before) / 1000000.0F) * 1000 << " milliSeconds ------ \n";

	float num = ((float)(after - before) / 1000000.0F) * 1000;
	char snum[50];
	gcvt(num, 15, snum);

	ioMutex.release();

	ioMutex.acquire();
	cout << "\n *************   C get event from D  ********************* \n";
	ioMutex.release();

	for (int k = 0; k < 15; k++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "C";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   C acquire the mutex   ********************* \n";
	ioMutex.release();

	Mutexy.acquire();

	for (int n = 0; n < 15; n++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "C";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   C release the mutex   ********************* \n";
	ioMutex.release();

	Mutexy.release();

	ioMutex.acquire();
	cout << "\n *************   C Finish   ********************* \n";
	ioMutex.release();
}

void d()
{
	ioMutex.acquire();
	cout << "\n *************   D Start    ********************* \n";
	ioMutex.release();

	for (int i = 0; i < 10; i++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "D";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   D acquire the mutex   ********************* \n";
	ioMutex.release();

	Mutexy.acquire();

	for (int k = 0; k < 10; k++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "D";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   D send event to C   ********************* \n";
	cout << "\n *************   D send event to F   ********************* \n";
	ioMutex.release();

	evDToC.send('C', NULL, 1);
	evDToF.send('F', NULL, 1);

	for (int l = 0; l < 10; l++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "D";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   D release the mutex   ********************* \n";
	ioMutex.release();

	Mutexy.release();

	for (int n = 0; n < 10; n++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "D";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   D Finish   ********************* \n";
	ioMutex.release();
}

void e()
{
	ioMutex.acquire();
	cout << "\n *************   E Start    ********************* \n";
	ioMutex.release();

	for (int i = 0; i < 30; i++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "E";
		ioMutex.release();
	}


	ioMutex.acquire();
	cout << "\n *************   E Waiting to event from B  ********************* \n";
	ioMutex.release();

	char w;
	clock_t before = clock();
	evBToE.wait(w);
	clock_t after = clock();

	ioMutex.acquire();
	cout << "\n               ------ E wait " << ((float)(after - before) / 1000000.0F) * 1000 << " milliSeconds ------ \n";

	float num = ((float)(after - before) / 1000000.0F) * 1000;
	char snum[50];
	gcvt(num, 15, snum);

	ioMutex.release();

	ioMutex.acquire();
	cout << "\n *************   E get event from B  ********************* \n";
	ioMutex.release();

	for (int k = 0; k < 5; k++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "E";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   E Finish   ********************* \n";
	ioMutex.release();
}

void f()
{
	ioMutex.acquire();
	cout << "\n *************   F Start    ********************* \n";
	ioMutex.release();

	for (int i = 0; i < 30; i++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "F";
		ioMutex.release();
	}


	ioMutex.acquire();
	cout << "\n *************   F Waiting to event from D  ********************* \n";
	ioMutex.release();

	char w;
	clock_t before = clock();
	evDToF.wait(w);
	clock_t after = clock();

	ioMutex.acquire();
	cout << "\n               ------ F wait " << ((float)(after - before) / 1000000.0F) * 1000 << " milliSeconds ------ \n";

	float num = ((float)(after - before) / 1000000.0F) * 1000;
	char snum[50];
	gcvt(num, 15, snum);

	outMutex.release();

	ioMutex.acquire();
	cout << "\n *************   F get event from B  ********************* \n";
	ioMutex.release();

	for (int k = 0; k < 5; k++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "F";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   F Finish   ********************* \n";
	ioMutex.release();
}

void main()
{
	clrscr();
	SMARTS.externalFunctions(timerInterruptHandler, scheduler, myTaskEnd, edf);
	
	SMARTS.declareTask(a, 'A', 400, 3); //Task 1 = High task from A/B
	SMARTS.declareTask(b, 'B', 404, 3); //Task 2 = Low task from A/B

	SMARTS.declareTask(c, 'C', 406, 3); //Task 3 = High task from C/D
	SMARTS.declareTask(d, 'D', 410, 3); //Task 4 = Low task from C/D

	SMARTS.declareTask(e, 'E', 402, 3); //middle task A and B
	SMARTS.declareTask(f, 'F', 408, 3); //middle task C and D

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



