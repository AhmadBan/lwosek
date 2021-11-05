
#ifndef OSEK_QUEUE_H
#define OSEK_QUEUE_H
#include "common.h"
#include "osek_task.h"
#include "osek_config.h"


#define POSEK_QUEUE_CREATE(prio,queue_size,stk_size) extern OsekQueue_t Q##prio;
#include "osek_queue_def.h"
#undef POSEK_QUEUE_CREATE
enum{
#define POSEK_QUEUE_CREATE(prio,queue_size,stk_size) IDQ##prio,
#include "osek_queue_def.h"
#undef POSEK_QUEUE_CREATE
MAX_QUEUES
};
#endif
