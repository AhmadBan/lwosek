#ifndef OSEK_TYPES_H
#define OSEK_TYPES_H
#include "common.h"
#define TASK(name)   void OSEK_TASK_ ## name(void)
#define DeclareTask(name)   extern void OSEK_TASK_ ## name(void);
#define NOT_USED 0
typedef void (*OsekTask_t)();
typedef struct OsekQueue_t OsekQueue_t;
typedef enum{
	E_OK=0,
	E_OS_ACCESS  = 1, 
  E_OS_CALLEVEL  = 2, 
  E_OS_ID  = 3, 
  E_OS_LIMIT  = 4, 
  E_OS_NOFUNC  = 5, 
  E_OS_RESOURCE  = 6, 
  E_OS_STATE  = 7, 
  E_OS_VALUE  = 8,
	E_OS_LOSTCTRL=9
}StatusType;

typedef enum{
RUNNING=0,  //  Constant of data type TaskStateType for task state running. 
WAITING,  //•  Constant of data type TaskStateType for task state waiting. 
READY,   //•  Constant of data type TaskStateType for task state ready. 
SUSPENDED,  //•  Constant of data type TaskStateType for task state suspended. 

}TaskStateType;

typedef TaskStateType* TaskStateRefType;

typedef enum {
	OSDEFAULTAPPMODE= 1,
}AppModeType;

typedef struct {
	OsekTask_t task; //pointer to function of task
	OsekQueue_t* q;
	uint8_t taskId;
	uint8_t prio; //priority of task
	uint8_t maxAct;//maximum activation
	uint8_t noAct;//number of activation
	TaskStateType state;

}Task_t;
struct OsekQueue_t{
	uint32_t* sp;
	Task_t** tasks;
	int prio;
	int8_t head;					/*< Points to the beginning of the queue storage area. */
	uint8_t queue_size;
	uint8_t max_queue_Size;
					/*< Points to the current location of queue stack */
};



#endif

