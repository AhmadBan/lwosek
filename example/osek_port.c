#include "common.h"
#include "osek_port.h"
#include "osek.h"
#include "TM4C123GH6PM.h"
#include "core_cm4.h"

extern uint64_t timerTick;
/*
Parameter (In): none 
Parameter (Out):  none 
Description: this is tick timer ISR which is interrupt handler
*/
void tickHandler(void) {
	
}
/*
Parameter (In): none
Parameter (Out):  none 
Description: configure context switch interrupt as the least priority interrupt
configure Tickhandler interrupt as the level equal to the level of ISR category 2 it should be equal to OSEK_INTERRUPT_LEVEL configuration

*/
void initConfigureINTC() {
	
	
}


/*
Parameter (In): none
Parameter (Out):  none 
Description: Activate Context Switch Interrupt
*/
void activateCSInt(void) {
	
}
/*
Parameter (In): none
Parameter (Out):  none 
Description: ISR handler that is exclusively used for context switching
*/
__asm void contextSwitchISR(void) {

	
}


/*
Parameter (In): none
Parameter (Out):  none 
Description: Globally enable all interrupt in microcontroller
*/
void EnableAllInterrupts_uC(void){
	
}
/*
Parameter (In): none
Parameter (Out):  none 
Description: Globally disable all interrupt in microcontroller
*/
void DisableAllInterrupts_uC( void ){
	
	
}
/*
Parameter (In): none
Parameter (Out):  none 
Description:  This service saves the recognition status of all interrupts and 
disables all interrupts for  which the hardware supports 
disabling. 
*/
void SuspendAllInterrupts_uC(void){
}
/*
Parameter (In): none
Parameter (Out):  none 
Description: This service restores the recognition status of all interrupts 
saved by the SuspendAllInterruptsservice. 
*/
void ResumeAllInterrupts_uC(void){}

/*
Parameter (In): none
Parameter (Out):  none 
Description: This service saves the recognition status of interrupts of 
category 2 and disables the recognition of these interrupts.
	/*
Parameter (In): none
Parameter (Out):  none 
Description: This service restores the recognition status of interrupts saved 
by the SuspendOSInterrupts service. 
*/
void ResumeOSInterrupts_uC(void){}


