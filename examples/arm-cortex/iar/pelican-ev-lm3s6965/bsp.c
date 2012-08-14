/*****************************************************************************
* Product: PELICAN crossing example, EV-LM3S6965 board
* Last Updated for Version: 4.4.00
* Date of the Last Update:  Feb 29, 2012
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2012 Quantum Leaps, LLC. All rights reserved.
*
* This software may be distributed and modified under the terms of the GNU
* General Public License version 2 (GPL) as published by the Free Software
* Foundation and appearing in the file GPL.TXT included in the packaging of
* this file. Please note that GPL Section 2[b] requires that all works based
* on this software must also be made publicly available under the terms of
* the GPL ("Copyleft").
*
* Alternatively, this software may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GPL and are specifically designed for licensees interested in
* retaining the proprietary status of their code.
*
* Contact information:
* Quantum Leaps Web site:  http://www.quantum-leaps.com
* e-mail:                  info@quantum-leaps.com
*****************************************************************************/
#include "qpn_port.h"
#include "bsp.h"
#include "pelican.h"

#include "lm3s_cmsis.h"

                      /* include the correct OLED display implementation... */
#include "rit128x96x4.h"      /* RITEK 128x96x4 OLED used in Rev C-D boards */

enum ISR_Priorities {   /* ISR priorities starting from the highest urgency */
    GPIOPORTA_PRIO,
    SYSTICK_PRIO,
    /* ... */
};

#define USER_LED           (1U << 0)
#define USER_BTN           (1U << 1)

/* ISR prototypes */
void SysTick_Handler(void);
void GPIOPortA_IRQHandler(void);

/*..........................................................................*/
void SysTick_Handler(void) {
    QF_tickISR();
}
/*..........................................................................*/
void GPIOPortA_IRQHandler(void) {
    QActive_postISR((QActive *)&AO_Ped, PEDS_WAITING_SIG, 0);
}
/*..........................................................................*/
void BSP_init(void) {

    SystemInit();                         /* initialize the system clocking */

                      /* enable the peripherals used by this application... */
    SYSCTL->RCGC2 |= (1 << 5);                    /* enable clock to GPIOF  */
    __NOP();
    __NOP();
                                       /* configure the User LED (PortF)... */
    GPIOF->DIR   |= USER_LED;                      /* set direction: output */
    GPIOF->DEN   |= USER_LED;                             /* digital enable */
    GPIOF->AMSEL &= ~USER_LED;
    GPIOF->DATA_Bits[USER_LED] = 0;                     /* turn the LED off */

                              /* configure the pin connected to the Buttons */
    GPIOF->DIR   &= ~USER_BTN;                      /* set direction: input */
    GPIOF->DR2R  |=  USER_BTN;
    GPIOF->ODR   &= ~USER_BTN;
    GPIOF->PUR   |=  USER_BTN;
    GPIOF->PDR   &= ~USER_BTN;
    GPIOF->DEN   |=  USER_BTN;
    GPIOF->AMSEL &= ~USER_BTN;

    RIT128x96x4Init(1000000);                /* initialize the OLED display */
}
/*..........................................................................*/
void QF_onStartup(void) {
              /* set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */
    SysTick_Config(SystemFrequency / BSP_TICKS_PER_SEC);

                 /* enable GPIOPortA interrupt used for testing preemptions */
    NVIC_EnableIRQ(GPIOPortA_IRQn);
                                        /* set priorities of all interrupts */
    NVIC_SetPriority(SysTick_IRQn,   SYSTICK_PRIO);
    NVIC_SetPriority(GPIOPortA_IRQn, GPIOPORTA_PRIO);
}
/*..........................................................................*/
void QF_onIdle(void) {      /* entered with interrupts DISABLED, see NOTE01 */

    /* toggle the User LED on and then off, see NOTE02 */
    GPIOF->DATA_Bits[USER_LED] = USER_LED;         /* turn the User LED on  */
    GPIOF->DATA_Bits[USER_LED] = 0;                /* turn the User LED off */

#ifdef NDEBUG
    /* put the CPU and peripherals to the low-power mode, see NOTE02 */
    __WFI();
#endif
    QF_INT_ENABLE();                            /* always enable interrupts */
}
/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    (void)file;                                   /* avoid compiler warning */
    (void)line;                                   /* avoid compiler warning */
    QF_INT_DISABLE();         /* make sure that all interrupts are disabled */
    for (;;) {       /* NOTE: replace the loop with reset for final version */
    }
}
/*..........................................................................*/
void BSP_signalCars(enum BSP_CarsSignal sig) {
    switch (sig) {
        case CARS_RED: {
            RIT128x96x4StringDraw("RED", 78, 10, 5);
            break;
        }
        case CARS_YELLOW: {
            RIT128x96x4StringDraw("YLW", 78, 10, 5);
            break;
        }
        case CARS_GREEN: {
            RIT128x96x4StringDraw("GRN", 78, 10, 5);
            break;
        }
        case CARS_OFF: {
            RIT128x96x4StringDraw("   ", 78, 10, 5);
            break;
        }
    }
}
/*..........................................................................*/
void BSP_signalPeds(enum BSP_PedsSignal sig) {
    switch (sig) {
        case PEDS_DONT_WALK: {
            RIT128x96x4StringDraw("DON'T WALK", 0, 10, 5);
            break;
        }
        case PEDS_BLANK: {
            RIT128x96x4StringDraw("          ", 0, 10, 5);
            break;
        }
        case PEDS_WALK: {
            RIT128x96x4StringDraw("** WALK **", 0, 10, 5);
            break;
        }
    }
}
/*..........................................................................*/
void BSP_showState(uint8_t prio, char const *state) {
    if (QF_active[prio].act == (QActive *)&AO_Pelican) {
        RIT128x96x4StringDraw(state, 0, 0, 5);
    }
}

/*****************************************************************************
* NOTE01:
* The QF_onIdle() callback is called with interrupts disabled, because the
* determination of the idle condition might change by any interrupt posting
* an event. QF_onIdle() must internally enable interrupts, ideally atomically
* with putting the CPU to the power-saving mode.
*
* NOTE02:
* The User LED is used to visualize the idle loop activity. The brightness
* of the LED is proportional to the frequency of invcations of the idle loop.
* Please note that the LED is toggled with interrupts disabled, so no interrupt
* execution time contributes to the brightness of the User LED.
*/
