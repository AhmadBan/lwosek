#ifndef OSEK_H
#define OSEK_H
#include "common.h"
#include "osek_types.h"
#include "osek_task.h"
#include "osek_queue.h"
#include "osek_port.h"
#include "osek.h"

extern Task_t *volatile osek_currTask; // pointer to current task running
extern Task_t *volatile osek_nextTask; // pointer to next task running
extern OsekQueue_t *volatile osek_currQueue;
extern OsekQueue_t *volatile osek_nextQueue;
void InitOS(void);
void dispatcher(void);

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
Standard:  �  No error, E_OK
�  Too many task activations of <TaskID>, E_OS_LIMIT
Extended:  �  Task <TaskID> is invalid, E_OS_ID
Conformance:  BCC1, BCC2

*/
StatusType ActivateTask(TaskType TaskID);

/*
Parameter (In):  none
Parameter (Out):  none
Description:  This service causes the termination of the calling task. The
calling task is transferred from the runningstate into the
suspendedstate.
Particularities:  An internal resource assigned to the calling task is automatically
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
Extended:  �  Task still occupies resources, E_OS_RESOURCE
�  Call at interrupt level, E_OS_CALLEVEL
Conformance:  BCC1, BCC2
*/
StatusType TerminateTask(void);
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
Standard:  �  No return to call level
�  Too many task activations of <TaskID>, E_OS_LIMIT
Extended:  �  Task <TaskID> is invalid, E_OS_ID
�  Calling task still occupies resources, E_OS_RESOURCE
�  Call at interrupt level, E_OS_CALLEVEL
Conformance:  BCC1, BCC2
*/

StatusType ChainTask(TaskType TaskID);
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
Standard:  �  No error, E_OK
Extended:  �  Call at interrupt level, E_OS_CALLEVEL
�  Calling task occupies resources, E_OS_RESOURCE
Conformance:  BCC1, BCC2
*/
StatusType Schedule(void);

/*
Parameter (In):  none
Parameter (Out):
TaskID  Reference to the task which is currently running
Description:  GetTaskIDreturns the information aboutthe TaskID of the task
which is currently running.
Particularities:  Allowed on task level, ISR level and in several hook routines
(see Figure 12-1).
This service is intended to be used by library functions and
hook routines.
If <TaskID> can�t be evaluated (no task currently running), the
service returns INVALID_TASK as TaskType.
Status:
Standard:  �  No error, E_OK
Extended:  �  No error, E_OK
Conformance:  BCC1, BCC2
*/
StatusType GetTaskID(TaskRefType TaskID);

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
than once, the state is set to runningif any instance of the task
is running.
Status:
Standard:  �  No error, E_OK
Extended:  �  Task <TaskID> is invalid, E_OS_ID
Conformance:  BCC1, BCC2
*/
StatusType GetTaskState(TaskType TaskID, TaskStateRefType State);

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
Standard:  �none
Extended:  �none
Conformance:  BCC1, BCC2
*/
void EnableAllInterrupts(void);

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
Standard:  �none
Extended:  �none
Conformance:  BCC1, BCC2
*/
void DisableAllInterrupts(void);

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
Standard:  �none
Extended:  �none
Conformance:  BCC1, BCC2
*/

void ResumeAllInterrupts(void);
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
Standard:  �none
Extended:  �none
Conformance:  BCC1, BCC2
*/
void SuspendAllInterrupts(void);

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
Standard:  �none
Extended:  �none
Conformance:  BCC1, BCC2
*/
void ResumeOSInterrupts(void);
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
Standard:  �none
Extended:  �none
Conformance:  BCC1, BCC2
*/
void SuspendOSInterrupts(void);
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
void ErrorHook(StatusType Error);

/*
Parameter (In): none
Parameter (Out):  none
Description:  This hook routine is called by the operating system before
executing a new task, but after the transition of the task to the
runningstate (to allow evaluation of the TaskID by GetTaskID).
Particularities:  See chapter 11.1 for general description of hook routines.
Conformance:  BCC1, BCC2
*/
void PreTaskHook(void);
/*
Parameter (In): none
Parameter (Out):  none
Description:  This hook routine is called by the operating system after
executing the current task, but before leaving the task's running
state (to allow evaluation of the TaskID by GetTaskID).
Particularities:  See chapter 11.1 for general description of hook routines.
Conformance:  BCC1, BCC2
*/
void PostTaskHook(void);
/*
Parameter (In): none
Parameter (Out):  none
Description:  This hook routine is called by the operating systemat the end of
the operating system initialisation and before the scheduler is
running. At this time the application can initialise device drivers
etc.
Particularities:  See chapter 11.1 for general description of hook routines.
Conformance:  BCC1, BCC2
*/
void StartupHook(void);
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
void ShutdownHook(StatusType Error);
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
void StartOS(AppModeType Mode);
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
void ShutdownOS(StatusType Error);

#endif
