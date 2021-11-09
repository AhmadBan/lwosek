#include <stdint.h>
#include "bsp.h"
#include "osek.h"
void Q_onAssert(char const *module, int loc)
{
    /* TBD: damage control */
    (void)module; /* avoid the "unused parameter" compiler warning */
    (void)loc;    /* avoid the "unused parameter" compiler warning */
    // NVIC_SystemReset();
}

int main()
{
    ActivateTask(ID_idle);
    StartOS(OSDEFAULTAPPMODE);

    // return 0;
}
