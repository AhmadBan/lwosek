#include "common.h"
#include "osek_types.h"
#include "osek_queue.h"
/*************define and implement Tasks here**********/
//idle task which is active when no other task is active
TASK(idle){
	
	//chainTask(IDidle);
}


TASK(task1) {
	
}
TASK(task2) {

}
TASK(task3) {

}
TASK(task5) {

}
TASK(task4) {

}

/***********Do not modify below****************/
enum{
#define OSEK_TASK_CREATE(name,prio,maxAct) ID_##name,
#include "osek_task_def.h"
#undef OSEK_TASK_CREATE
									};
Task_t tasks[]={
#define OSEK_TASK_CREATE(name,prio,maxAct) {		\
                    OSEK_TASK_ ## name,	\
										&Q##prio,						\
										ID_##name,								\
										prio ,							\
										maxAct,							\
										0U									\
},
#include "osek_task_def.h"
#undef OSEK_TASK_CREATE
};
