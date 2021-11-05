#include "osek_task.h"
#include "osek.h"
/*************define and implement Tasks here**********/
//idle task which is active when no other task is active
TASK(idle){
	ActivateTask(ID_task1);
	ChainTask(ID_idle);
}


TASK(task1) {
		ActivateTask(ID_task2);
}
TASK(task2) {
		ActivateTask(ID_task3);
}
TASK(task3) {
		ActivateTask(ID_task5);
}
TASK(task5) {
		ActivateTask(ID_task4);
}
TASK(task4) {

}

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
