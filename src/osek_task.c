#include "osek_task.h"
#include "osek.h"
/*************define and implement Tasks here**********/
// idle task which is active when no other task is active
TASK(idle)
{
	/*******Idle code starts here************/

	/*******Idle code ends here************/
	/***do not modify below code*****/
	ChainTask(ID_idle);
}
// Tasks definitions starts here

// Tasks definition ends here
/***********Do not modify below****************/

Task_t tasks[] = {
#define OSEK_TASK_CREATE(name, prio, maxAct) {                     \
												 OSEK_TASK_##name, \
												 &Q##prio,         \
												 ID_##name,        \
												 prio,             \
												 maxAct,           \
												 0U,               \
												 SUSPENDED},

#include "osek_task_def.h"
#undef OSEK_TASK_CREATE
};
