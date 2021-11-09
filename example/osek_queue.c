#include "osek_types.h"
#include "osek_queue.h"
/***************** Allocate memory for holding Task pointer in a Queue*********************/
#define POSEK_QUEUE_CREATE(prio, queue_size, stk_size) Task_t *queue_Q##prio[queue_size];
#include "osek_queue_def.h"
#undef POSEK_QUEUE_CREATE

/***************** allocate all stack for queues********************************************************/

#define POSEK_QUEUE_CREATE(prio, queue_size, stk_size) uint32_t sp_##prio[stk_size];
#include "osek_queue_def.h"
#undef POSEK_QUEUE_CREATE

/***************** define all queues ********************************************************/
#define POSEK_QUEUE_CREATE(prio, queue_size, stk_size) OsekQueue_t Q##prio = {sp_##prio, queue_Q##prio, prio, 0, 0, 0, queue_size};
#include "osek_queue_def.h"
#undef POSEK_QUEUE_CREATE

/***************** Allocate all stack for queues ********************************************************/

OsekQueue_t *queues[] = {
#define POSEK_QUEUE_CREATE(prio, queue_size, stk_size) &Q##prio,
#include "osek_queue_def.h"
#undef POSEK_QUEUE_CREATE
};
