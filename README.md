 lwosek
Light Weight Osek Operating system
# Getting Started

First, you need to define your tasks and charachteristics, for this in file `osek_task.c ` define all tasks and register them into OSEK-OS by declaring every task in file ` osek_task_def.h `. Consider that while declaring the task you have to provide other information like priority and maximum activation. Further, in file ` osek_config.h ` configure os queue size and maximum stack needed for that queue if it is used or else left it ` NOT_USED `

you can also add ` #include "osek.h" ` to each file where you want to use OSEK APIs.

# supported features



| Services             | Task  | ISRcat1 | ISRcat2 | ErrorHook | PreTaskHook | PreTaskHook | PostTaskHook | StartupHook | ShutdownHook | alarmcallback |
| :------------------- | :---: | :-----: | :-----: | :-------: | :---------: | :---------: | :----------: | :---------: | :----------: | ------------: |
| ActivateTask         |  [x]  |         |   [x]   |           |             |             |              |             |              |               |
| TerminateTask        | [x] P |         |         |           |             |             |              |             |              |               |
| ChainTask            |  [x]  |         |         |           |             |             |              |             |              |               |
| Schedule             |  [x]  |         |         |           |             |             |              |             |              |               |
| GetTaskID            |  [x]  |         |         |           |             |             |              |             |              |               |
| GetTaskState         |  [x]  |         |         |           |             |             |              |             |              |               |
| DisableAllInterrupts |  [x]  |   [x]   |   [x]   |           |             |             |              |             |              |               |
| EnableAllInterrupts  |  [x]  |   [x]   |   [x]   |           |             |             |              |             |              |               |
| SuspendAllInterrupts |  [x]  |   [x]   |   [x]   |           |             |             |              |             |              |               |
| ResumeAllInterrupts  |  [x]  |   [x]   |   [x]   |           |             |             |              |             |              |               |
| SuspendOSInterrupts  |  [x]  |   [x]   |   [x]   |           |             |             |              |             |              |               |
| ResumeOSInterrupts   |  [x]  |   [x]   |   [x]   |           |             |             |              |             |              |               |
| GetResource          |       |         |         |           |             |             |              |             |              |               |
| ReleaseResource      |       |         |         |           |             |             |              |             |              |               |
| SetEvent             |       |         |         |           |             |             |              |             |              |               |
| ClearEvent           |       |         |         |           |             |             |              |             |              |               |
| GetEvent             |       |         |         |           |             |             |              |             |              |               |
| WaitEvent            |       |         |         |           |             |             |              |             |              |               |
| GetAlarmBase         |       |         |         |           |             |             |              |             |              |               |
| GetAlarm             |       |         |         |           |             |             |              |             |              |               |
| SetRelAlarm          |       |         |         |           |             |             |              |             |              |               |


* P: Partially supported.

* TerminateTask can only be used in the body of the function Task and must not be used in nested function in the task.
