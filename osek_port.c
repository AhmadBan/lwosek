#include "common.h"
#include "osek_port.h"
#include "osek.h"
#include "TM4C123GH6PM.h"
#include "core_cm4.h"

extern uint64_t timerTick;
void tickHandler(void) {
	timerTick++;
	Schedule();
}
// configure context switch interrupt as the least priority interrupt
// configure Tickhandler interrupt as the level equal to the level of ISR category 2 it should be equal to OSEK_INTERRUPT_LEVEL configuration
void initConfigureINTC() {
	/* set the PendSV interrupt priority to the lowest level 0xFF */
	//*(uint32_t volatile*)0xE000ED20 |= (0xFFU << 16);
	NVIC_SetPriority(PendSV_IRQn, OSEK_LEAST_INTERRUPT_LEVEL);
	NVIC_SetPriority(SysTick_IRQn, OSEK_INTERRUPT_LEVEL);
	
}



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


void EnableAllInterrupts_uC(void){
	
}

void DisableAllInterrupts_uC( void ){
	
	
}

void SuspendAllInterrupts_uC(void){
}
void ResumeAllInterrupts_uC(void){}
void SuspendOSInterrupts_uC(void){}
void ResumeOSInterrupts_uC(void){}


