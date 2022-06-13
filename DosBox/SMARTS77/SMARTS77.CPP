/*
Zuf Newfield
Daniel Grunberger
*/
#include "smarts77.h"

/**********    Function     **********/
unsigned getTimerClocks( )
// Gets the remaining clocks of the timer register
{
	unsigned clocks;
		/* latch counter #0 */
	outportb(0x43,0x00);
		/* read counter #0 low byte */
	clocks=inportb(0x40);
		/* read counter #0 high byte */
	clocks += inportb(0x40)<<8;
	return clocks;
}
////////////////////////////////////////////////////
/**********    class body:  Parallelism     **********/
Parallelism::Parallelism()
{
	currentTask = 0;
	sleepTasks  = 0;
	activeTasks = 0;
	totalTasks  = 0;
	deadlock    = false;
	error = false;
	contextSwitchFlag = true;
	endOfTimeSlice = true;
}

void Parallelism::externalFunctions(void interrupt ( *timerInterruptHandler)(...),
				    void far *scheduler, void far *userTaskEnd,
				    int far (*algorithm)( ))
		// Sets the external functions
{
	this->timerInterruptHandler = timerInterruptHandler;
	this->scheduler = scheduler;
	this->userTaskEnd = userTaskEnd;
	this->algorithm = algorithm;
	contextSched.declare(scheduler, userTaskEnd, 'S', 10, 10); // prepare the stack of the scheduler task
	for (int i=MaxStack-1; i >= (MaxStack-14); i--)
		schedCopy[i]=contextSched.stack[i];
}

// We need to decrease remaining cycle time for all tasks
void  Parallelism::decreaseCycleTime(int taskNum)
{
	context[taskNum].remaining_time_for_cycle--;
	if (context[taskNum].remaining_time_for_cycle <= 0)
	{
		// If task daeadline exceeded but is not set to not active
		if (context[taskNum].status != NOT_ACTIVE)
		{
			this->error = 1;
		}
		else {
			reDeclare(taskNum);
		}

	}
}

// Responsible for restoring stack for next task to run and update status
void Parallelism::reDeclare(int taskNum)
{

	context[taskNum].num_of_runs--;
	if (context[taskNum].num_of_runs > 0)
	{
		context[taskNum].remaining_time_for_cycle = context[taskNum].cpuTime;

		// restore stack
		for (int i = MaxStack - 1; i >= (MaxStack - 14); i--)
			context[taskNum].stack[i] = context[taskNum].backupStack[i];

		context[taskNum].stackSeg =  FP_SEG(&context[taskNum].stack[MaxStack - 14]);
		context[taskNum].stackPtr = FP_OFF(&context[taskNum].stack[MaxStack - 14]);

		context[taskNum].status = READY;
	}
}



int Parallelism::declareTask(void far *code, char name, int cpuTime, int num_of_runs)
		// Insert a new task entry in SMARTS context array [ ]
{
	if (totalTasks < MaxTask-1) 
	{
		// Add task to task entries array
		context[totalTasks++].declare(code, userTaskEnd, name, cpuTime, num_of_runs);
		++activeTasks;
		return true;
	}
	else	
		return false;
}

void Parallelism::runTheTasks()
		// Start running all tasks declared in SMARTS.		
{
	context[totalTasks].status = READY;	//for task 'runTheTasks' (IDLE task)
	context[totalTasks].priority = MAXINT;
	context[totalTasks].currentPriority = MAXINT;

	currentTask = totalTasks;

	asm	cli;		// forbids interrupts (clear interrupts) while changing the interrupt vect
		// saves the original BIOS userInt in our variable 'userIntAddress' to be restored at the end
	userIntAddress = getvect(userInt);	// BIOS userInt 0x60  (unused by PC)
				// puts the normal BIOS timerInt into the unused userInt address
	setvect(userInt,getvect(timerInt));	// timerInt 0x08 
	
	// sets our SMARTS external function 'timerInterruptHandler' as the new PC hard interrupt time handler
	setvect(timerInt,timerInterruptHandler);
	asm	sti;	// allows back interrupts (set interrupts)

	    // waits for end of runTheTasks (end of all the tasks)
	while(true)
	{
		if (deadlock)
		{
			textcolor(RED);
			cprintf("\n\n\rExit : deadlock");
			break;
		}
		if (this->error == 1)
		{
			textcolor(RED);
			cprintf("\n\n\rExit : error");
			break;
		}
		//if (activeTasks==0)
		//{
			if (isAllFinished() == 1) {
				cprintf("\n\n\rExit : finish");
				break;
			}
		//}
	}
		// restore the original BIOS 'interrupt vector' at the end before returning to regular DOS
	asm	cli;	// no interrupts
	setvect(timerInt, getvect(userInt));	// restore original BIOS time handler
	setvect(userInt, userIntAddress);	// restore original BIOS userInt
	asm	sti;	// yes interrupts
}
int Parallelism::isAllFinished()
{
	for (int i = 0; i < totalTasks-1; i++)
	{
		if (context[i].num_of_runs > 0)
		{
			return 0;
		}
	}
	return 1;
}

int Parallelism::getError()
{
	return this->error;
}
void Parallelism::callScheduler( )
	// Return the control to the scheduler, this sets ON the software interrupt ProgInt flag
{
	setProgInt( );
	asm int timerInt;
}

void Parallelism::restoreSchedStack( )
	// Restore the scheduler stack
{
	for (int i=MaxStack-1; i >= (MaxStack-14); i--)
		contextSched.stack[i] = schedCopy[i];
}

int Parallelism::getCurrentTask( )
{	
	return currentTask;
}

void Parallelism::setCurrentTask(int taskNum)
	// Sets the next task to be run
{	
	if (taskNum <= totalTasks)
		currentTask = taskNum;
}

int Parallelism::getTotalTasks( )
	// Gets total tasks declared
{	
	return totalTasks;
}

int Parallelism::getDeadlock( )
{	
	return deadlock;
}

void Parallelism::setDeadlock( )
{	
	deadlock = true;
}

int Parallelism::contextSwitchOn( )
				// flag which enables context switch
{
	if (endOfTimeSlice) //is current time slice finished ?
	{	
		endOfTimeSlice = false;
		this->context[currentTask].contextSwitchFlag = true;
		callScheduler();	// return control to the scheduler
		return 1;
	}
	this->context[currentTask].contextSwitchFlag = true;
	return 0;
}

void Parallelism::contextSwitchOff( )
{	
	this->context[currentTask].contextSwitchFlag = false;
}

int Parallelism::getContextSwitch( )
{	
	return this->context[currentTask].contextSwitchFlag;
}

void Parallelism::setProgInt( )
			// flag indicates to the extern function 'timerInterruptHandler' 
			// that this is an internal SMARTS software interrupt call, 
			// and the original BIOS function has no to be called.
{	
	progInt = true;
}

void Parallelism::resetProgInt()
{	
	progInt = false;
}

int Parallelism::getProgInt( )
{
	return progInt;
}

void Parallelism::setEndOfTimeSlice( )
		// flag indicates that when 'context switch' will be enabled, 
		// it must also return the control to the scheduler.
{	
	endOfTimeSlice = true;
}

char Parallelism::getName(int taskNum)	// returns name found or ' ' if not
{	
	return (taskNum <= totalTasks)? context[taskNum].name : ' ';
}

char Parallelism::getCurrentName( )
{	
	return context[currentTask].name;
}

int Parallelism::getRemainingTimeOfTask(int taskNum)
{
	return context[taskNum].remaining_time_for_cycle;
}
int Parallelism::getCurrentPriorityOfTask(int taskNum)
{
	return context[taskNum].currentPriority;
}

void Parallelism::setError()
{
	this->error = 1;
}

taskStatus Parallelism::getStatus(int taskNum)  	
	// returns status or undefined if not found
{
	return (taskNum <= totalTasks)? context[taskNum].status : UNDEFINED;
}

taskStatus Parallelism::getCurrentStatus( )
{	
	return context[currentTask].status;
}

void Parallelism::resume(int taskNum)
{
	if (taskNum < totalTasks)
		context[taskNum].status = READY;
}

void Parallelism::resume(char taskName)
{
	for(int i=0;i<totalTasks;++i)
		if (context[i].name == taskName)
			context[i].status = READY;
}


void Parallelism::setCurrentNotActive()
{	
	context[currentTask].status = NOT_ACTIVE;
	--activeTasks;
}
void Parallelism::suspended()
{	
	context[currentTask].status = SUSPENDED;
	callScheduler();
}

void Parallelism::incrPriority(int taskNum )
{
	if (taskNum < totalTasks)
		context[taskNum].incrPriority();
}
void Parallelism::setOriginalPriority(int taskNum )
{
	if (taskNum < totalTasks)
		context[taskNum].setOriginalPriority();
}

void Parallelism::setCurrentOriginalPriority()
{
	context[currentTask].setOriginalPriority();
}



Event *Parallelism::getExpectedEvent(int taskNum)    
// returns *Event  or  NULL  if not found
{	
	return (taskNum <= totalTasks)? context[taskNum].expectedEvent : NULL;
}

Event *Parallelism::getCurrentExpectedEvent( )
{	
	return context[currentTask].expectedEvent;
}

int Parallelism::getCurrentPriority(int taskNum)
{
	return context[taskNum].currentPriority;
}

void Parallelism::setCurrentExpectedEvent(Event *expectedEvent)
{	
	context[currentTask].expectedEvent = expectedEvent;
}

void Parallelism::sleep(int t)
// Current task sleeps for 't' milliseconds
{
	if (t < MAXINT) 
	{
		context[currentTask].sleepCount = t/55+1;
		context[currentTask].status = SLEEP;
		++sleepTasks;
		callScheduler();		// return control to scheduler
	}
}

void Parallelism::sleepDecr(int taskNum)
{
	if (taskNum < totalTasks)
		context[taskNum].sleepDecr();
}

void Parallelism::getCurrentStack(unsigned &StackSeg, unsigned &StackPtr)
	// Load current task stack pointer
{   
	StackSeg = context[currentTask].stackSeg;
	StackPtr = context[currentTask].stackPtr;
}

void Parallelism::setCurrentStack(unsigned StackSeg, unsigned StackPtr)
	// Save current task stack pointer
{
	context[currentTask].stackSeg = StackSeg;
	context[currentTask].stackPtr = StackPtr;
}

void Parallelism::getSchedStack(unsigned &StackSeg, unsigned &StackPtr)
					// Load scheduler  stack pointer
{   
	StackSeg = contextSched.stackSeg;
	StackPtr = contextSched.stackPtr;
}

/*void Parallelism::handleTimers()
	// handling of the sleep status mode
{
	for (int i = totalTasks - 1; i >= 0; --i)
	{
		if (getStatus(i) == SLEEP)
		{
			sleepDecr(i);
			if (getStatus(i) == READY)
				--sleepTasks;
		}
	}
	for (int j = totalTasks - 1; j >= 0; --j)
	{
		context[j].remaining_time_for_cycle--;
		if (context[j].remaining_time_for_cycle <= 0)
		{
			if (getStatus(j) != NOT_ACTIVE)
			{
				cprintf("error");
				cout << "in hadleTimers: status of task:" <<  j   << "is not NOT_ACTVE\n";
				setError();
			}
			else if (context[j].num_of_runs > 0)
			{
				context[j].redeclare();
			}
		}
	}
}*/

void Parallelism::handleTimers()
// handling of the sleep status mode
{
	int i;

	// It gets here

	for (i = totalTasks - 1; i >= 0; --i)
	{


		if (getStatus(i) == SLEEP)
		{
			sleepDecr(i);
			if (getStatus(i) == READY)
				--sleepTasks;
		}
	}

	for (i = totalTasks - 1; i >= 0; --i)
	{
		decreaseCycleTime(i);
	}

}

void Parallelism::taskEnd( )
	// This function is called after the last operation of a task, instead of function return
{
	SMARTS.setCurrentNotActive();
	SMARTS.callScheduler();	// return the control to the scheduler to run a next task
}

int Parallelism::getPriority(int taskNum)
{
	return context[taskNum].currentPriority;
}

void Parallelism::setPriority(int taskNum, int priority)
{
	context[taskNum].currentPriority = priority;
}

/**********    class body:  Task     **********/
Task::Task()
{
	stack[MaxStack-14]=_BP;
	stack[MaxStack-13]=_DI;
	stack[MaxStack-12]=_SI;
	stack[MaxStack-11]=_DS;
	stack[MaxStack-10]=_ES;
	stack[MaxStack-9]=_DX;
	stack[MaxStack-8]=_CX;
	stack[MaxStack-7]=_BX;
	stack[MaxStack-6]=_AX;
	stackSeg=FP_SEG(&stack[MaxStack-14]);
	stackPtr=FP_OFF(&stack[MaxStack-14]);
	status = NOT_ACTIVE;
	sleepCount = 0;
	currentPriority=priority=0;
	contextSwitchFlag = true;
}
//-----------------------------------------------------
void Task::declare(void far *code, void far *userTaskEnd, char name,int cpuTime, int num_of_runs)
{
	stack[MaxStack-5]=FP_OFF(code);
	stack[MaxStack-4]=FP_SEG(code);
	stack[MaxStack-3]=_FLAGS;
	stack[MaxStack-2]=FP_OFF(userTaskEnd);
	stack[MaxStack-1]=FP_SEG(userTaskEnd);

	for (int i = MaxStack - 1; i >= (MaxStack - 14); i--)
		backupStack[i] = stack[i];

	this->name= name;
	this->cpuTime = cpuTime;
	this->currentPriority = this->priority = cpuTime;
	this->remaining_time_for_cycle = cpuTime; // deadline is cpuTime
	this->num_of_runs = num_of_runs;
	this->contextSwitchFlag = true;
	this->status = READY;
}



//----------------------------------------------------
void Task::incrPriority( )
{
 	--currentPriority;
}
//----------------------------------------------------
void Task::setOriginalPriority( )
{
	currentPriority = priority;
}
void Task::redeclare()
{
	for (int i = MaxStack - 1; i >= (MaxStack - 14); i--)
		this->stack[i] = this->backupStack[i];

	stackSeg = FP_SEG(&stack[MaxStack - 14]);
	stackPtr = FP_OFF(&stack[MaxStack - 14]);
	this->num_of_runs--;
	this->remaining_time_for_cycle = this->remaining_time_for_cycle;
	status = READY;
}
//----------------------------------------------------
void Task::sleepDecr( )
	// Decrements the sleep counter and update the task status accordingly
{	
	if (status==SLEEP)
	{
		if (sleepCount > 0) {
			--sleepCount;
		}
		if (!sleepCount) {
			status = READY;
		}
	}
}

void Mutex::acquire()
{
	SMARTS.contextSwitchOff();
	int currentTask = SMARTS.getCurrentTask();
	if (s == 1 || this->owner == currentTask) {
		s = 0;
	}
	else {
	if (this->inheritance == 1) {
		if (SMARTS.getPriority(owner) > SMARTS.getPriority(currentTask)) {
			SMARTS.setPriority(owner, SMARTS.getPriority(currentTask));
		}
	}

		this->pQueue.push(currentTask);
		SMARTS.suspended();
	}
	owner = currentTask;
	level++;
	SMARTS.contextSwitchOn();
}

void Mutex::release()
{
	SMARTS.contextSwitchOff();
	int currentTask = SMARTS.getCurrentTask();
	if (owner == currentTask) {
		if (--level)
			return;
	else {
		this->owner = -1;
		if (!this->pQueue.isEmpty()) {
			int task = this->pQueue.pop();
			SMARTS.resume(task);
		}
		else {
			s = 1;
		}
	}
	}
	if (this->inheritance == 1) {
		SMARTS.setOriginalPriority(owner);
	}
	SMARTS.contextSwitchOn();
}


Mutex::Mutex(int inheritance)
{
	this->s = 1;
	this->owner = -1;
	this->level = 0;
	this->inheritance = inheritance;
}

//----------------------------------------------------



void PriorityQueue::push(int numberTask)
{ 
	priorityQueue[pointerQueue] = numberTask; // put task in the next empty slot
    pointerQueue++; 

	this->sort(); // sort queue according to priority 
}

//----------------------------------------------------
void PriorityQueue::sort()
{
	// bubble sort
	int min;
	if (this->priorityQueue)
	{
		for (int i = 0; i < this->pointerQueue - 1; i++)
		{
			for (int j = 0; j < this->pointerQueue -i - 1; j++)
			{
				if (SMARTS.getCurrentPriority(priorityQueue[j]) > SMARTS.getCurrentPriority(priorityQueue[j + 1]))
				{
				swap(&this->priorityQueue[j], &this->priorityQueue[j+1]);
				}
			}
		}
	}
}
//----------------------------------------------------
void PriorityQueue::swap(int *task1, int *task2)
{
	int temp = *task1;
	*task1 = *task2;
	*task2 = temp;
}
//----------------------------------------------------
int PriorityQueue::pop()
{
	if (isEmpty())
	{
		return -1;
	}
	return priorityQueue[pointerQueue--];
	
}
//----------------------------------------------------
int PriorityQueue::isEmpty()
{
	if (this->pointerQueue == 0)
	{
		return true;
	}
	return false;
}