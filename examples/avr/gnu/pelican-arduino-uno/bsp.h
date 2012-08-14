/*****************************************************************************
* PELICAN crossing example
* Last Updated for Version: 4.1.05
* Date of the Last Update:  Dec 08, 2010
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) 2002-2010 Quantum Leaps, LLC. All rights reserved.
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
#ifndef bsp_h
#define bsp_h
                                                          /* CPU clock [Hz] */
#define F_CPU                8000000UL

                                              /* Sys timer tick per seconds */
#define BSP_TICKS_PER_SEC    50

/* street signals ..........................................................*/
enum BSP_CarsSignal {
    CARS_RED, CARS_YELLOW, CARS_GREEN, CARS_OFF
};
enum BSP_PedsSignal {
    PEDS_DONT_WALK, PEDS_BLANK, PEDS_WALK
};
void BSP_init(void);
void BSP_signalCars(enum BSP_CarsSignal sig);
void BSP_signalPeds(enum BSP_PedsSignal sig);

#define BSP_showState(prio_, state_) ((void)0)


#include <avr\io.h>                                              /* AVR I/O */

#endif                                                             /* bsp_h */

