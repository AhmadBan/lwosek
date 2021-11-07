#ifndef OSEK_TYPES_H
#define OSEK_TYPES_H
#include "common.h"
#define TASK(name)   void OSEK_TASK_ ## name(void)
#define DeclareTask(name)   extern void OSEK_TASK_ ## name(void);
#define ISR(name)   void OSEK_ISR_ ## name(void)
#define NOT_USED 0


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





#endif

