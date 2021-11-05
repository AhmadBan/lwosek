#ifndef OSEK_TASK_H
#define OSEK_TASK_H
#include "common.h"
#include "common.h"
#include "osek_types.h"
#include "osek_queue.h"

#define OSEK_TASK_CREATE(name,prio,maxAct) DeclareTask(name)

#include "osek_task_def.h"
#undef OSEK_TASK_CREATE
typedef enum{
#define OSEK_TASK_CREATE(name,prio,maxAct) ID_##name,
#include "osek_task_def.h"
#undef OSEK_TASK_CREATE
	MAX_TASK,
	INVALID_TASK
}TaskType;
typedef TaskType* TaskRefType;
extern Task_t tasks[MAX_TASK];
#endif
