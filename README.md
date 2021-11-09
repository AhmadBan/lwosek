 lwosek
Light Weight Osek Operating system
# Getting Started

First, you need to define your tasks and charachteristics, for this in file `osek_task.c ` define all tasks and register them into OSEK-OS by declaring every task in file ` osek_task_def.h `. Consider that while declaring the task you have to provide other information like priority and maximum activation. Further, in file ` osek_config.h ` configure os queue size and maximum stack needed for that queue if it is used or else left it ` NOT_USED `

you can also add ` #include "osek.h" ` to each file where you want to use OSEK APIs.

# Supported Features


## API Features
| Services             |           Task           | ISRcat1 | ISRcat2 | ErrorHook | PreTaskHook | PreTaskHook | PostTaskHook | StartupHook | ShutdownHook | alarmcallback |
| :------------------- | :----------------------: | :-----: | :-----: | :-------: | :---------: | :---------: | :----------: | :---------: | :----------: | ------------: |
| ActivateTask         | <ul><li>[x] item1</li><li>[ ] item2</li></ul> |         |  - [x]  |           |             |             |              |             |              |               |
| TerminateTask        |         - [x] P          |         |         |           |             |             |              |             |              |               |
| ChainTask            |          - [x]           |         |         |           |             |             |              |             |              |               |
| Schedule             |          - [x]           |         |         |           |             |             |              |             |              |               |
| GetTaskID            |          - [x]           |         |         |           |             |             |              |             |              |               |
| GetTaskState         |          - [x]           |         |         |           |             |             |              |             |              |               |
| DisableAllInterrupts |          - [x]           |  - [x]  |  - [x]  |           |             |             |              |             |              |               |
| EnableAllInterrupts  |          - [x]           |  - [x]  |  - [x]  |           |             |             |              |             |              |               |
| SuspendAllInterrupts |          - [x]           |  - [x]  |  - [x]  |           |             |             |              |             |              |               |
| ResumeAllInterrupts  |          - [x]           |  - [x]  |  - [x]  |           |             |             |              |             |              |               |
| SuspendOSInterrupts  |          - [x]           |  - [x]  |  - [x]  |           |             |             |              |             |              |               |
| ResumeOSInterrupts   |          - [x]           |  - [x]  |  - [x]  |           |             |             |              |             |              |               |
| GetResource          |                          |         |         |           |             |             |              |             |              |               |
| ReleaseResource      |                          |         |         |           |             |             |              |             |              |               |
| SetEvent             |                          |         |         |           |             |             |              |             |              |               |
| ClearEvent           |                          |         |         |           |             |             |              |             |              |               |
| GetEvent             |                          |         |         |           |             |             |              |             |              |               |
| WaitEvent            |                          |         |         |           |             |             |              |             |              |               |
| GetAlarmBase         |                          |         |         |           |             |             |              |             |              |               |
| GetAlarm             |                          |         |         |           |             |             |              |             |              |               |
| SetRelAlarm          |                          |         |         |           |             |             |              |             |              |               |


- [x]P: Partially supported.
- [x]TerminateTask can only be used in the body of the function Task and must not be used in nested function in the task.
# TODOs
## Parameters of implementation 
Detailed information is required concerning the functionality, performance and memory 
demand. Furthermore the basic conditions to reproduce the measurement of those parameters 
have to be mentioned, e.g. functionality, target CPU, clock speed, bus configuration, wait 
states etc. 
### Functionality
- [x] Maximum number of tasks : No Limitation 
- [x] Maximum number of not suspended tasks : No Limitation 
- [x] Maximum number of priorities: 32 including Idle 
- [x] Number of tasks per priority (for BCC2 and ECC2) : No Limitation 
- [x] Upper limit for number of task activations: "1" for BCC1 and  No Limitation for BCC2
- [] Maximum number of events per task : Not Supported
- [] Limits for the number of alarm objects (per system/ per task) : Not Supported
- [] Limits for the number of nested resources (per system/ per task) : Not Supported
- [x] Lowest priority level used internally by the OS : level 0 used for Idle

### Hardware resources
- [x] RAM and ROM requirement for each of the operating systemcomponents : ???
- [x] Size for each linkable module : ???
- [x] Application dependent RAM and ROM requirements for operating system data (e.g. bytes RAM per task, RAM required per alarm,...) : ???
- [x] Execution context of the operating system(e.g. size of OS internal tables) : ???
- [x] Timer units reserved for the OS : 1 timer
- [x] Interrupts, traps and other hardware resources occupied by the operating system: Depends on Architecture

### Performance
- [x] Total execution time for each service : ???
- [x] OS start-up time (beginning of StartOSuntil execution of first task in standard mode) without invoking hook routines : ??
- [x] Interrupt latency for ISRs of category 1 and 2 : ???
- [x] Task switching times for all types of switching : ???
- [x] Base load of system without applications running : ??? 
All performance figures shall be stated as minimumand maximum(worst case) values. 

