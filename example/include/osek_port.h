
#define contextSwitchISR PendSV_Handler
#define tickHandler SysTick_Handler
// Activate Context Switch Interrupt
void activateCSInt(void);
void initConfigureINTC(void);
void EnableAllInterrupts_uC(void);
void DisableAllInterrupts_uC(void);
void SuspendAllInterrupts_uC(void);
void ResumeAllInterrupts_uC(void);
void SuspendOSInterrupts_uC(void);
void ResumeOSInterrupts_uC(void);
