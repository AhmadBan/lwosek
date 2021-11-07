#include "osek_task.h"
#include "osek.h"
/*************define and implement Tasks here**********/
//idle task which is active when no other task is active
TASK(idle){
	ActivateTask(ID_task1);
	ChainTask(ID_idle);
}
// Tasks definitions starts here

TASK(task1) {
		ActivateTask(ID_task2);
		TerminateTask();
}
TASK(task2) {
		ActivateTask(ID_task3);
		TerminateTask();
}
TASK(task3) {
		ActivateTask(ID_task4);
		TerminateTask();
}
TASK(task5) {
		
		TerminateTask();
}
TASK(task4) {
	ActivateTask(ID_task5);
	TerminateTask();
}
// Task definition end here
/***********Do not modify below****************/

Task_t tasks[]={
#define OSEK_TASK_CREATE(name,prio,maxAct) {		\
                    OSEK_TASK_ ## name,	\
										&Q##prio,						\
										ID_##name,								\
										prio ,							\
										maxAct,							\
										0U,									\
										SUSPENDED						\
},

#include "osek_task_def.h"
#undef OSEK_TASK_CREATE
};
