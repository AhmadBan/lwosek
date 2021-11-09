
#ifndef OSEK_QUEUE_H
#define OSEK_QUEUE_H
#include "common.h"
#include "osek_task.h"
#include "osek_config.h"

struct OsekQueue_t
{
	uint32_t *sp;
	Task_t **tasks;
	int prio;
	int8_t head; /*< Points to the beginning of the queue storage area. */
	int8_t tail;
	uint8_t queue_size;
	uint8_t max_queue_Size;
	/*< Points to the current location of queue stack */
};

#define POSEK_QUEUE_CREATE(prio, queue_size, stk_size) extern OsekQueue_t Q##prio;
#include "osek_queue_def.h"
#undef POSEK_QUEUE_CREATE
enum
{
#define POSEK_QUEUE_CREATE(prio, queue_size, stk_size) IDQ##prio,
#include "osek_queue_def.h"
#undef POSEK_QUEUE_CREATE
	MAX_QUEUES
};

#endif
