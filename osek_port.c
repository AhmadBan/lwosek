#include "common.h"
#include "osek_port.h"
//Activate Context Switch Interrupt
void activateCSInt(void) {
	*(uint32_t volatile *)0xE000ED04 = (1U << 28);
	//port_INT_CTRL_REG = port_INT_SET_BIT;
}

__asm void contextSwitchISR(void) {

	IMPORT  osek_currQueue  /* extern variable */
	IMPORT  osek_nextQueue /* extern variable */
	
	CPSID         I
	//OsekQueue_t* oq = osek_nextQueue; //
  LDR           r0,=osek_currQueue
  LDR           r0,[r0,#0x00]
	CBZ           r0,PendSV_restore
	
	PUSH {r4-r11}
	/*OS_curr->sp = sp; */
	LDR           r1, =osek_currQueue
	LDR           r1, [r1, #0x00]
  STR           sp, [r1, #0x00]
PendSV_restore
	/* sp = OS_next->sp; */
	LDR           r1, =osek_nextQueue
	LDR           r1, [r1, #0x00]
	LDR           sp, [r1, #0x00]

//		/* OS_curr = OS_next; */
  LDR           r1, =osek_nextQueue
  LDR           r1, [r1, #0x00]
	LDR           r2, =osek_currQueue
	STR           r1, [r2, #0x00]

//		/* pop registers r4-r11 */
	POP{ r4 - r11 }
//		/* __enable_irq(); */
	CPSIE         I
	
	BX            lr
}

/*
Parameter (In):  none 
Parameter (Out):  none 
Description:  This service restores the state saved by DisableAllInterrupts. 
Particularities:  The service may be called from an ISR category 1 and category 
2 and from the task level, but not from hook routines. 
This service is a counterpart of DisableAllInterruptsservice, 
which has to be called before, and its aim is the completion of 
the critical section of code. No API service calls are allowed 
within this critical section. 
The implementation should adapt this service to the target 
hardware providing a minimum overhead. Usually, this service 
enables recognition of interrupts by the central processing unit. 
Status: 
Standard:  •none 
Extended:  •none 
Conformance:  BCC1, BCC2
*/
void EnableAllInterrupts ( void ){
	
	
}
/*
Parameter (In):  none 
Parameter (Out):  none 
Description:  This service disables all interrupts for which the hardware 
supports disabling. The state before is saved for the 
EnableAllInterruptscall. 
Particularities:  The service may be called from an ISR category 1 and category 
2 and from the task level, but not from hook routines. 
This service is intended to start a critical section of the code. 
This section shall be finished by calling the EnableAllInterrupts
service. No API service calls are allowed within this critical 
section. 
The implementation should adapt this service to the target 
hardware providing a minimum overhead. Usually, this service 
disables recognition of interrupts by the central processing unit. 
Note that this service does not support nesting. If nesting is 
needed for critical sections e.g. for libraries 
SuspendOSInterrupts/ResumeOSInterruptsor 
SuspendAllInterrupt/ResumeAllInterruptsshould be used. 
Status: 
Standard:  •none 
Extended:  •none 
Conformance:  BCC1, BCC2
*/
void DisableAllInterrupts ( void ) {
	
}
/*
Parameter (In):  none 
Parameter (Out):  none 
Description:  This service restores the recognition status of all interrupts 
saved by the SuspendAllInterruptsservice. 
Particularities:  The service may be called from an ISR category 1 and category 
2, from alarm-callbacks and from the task level, but not from all 
hook routines. 
This service is the counterpart of SuspendAllInterruptsservice, 
which has to have been called before, and its aim is the 
completion of the critical section of code. No API service calls 
beside  SuspendAllInterrupts/ResumeAllInterruptspairs and 
SuspendOSInterrupts/ResumeOSInterruptspairs are allowed 
within this critical section. 
The implementation should adapt this service to the target 
hardware providing a minimum overhead. 
SuspendAllInterrupts/ResumeAllInterruptscan be nested. In 
case of nesting pairs of the calls SuspendAllInterruptsand 
ResumeAllInterruptsthe interrupt recognition status saved by 
the first call of SuspendAllInterruptsis restored by the last call 
of the ResumeAllInterruptsservice. 
Status: 
Standard:  •none 
Extended:  •none 
Conformance:  BCC1, BCC2
*/
void ResumeAllInterrupts ( void ){
	
}
/*
Parameter (In):  none 
Parameter (Out):  none 
Description:  This service saves the recognition status ofall interrupts and 
disables all interrupts for  which the hardware supports 
disabling. 
Particularities:  The service may be called from an ISR category 1 and category 
2, from alarm-callbacks and from the task level, but not from all 
hook routines. 
This service is intended to protect a critical section of code from 
interruptions of any kind. Thissection shall be finished by 
calling the ResumeAllInterruptsservice. No API service calls 
beside  SuspendAllInterrupts/ResumeAllInterruptspairs and 
SuspendOSInterrupts/ResumeOSInterruptspairs are allowed 
within this critical section. 
The implementation should adapt this service to the target 
hardware providing a minimum overhead. 
Status: 
Standard:  •none 
Extended:  •none 
Conformance:  BCC1, BCC2
*/
void SuspendAllInterrupts ( void ) {
	
}
/*
Parameter (In):  none 
Parameter (Out):  none 
Description:  This service restores the recognition status of interrupts saved 
by the SuspendOSInterruptsservice. 
Particularities:  The service may be called from an ISR category 1 and category 
2 and from the task level, but not from hook routines. 
This service is the counterpart of SuspendOSInterruptsservice, 
which has to have been called before, and its aim is the 
completion of the critical section of code. No API service calls 
beside  SuspendAllInterrupts/ResumeAllInterruptspairs and 
SuspendOSInterrupts/ResumeOSInterruptspairs are allowed 
within this critical section. 
The implementation should adapt this service to the target 
hardware providing a minimum overhead. 
SuspendOSInterrupts/ResumeOSInterruptscan be nested. In 
case of nesting pairs of the calls SuspendOSInterruptsand 
ResumeOSInterruptsthe interrupt recognition status saved by 
the first call of SuspendOSInterruptsis restored by the last call 
of the ResumeOSInterruptsservice. 
Status: 
Standard:  •none 
Extended:  •none 
Conformance:  BCC1, BCC2
*/
void ResumeOSInterrupts ( void ){
	
}
/*
Parameter (In):  none 
Parameter (Out):  none 
Description:  This service saves the recognition status of interrupts of 
category 2 and disables the recognition of these interrupts. 
Particularities:  The service may be called from an ISR and from the task level, 
but not from hook routines. 
This service is intended to protect a critical section of code. This 
section shall be finished by calling the ResumeOSInterrupts
service. No API service calls beside 
SuspendAllInterrupts/ResumeAllInterruptspairs and 
SuspendOSInterrupts/ResumeOSInterruptspairs are allowed 
within this critical section. 
The implementation should adapt this service to the target 
hardware providing a minimum overhead. 
It is intended only to disable interrupts of category 2. However, 
if this is not possible in an efficient way more interrupts may be 
disabled. 
Status: 
Standard:  •none 
Extended:  •none 
Conformance:  BCC1, BCC2
*/
void SuspendOSInterrupts ( void ) {
	
}
//inline void tick() {
//	timerTick++;
//	DisableAllInterrupts();
//	Schedule();
//	EnableAllInterrupts();
//}