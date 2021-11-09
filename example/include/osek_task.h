#ifndef OSEK_TASK_H
#define OSEK_TASK_H
#include "common.h"
#include "common.h"
#include "osek_types.h"

#define OSEK_TASK_CREATE(name, prio, maxAct) DeclareTask(name)

#include "osek_task_def.h"
#undef OSEK_TASK_CREATE
typedef enum
{
#define OSEK_TASK_CREATE(name, prio, maxAct) ID_##name,
#include "osek_task_def.h"
#undef OSEK_TASK_CREATE
	MAX_TASK,
	INVALID_TASK
} TaskType;
typedef struct OsekQueue_t OsekQueue_t;
typedef void (*OsekTask_t)(void);
typedef struct
{
	OsekTask_t task; // pointer to function of task
	OsekQueue_t *q;
	TaskType taskId;
	uint8_t prio;	// priority of task
	uint8_t maxAct; // maximum activation
	uint8_t noAct;	// number of activation
	TaskStateType state;

} Task_t;

typedef TaskType *TaskRefType;
extern Task_t tasks[MAX_TASK];
#endif
