#include "common.h"
#include "osek.h"
#include "osek_port.h"
#define FIND_READY_QUEUE(x) (31U - __clz(x))

uint64_t timerTick = 0;

Task_t* volatile osek_currTask; //pointer to current task running
Task_t* volatile osek_nextTask; //pointer to next task running


uint8_t DisableAllInterrupts_count=0;
uint8_t SuspendAllInterrupts_count=0;
uint8_t SuspendOSInterrupts_count=0;

uint32_t osek_readySet; /* bitmask of threads that are ready to run */


OsekQueue_t* osek_queues[]= /* array of threads started so far */
{
#define POSEK_QUEUE_CREATE(prio,queue_size,stk_size) &Q##prio,
#include "osek_queue_def.h"
#undef POSEK_QUEUE_CREATE
};
OsekQueue_t* volatile osek_currQueue;
OsekQueue_t* volatile osek_nextQueue;



static StatusType pushToQueue(OsekQueue_t* oq, TaskType taskID) {
	StatusType result=E_OK;
	if (oq->queue_size < oq->max_queue_Size)
	{
		//every task or category 2 isr can call activateTask which fills oq->tasks so it is a shared resource which must be inside critical section
		DisableAllInterrupts();
		oq->tasks[oq->tail] = &tasks[taskID];
		oq->tail++;
		if(oq->tail==oq->max_queue_Size)
			oq->tail=0;
		oq->queue_size++;
		EnableAllInterrupts();
		
	}
	else {
		result = E_OS_LIMIT;
	}
	
	return result;
}

static Task_t* popFromQueue(OsekQueue_t* oq) {
	Task_t* res=NULL;
	if (oq->queue_size > 0)
	{
		DisableAllInterrupts();
		res=oq->tasks[oq->head];
		oq->head++;
		if(oq->head==oq->max_queue_Size)
			oq->head=0;
		oq->queue_size--;
		EnableAllInterrupts();
	}
	
	return res;
}
static Task_t* topFromQueue(OsekQueue_t* oq) {
	Task_t* res = NULL;
	if (oq->queue_size > 0)
	{
		res = oq->tasks[oq->head];
	}
	return res;
}

void dispatcher() {
	OsekQueue_t* oq = osek_currQueue;
	Task_t* taskStruct=NULL;
	while ((taskStruct=topFromQueue(oq))!=NULL) {
		taskStruct->state=RUNNING;
		PreTaskHook();
		taskStruct->task();
		Schedule();
	}
	//if we reach here it means all tasks in oq Queue are executed
	DisableAllInterrupts();
	osek_readySet &= ~(1<<oq->prio);//make appropriate Queue ready bit equal to zero
	EnableAllInterrupts();
	Schedule();
}
void initSP(OsekQueue_t* oq,uint32_t stkSize){
	  uint32_t *sp = (uint32_t *)((((uint32_t)oq->sp + stkSize) / 8) * 8);
    uint32_t *stk_limit;

    /* priority must be in ragne
    * and the priority level must be unused
    */
 
    *(--sp) = (1U << 24);  /* xPSR */
    *(--sp) = (uint32_t)dispatcher; /* PC */
    *(--sp) = 0x0000000EU; /* LR  */
    *(--sp) = 0x0000000CU; /* R12 */
    *(--sp) = 0x00000003U; /* R3  */
    *(--sp) = 0x00000002U; /* R2  */
    *(--sp) = 0x00000001U; /* R1  */
    *(--sp) = 0x00000000U; /* R0  */
    /* additionally, fake registers R4-R11 */
    *(--sp) = 0x0000000BU; /* R11 */
    *(--sp) = 0x0000000AU; /* R10 */
    *(--sp) = 0x00000009U; /* R9 */
    *(--sp) = 0x00000008U; /* R8 */
    *(--sp) = 0x00000007U; /* R7 */
    *(--sp) = 0x00000006U; /* R6 */
    *(--sp) = 0x00000005U; /* R5 */
    *(--sp) = 0x00000004U; /* R4 */

    /* round up the bottom of the stack to the 8-byte boundary */
    stk_limit = (uint32_t *)(((((uint32_t)oq->sp - 1U) / 8) + 1U) * 8);
		
		/* save the top of the stack in the thread's attibute */
    oq->sp = sp;
    
		/* pre-fill the unused part of the stack with 0xDEADBEEF */
    for (sp = sp - 1U; sp >= stk_limit; --sp) {
        *sp = 0xDEADBEEFU;
    }

}
void initQueue(void){
#define POSEK_QUEUE_CREATE(prio,queue_size,stk_size) initSP(&Q##prio,stk_size);		
#include "osek_queue_def.h"
#undef POSEK_QUEUE_CREATE
	//ActivateTask(ID_idle);
}

void InitOS() {
	initQueue();
	osek_readySet |= (1<<tasks[ID_idle].q->prio);
	tasks[ID_idle].state=READY;
	pushToQueue(tasks[ID_idle].q,ID_idle);
	//Interrupt controller configuration
	initConfigureINTC();
}
/*
Parameter (In): 
TaskID Task reference 
Parameter (Out):  none 
Description: The task <TaskID>is transferred from the suspendedstate into 
the readystate. The operating system ensures that the task 
code is being executed from the first statement. 
Particularities:  The service may be called from interrupt level and from task 
level. 
Rescheduling after the call to ActivateTaskdepends on the 
place it is called from (ISR, non preemptable task, preemptable 
task). 
If E_OS_LIMIT is returned the activation is ignored. 
When an extended task is transferred from suspended state 
into ready state all its events are cleared. 
Status: 
Standard:  •  No error, E_OK 
•  Too many task activations of <TaskID>, E_OS_LIMIT 
Extended:  •  Task <TaskID> is invalid, E_OS_ID 
Conformance:  BCC1, BCC2

*/
StatusType ActivateTask ( TaskType TaskID ) {
	StatusType result;
	if(TaskID>=MAX_TASK)
	{
		result=E_OS_LIMIT;
	}
	//readySet is a shared resource which maintain all Queues status so it must be inside a critical section
	DisableAllInterrupts();
	osek_readySet |= (1<<tasks[TaskID].q->prio);
	tasks[TaskID].state=READY;
	result=pushToQueue(tasks[TaskID].q,TaskID);
	EnableAllInterrupts();
	Schedule(); 
	return result;
}

/*
Parameter (In):  none 
Parameter (Out):  none 
Description:  This service causes the termination of the calling task. The 
calling task is transferred from the runningstate into the 
suspendedstate.Particularities:  An internal resource assigned to the calling task is automatically 
released. Other resources occupied by the task shall have been 
released before the call to TerminateTask. If a resource is still 
occupied in standard status the behaviour is undefined. 
If the call was successful, TerminateTaskdoes not return to the 
call level and the status can not be evaluated. 
If the version with extended statusis used, the service returns 
in case of error, and providesa status which can be evaluated 
in the application. 
If the service TerminateTaskis called successfully, it enforces a 
rescheduling. 
Ending a task function without call to TerminateTaskor 
ChainTaskis strictly forbidden and may leave the system in an 
undefined state. 
Status: 
Standard:    No return to call level 
Extended:  •  Task still occupies resources, E_OS_RESOURCE 
•  Call at interrupt level, E_OS_CALLEVEL 
Conformance:  BCC1, BCC2

*/
StatusType TerminateTask ( void ) {
	PostTaskHook();
	DisableAllInterrupts();
	popFromQueue(osek_currQueue)->state=SUSPENDED;
	EnableAllInterrupts();
	Schedule();
	return E_OK;
}
/*
Parameter (In): 
TaskID  Reference to the sequential succeeding task to be activated. 
Parameter (Out):  none 
Description:  This service causes the termination of the calling task. After 
termination of the calling task a succeeding task <TaskID> is 
activated. Using this service, it ensures that the succeeding task 
starts to run at the earliestafter the calling task has been 
terminated. 
Particularities:  If the succeeding task is identical with the current task, this 
does not result in multiple requests. The task is not transferred 
to the suspended state, but will immediately become  ready
again. 
An internal resource assigned tothe calling task is automatically 
released, even if the succeeding task is identical with the current task. Other resources occupied by the calling shall have 
been released before ChainTaskis called. If a resource is still 
occupied in standard status the behaviour is undefined. 
If called successfully, ChainTaskdoes not return to the call level 
and the status can not be evaluated. 
In case of error the service returns to the calling task and 
provides a status which can then be evaluated in the 
application. 
If the service ChainTaskis called successfully, this enforces a 
rescheduling. 
Ending a task function without call to TerminateTaskor 
ChainTaskis strictly forbidden and may leave the system in an 
undefined state. 
If E_OS_LIMIT is returned the activation is ignored. 
When an extended task is transferred from suspended state 
into ready state all its events are cleared. 
Status: 
Standard:  •  No return to call level 
•  Too many task activations of <TaskID>, E_OS_LIMIT 
Extended:  •  Task <TaskID> is invalid, E_OS_ID 
•  Calling task still occupies resources, E_OS_RESOURCE 
•  Call at interrupt level, E_OS_CALLEVEL 
Conformance:  BCC1, BCC2
*/

StatusType ChainTask ( TaskType TaskID ){
	
	TerminateTask();
	return ActivateTask(TaskID);
	
	
}
/*
Parameter (In):  none 
Parameter (Out):  none 
Description:  If a higher-priority task is ready, the internal resource of the task 
is released, the currenttask is put into the  readystate, its 
context is saved and the higher-priority task is executed. 
Otherwise the calling task is continued. 
Particularities:  Rescheduling only takes place if the task an internal resource 
is assigned to the calling task during system generation. For 
these tasks, Scheduleenables a processor assignment to other 
tasks with lower or equal priority than the ceiling priority of the 
internal resource and higher priority than the priority of the 
calling task in application-specific locations. When returning 
from Schedule, the internal resource has been taken again. 
This service has no influence on tasks with no internal resource 
assigned (preemptable tasks). 
Status: 
Standard:  •  No error, E_OK 
Extended:  •  Call at interrupt level, E_OS_CALLEVEL 
•  Calling task occupies resources, E_OS_RESOURCE 
Conformance:  BCC1, BCC2 */
StatusType Schedule ( void ){

	osek_nextQueue = osek_queues[FIND_READY_QUEUE(osek_readySet)];
	//check osek_nextQueue against null but it adds overhead on schedule which has a a high recurrence in system

	if(osek_nextQueue!=osek_currQueue){
		activateCSInt();
	}
	return E_OK;
}



/*
Parameter (In):  none 
Parameter (Out): 
TaskID  Reference to the task which is currently running
Description:  GetTaskID returns the information about the TaskID of the task 
which is currently running. 
Particularities:  Allowed on task level, ISR level and in several hook routines 
(see Figure 12-1). 
This service is intended to be used by library functions and 
hook routines. 
If <TaskID> can’t be evaluated (no task currently running), the 
service returns INVALID_TASK as TaskType. 
Status: 
Standard:  •  No error, E_OK 
Extended:  •  No error, E_OK 
Conformance:  BCC1, BCC2
*/
StatusType GetTaskID ( TaskRefType TaskID ) {
	*TaskID=INVALID_TASK;
	Task_t* task=topFromQueue(osek_currQueue);
	
	if(task->state==RUNNING){
		*TaskID=task->taskId;

	}

		return E_OK;
}

/*
Parameter (In): 
TaskID Task reference 
Parameter (Out): 
State  Reference to the state of the task <TaskID>
Description:  Returns the state of a task (running, ready, waiting, suspended) 
at the time of calling GetTaskState. 
Particularities:  The service may be called from interrupt service routines, task 
level, and some hook routines (see Figure 12-1). 
When a call is made from a task in a full preemptive system, the 
result may already be incorrect at the time of evaluation. 
When the service is called for a task, which is activated more 
than once, the state is set to running if any instance of the task 
is running. 
Status: 
Standard:  •  No error, E_OK 
Extended:  •  Task <TaskID> is invalid, E_OS_ID 
Conformance:  BCC1, BCC2 
*/
StatusType GetTaskState ( TaskType TaskID, TaskStateRefType State ) {

	*State=tasks[TaskID].state;
		return E_OK;
}

/*
Parameter (In):  none 
Parameter (Out):  none 
Description:  This service restores the state saved by DisableAllInterrupts. 
Particularities:  The service may be called from an ISR category 1 and category 
2 and from the task level, but not from hook routines. 
This service is a counterpart of DisableAllInterruptsservice, 
which has to be called before, and its aim is the completion of 
the critical section of code. No API service calls are allowed 
within this critical section. 
The implementation should adapt this service to the target 
hardware providing a minimum overhead. Usually, this service 
enables recognition of interrupts by the central processing unit. 
Status: 
Standard:  •none 
Extended:  •none 
Conformance:  BCC1, BCC2
*/
void EnableAllInterrupts( void ){
	DisableAllInterrupts_count--;
	if(DisableAllInterrupts_count==0)
	{
		EnableAllInterrupts_uC();
	}
	
}

/*
Parameter (In):  none 
Parameter (Out):  none 
Description:  This service disables all interrupts for which the hardware 
supports disabling. The state before is saved for the 
EnableAllInterruptscall. 
Particularities:  The service may be called from an ISR category 1 and category 
2 and from the task level, but not from hook routines. 
This service is intended to start a critical section of the code. 
This section shall be finished by calling the EnableAllInterrupts
service. No API service calls are allowed within this critical 
section. 
The implementation should adapt this service to the target 
hardware providing a minimum overhead. Usually, this service 
disables recognition of interrupts by the central processing unit. 
Note that this service does not support nesting. If nesting is 
needed for critical sections e.g. for libraries 
SuspendOSInterrupts/ResumeOSInterruptsor 
SuspendAllInterrupt/ResumeAllInterruptsshould be used. 
Status: 
Standard:  •none 
Extended:  •none 
Conformance:  BCC1, BCC2
*/
void DisableAllInterrupts ( void ) {
	DisableAllInterrupts_count++;
	DisableAllInterrupts_uC();
	
	
}
/*
Parameter (In):  none 
Parameter (Out):  none 
Description:  This service restores the recognition status of all interrupts 
saved by the SuspendAllInterruptsservice. 
Particularities:  The service may be called from an ISR category 1 and category 
2, from alarm-callbacks and from the task level, but not from all 
hook routines. 
This service is the counterpart of SuspendAllInterruptsservice, 
which has to have been called before, and its aim is the 
completion of the critical section of code. No API service calls 
beside  SuspendAllInterrupts/ResumeAllInterruptspairs and 
SuspendOSInterrupts/ResumeOSInterruptspairs are allowed 
within this critical section. 
The implementation should adapt this service to the target 
hardware providing a minimum overhead. 
SuspendAllInterrupts/ResumeAllInterruptscan be nested. In 
case of nesting pairs of the calls SuspendAllInterruptsand 
ResumeAllInterruptsthe interrupt recognition status saved by 
the first call of SuspendAllInterruptsis restored by the last call 
of the ResumeAllInterruptsservice. 
Status: 
Standard:  •none 
Extended:  •none 
Conformance:  BCC1, BCC2
*/
void ResumeAllInterrupts ( void ){
	SuspendAllInterrupts_count--;
	if(SuspendAllInterrupts_count==0)
	{
		ResumeAllInterrupts_uC();
	}
}
/*
Parameter (In):  none 
Parameter (Out):  none 
Description:  This service saves the recognition status ofall interrupts and 
disables all interrupts for  which the hardware supports 
disabling. 
Particularities:  The service may be called from an ISR category 1 and category 
2, from alarm-callbacks and from the task level, but not from all 
hook routines. 
This service is intended to protect a critical section of code from 
interruptions of any kind. Thissection shall be finished by 
calling the ResumeAllInterruptsservice. No API service calls 
beside  SuspendAllInterrupts/ResumeAllInterruptspairs and 
SuspendOSInterrupts/ResumeOSInterruptspairs are allowed 
within this critical section. 
The implementation should adapt this service to the target 
hardware providing a minimum overhead. 
Status: 
Standard:  •none 
Extended:  •none 
Conformance:  BCC1, BCC2
*/
void SuspendAllInterrupts ( void ) {
	SuspendAllInterrupts_count++;
	SuspendAllInterrupts_uC();
}
/*
Parameter (In):  none 
Parameter (Out):  none 
Description:  This service restores the recognition status of interrupts saved 
by the SuspendOSInterruptsservice. 
Particularities:  The service may be called from an ISR category 1 and category 
2 and from the task level, but not from hook routines. 
This service is the counterpart of SuspendOSInterruptsservice, 
which has to have been called before, and its aim is the 
completion of the critical section of code. No API service calls 
beside  SuspendAllInterrupts/ResumeAllInterruptspairs and 
SuspendOSInterrupts/ResumeOSInterruptspairs are allowed 
within this critical section. 
The implementation should adapt this service to the target 
hardware providing a minimum overhead. 
SuspendOSInterrupts/ResumeOSInterruptscan be nested. In 
case of nesting pairs of the calls SuspendOSInterruptsand 
ResumeOSInterruptsthe interrupt recognition status saved by 
the first call of SuspendOSInterruptsis restored by the last call 
of the ResumeOSInterruptsservice. 
Status: 
Standard:  •none 
Extended:  •none 
Conformance:  BCC1, BCC2
*/
void ResumeOSInterrupts ( void ){
	SuspendOSInterrupts_count--;
	if(SuspendOSInterrupts_count==0)
	{
		ResumeOSInterrupts_uC();
	}
}
/*
Parameter (In):  none 
Parameter (Out):  none 
Description:  This service saves the recognition status of interrupts of 
category 2 and disables the recognition of these interrupts. 
Particularities:  The service may be called from an ISR and from the task level, 
but not from hook routines. 
This service is intended to protect a critical section of code. This 
section shall be finished by calling the ResumeOSInterrupts
service. No API service calls beside 
SuspendAllInterrupts/ResumeAllInterruptspairs and 
SuspendOSInterrupts/ResumeOSInterruptspairs are allowed 
within this critical section. 
The implementation should adapt this service to the target 
hardware providing a minimum overhead. 
It is intended only to disable interrupts of category 2. However, 
if this is not possible in an efficient way more interrupts may be 
disabled. 
Status: 
Standard:  •none 
Extended:  •none 
Conformance:  BCC1, BCC2
*/
void SuspendOSInterrupts ( void ) {
	SuspendOSInterrupts_count++;
	SuspendOSInterrupts_uC();
	
}
/*
Parameter (In): 
Error error occurred 
Parameter (Out):  none 
Description:  This hook routine is called by the operating systemat the end of 
a system service which returns StatusType not equal E_OK. It 
is called before returning to the task level. 
This hook routine is called when an alarm expires and an error 
is detected during task activation or event setting. 
The ErrorHook is not called, ifa system service called from 
ErrorHook does not return E_OK asstatus value. Any error by 
calling of system services from the ErrorHook can only be 
detected by evaluating the status value. 
Particularities:  See chapter 11.1 for general description of hook routines. 
Conformance:  BCC1, BCC2
*/
void ErrorHook (StatusType Error ) {
	
}

/*
Parameter (In): none 
Parameter (Out):  none 
Description:  This hook routine is called by the operating system before 
executing a new task, but after the transition of the task to the 
running state (to allow evaluation of the TaskID by GetTaskID). 
Particularities:  See chapter 11.1 for general description of hook routines. 
Conformance:  BCC1, BCC2
*/
void PreTaskHook ( void ){
	
}
/*
Parameter (In): none 
Parameter (Out):  none 
Description:  This hook routine is called by the operating system after 
executing the current task, but before leaving the task's running
state (to allow evaluation of the TaskID by GetTaskID). 
Particularities:  See chapter 11.1 for general description of hook routines. 
Conformance:  BCC1, BCC2
*/
void PostTaskHook ( void ){
	
}
/*
Parameter (In): none 
Parameter (Out):  none 
Description:  This hook routine is called by the operating systemat the end of 
the operating system initialisation and before the scheduler is 
running. At this time the application can initialise device drivers 
etc. 
Particularities:  See chapter 11.1 for general description of hook routines. 
Conformance:  BCC1, BCC2, ECC1, ECC2 
*/
void StartupHook ( void ){
	
}
/*
Parameter (In): 
Error error occurred 
Parameter (Out):  none 
Description:  This hook routine is called by the operating system when the 
OS service ShutdownOShas been called. This routine is called 
during the operating system shut down. 
Particularities:  ShutdownHookis a hook routine for  user defined shutdown 
functionality, see chapter 11.4. 
Conformance:  BCC1, BCC2
*/
void ShutdownHook ( StatusType Error ) {
	
}
/*
Parameter (In): 
Mode application mode 
Parameter (Out):  none 
Description:  The user can call this system service to start the operating 
system in a specific mode, see chapter 5, Application modes. 
Particularities: Only allowed  outside of the operating  system, therefore 
implementation specific restrictions may apply. See also 
chapter 11.3, System start-up,especially with respect to 
systems where OSEK and OSEKtime coexist. This call does not 
need to return. 
Conformance:  BCC1, BCC2 
*/
void StartOS(AppModeType Mode) {
	StartupHook();
	InitOS();
	DisableAllInterrupts();
	Schedule();
	EnableAllInterrupts();

	/* the following code should never execute */
	ErrorHook(E_OS_LOSTCTRL);

	
}
/*
Parameter (In): 
Error error occurred 
Parameter (Out):  none 
Description:  The user can call this system service to abort the overall 
system (e.g. emergency off). The operating system also calls 
this function internally, if it has reached an undefined internal 
state and is no longer ready to run. 
If a ShutdownHook is configured the hook routine 
ShutdownHookis always called (with <Error> as argument) 
before shutting down the operating system. 
If ShutdownHookreturns, further behaviour of ShutdownOS is 
implementation specific. 
In case of a system where OSEK OS and OSEKtime OS 
coexist, ShutdownHook has to return. 
<Error> needs to be a valid error code supported by OSEK OS. 
In case of a system where OSEK OS and OSEKtime OS 
coexist, <Error> might also be a value accepted by OSEKtime 
OS. In this case, if enabled by an OSEKtime configuration 
parameter, OSEKtime OS will be shut down after OSEK OS 
shutdown. 
Particularities:  After this service the operating system is shut down. 
Allowed at task level, ISR level, in ErrorHookand StartupHook, 
and also called internally by the operating system. 
If the operating system calls ShutdownOS it never uses E_OK 
as the passed parameter value. 
Conformance:  BCC1, BCC2 
*/
void ShutdownOS ( StatusType Error ) {
	
	ShutdownHook (  Error );
	while(1);
}
