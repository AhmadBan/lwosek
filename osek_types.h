#ifndef OSEK_TYPES_H
#define OSEK_TYPES_H
#include "common.h"
#define TASK(name)   void OSEK_TASK_ ## name(void)
#define DeclareTask(name)   extern void OSEK_TASK_ ## name(void)
#define NOT_USED 0
typedef void (*OsekTask_t)();
typedef struct OsekQueue_t OsekQueue_t;

typedef struct {
	OsekTask_t task; //pointer to function of task
	OsekQueue_t* q;
	uint8_t taskId;
	uint8_t prio; //priority of task
	uint8_t maxAct;//maximum activation
	uint8_t noAct;//number of activation

}Task_t;
struct OsekQueue_t{
	Task_t* tasks;
	int prio;
	int8_t head;					/*< Points to the beginning of the queue storage area. */
	uint8_t queue_size;
	uint8_t max_queue_Size;
	uint32_t* sp;				/*< Points to the current location of queue stack */
};


#endif

