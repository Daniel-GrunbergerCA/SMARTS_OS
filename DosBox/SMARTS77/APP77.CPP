
#include "smarts77.h"

Mutex ioMutex(1), aMutex(1), bMutex(1);
Event ev_c_to_a, ev_c_to_b, ev_f_to_d, ev_f_to_e;
FILE* out_file;

void a()
{
	ioMutex.acquire();
	cout << "\n *************   A Start    *********************";
	fprintf(out_file, "\n *************   A Start    *********************");
	ioMutex.release();
	for (int j = 0; j < 50; j++)
	{
		for (long i = 0; i < 200000; i++);
		ioMutex.acquire();
		cout << "A";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   A Waiting to event from C  ********************* \n";
	fprintf(out_file, "\n *************A Waiting to event from C *********************\n");
	ioMutex.release();	


	char s;
	ev_c_to_a.wait(s);


	ioMutex.acquire();
	cout << "\n *************   A get event from C  ********************* \n";
	fprintf(out_file, "\n *************   A get event from C  ********************* \n");
	ioMutex.release();

	for (int k = 0; k < 15; k++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "A";
		ioMutex.release();
	}


	aMutex.acquire();
	ioMutex.acquire();
	cout << "\n *************   A acquire the mutex   ********************* \n";
	fprintf(out_file, "\n *************   A acquire the mutex    ********************* \n");
	ioMutex.release();

	for (int n = 0; n < 15; n++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "A";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   A release the mutex   ********************* \n";
	fprintf(out_file, "\n *************   A release the mutex   ********************* \n");
	ioMutex.release();

	aMutex.release();

	ioMutex.acquire();
	cout << "\n *************   A Finish   ********************* \n";
	fprintf(out_file, "\n *************   A Finish   ********************* \n");
	ioMutex.release();

}

void b()
{
	ioMutex.acquire();
	cout << "\n *************   B Start    ********************* \n";
	fprintf(out_file, "\n *************   B Start    ********************* \n");
	ioMutex.release();

	for (int i = 0; i < 50; i++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "B";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   B Waiting to event from C  ********************* \n";
	fprintf(out_file, "\n *************   B Waiting to event from C  ********************* \n");
	ioMutex.release();
	
	char s;
	ev_c_to_b.wait(s);

	for (int k = 0; k < 10; k++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "B";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   B Finish   *********************";
	fprintf(out_file, "\n * ************B Finish * ********************""\n");
	ioMutex.release();
}


void c()
{
	ioMutex.acquire();
	cout << "\n *************   C Start    ********************* \n";
	fprintf(out_file, "\n *************   C Start    ********************* \n");
	ioMutex.release();

	for (int i = 0; i < 15; i++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "C";
		ioMutex.release();
	}

	ioMutex.acquire();

	aMutex.acquire();
	cout << "\n *************   C acquire the mutex  ********************* \n";
	fprintf(out_file, "\n *************   C acquire the mutex  ********************* \n");
	ioMutex.release();

	for (int k = 0; k < 15; k++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "C";
		ioMutex.release();
	}

	ev_c_to_a.send('A', NULL, 0);
	ioMutex.acquire();
	cout << "\n *************   C send event to A  *********************";
	fprintf(out_file, "\n *************   C send event to A   *********************");
	ioMutex.release();

	ev_c_to_b.send('B', NULL, 0);
	ioMutex.acquire();
	cout << "\n *************   C send event to B  *********************";
	fprintf(out_file, "\n *************   C send event to B   *********************");
	ioMutex.release();

	for (int l = 0; l < 15; l++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "C";
		ioMutex.release();
	}

	aMutex.release();
	cout << "\n *************   C release the mutex  ********************* \n";
	fprintf(out_file, "\n *************   C release the mutex  ********************* \n");

	for (int g = 0; g < 15; g++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "C";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   C Finish   ********************* \n";
	fprintf(out_file, "\n *************   C Finish   ********************* \n");
	ioMutex.release();
}

void d()
{
	ioMutex.acquire();
	cout << "\n *************   D Start    *********************";
	fprintf(out_file, "\n *************   D Start    *********************");
	ioMutex.release();
	for (int j = 0; j < 50; j++)
	{
		for (long i = 0; i < 200000; i++);
		ioMutex.acquire();
		cout << "D";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   D Waiting to event from F  ********************* \n";
	fprintf(out_file, "\n *************D Waiting to event from F *********************\n");
	ioMutex.release();


	char s;
	ev_f_to_d.wait(s);


	ioMutex.acquire();
	cout << "\n *************   D get event from F  ********************* \n";
	fprintf(out_file, "\n *************   D get event from F  ********************* \n");
	ioMutex.release();

	for (int k = 0; k < 15; k++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "D";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   D trying to get the mutex   ********************* \n";
	fprintf(out_file, "\n *************   D trying to get the mutex    ********************* \n");
	ioMutex.release();
	
	bMutex.acquire();
	ioMutex.acquire();
	cout << "\n *************   D acquire the mutex   ********************* \n";
	fprintf(out_file, "\n *************   D acquire the mutex    ********************* \n");
	ioMutex.release();

	for (int n = 0; n < 15; n++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "D";
		ioMutex.release();
	}

	bMutex.release();

	ioMutex.acquire();
	cout << "\n *************   D release the mutex   ********************* \n";
	fprintf(out_file, "\n *************   D release the mutex   ********************* \n");
	ioMutex.release();


	ioMutex.acquire();
	cout << "\n *************   D Finish   ********************* \n";
	fprintf(out_file, "\n *************   D Finish   ********************* \n");
	ioMutex.release();

}

void e()
{
	ioMutex.acquire();
	cout << "\n *************   E Start    ********************* \n";
	fprintf(out_file, "\n *************   E Start    ********************* \n");
	ioMutex.release();

	for (int i = 0; i < 50; i++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "E";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   E Waiting to event from F  ********************* \n";
	fprintf(out_file, "\n *************   E Waiting to event from F  ********************* \n");
	ioMutex.release();

	char s;
	ev_f_to_e.wait(s);

	for (int k = 0; k < 10; k++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "E";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   E Finish   *********************";
	fprintf(out_file, "\n * ************E Finish * ********************""\n");
	ioMutex.release();
}


void f()
{
	ioMutex.acquire();
	cout << "\n *************   F Start    ********************* \n";
	fprintf(out_file, "\n *************   F Start    ********************* \n");
	ioMutex.release();

	for (int i = 0; i < 15; i++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "F";
		ioMutex.release();
	}

	bMutex.acquire();
	
	ioMutex.acquire();
	cout << "\n *************   F acquire the mutex  ********************* \n";
	fprintf(out_file, "\n *************   F acquire the mutex  ********************* \n");
	ioMutex.release();

	for (int k = 0; k < 15; k++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "F";
		ioMutex.release();
	}

	ev_f_to_d.send('D', NULL,	0);
	ioMutex.acquire();
	cout << "\n *************   F send event to D  *********************";
	fprintf(out_file, "\n *************   F send event to D  *********************");
	ioMutex.release();

	ev_f_to_e.send('E', NULL, 0);
	ioMutex.acquire();
	cout << "\n *************   F send event to E  *********************";
	fprintf(out_file, "\n *************   F send event to E   *********************");
	ioMutex.release();

	for (int l = 0; l < 15; l++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "F";
		ioMutex.release();
	}

	bMutex.release();

	ioMutex.acquire();
	cout << "\n *************   F release the mutex  ********************* \n";
	fprintf(out_file, "\n *************   F release the mutex  ********************* \n");
	ioMutex.release();

	for (int g = 0; g < 15; g++)
	{
		for (long j = 0; j < 200000; j++);
		ioMutex.acquire();
		cout << "F";
		ioMutex.release();
	}

	ioMutex.acquire();
	cout << "\n *************   F Finish   ********************* \n";
	fprintf(out_file, "\n *************   F Finish   ********************* \n");
	ioMutex.release();
}



void main()
{
	clrscr();
	SMARTS.externalFunctions(timerInterruptHandler, scheduler, myTaskEnd, rms);

	out_file = fopen("6_with_i.txt", "w");
	
	SMARTS.declareTask(a, 'A', 600, 3);
	SMARTS.declareTask(b, 'B', 601, 3);
	SMARTS.declareTask(c, 'C', 602, 3);
	SMARTS.declareTask(d, 'D', 603, 3);
	SMARTS.declareTask(e, 'E', 604, 3);
	SMARTS.declareTask(f, 'F', 605, 3);

	SMARTS.runTheTasks();
}

