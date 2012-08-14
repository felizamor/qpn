/***********************************************************************/
/*                                                                     */
/*  FILE        :resetprg.c                                            */
/*  DATE        :Tue, Oct 21, 2008                                     */
/*  DESCRIPTION :initialize for C language.                            */
/*  CPU GROUP   :87                                                    */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.8).     */
/*                                                                     */
/*  Quantum Leaps, LLC, 28-Oct-2008: Elimiated user stack usage        */
/***********************************************************************/

/*********************************************************************
 *  STARTUP for M32C
 *  Copyright(c) 2004 Renesas Technology Corp.
 *  And Renesas Solutions Corp.,All Rights Reserved.
 *
 *  restprg.c : startup file
 *
 *  Function:initialize each function
 *
 * $Date: 2006/06/16 03:10:49 $
 * $Revision: 1.16 $
 ********************************************************************/
#include "resetprg.h"
////////////////////////////////////////////
// declare sfr register
#pragma ADDRESS    protect  0AH
#pragma ADDRESS    pmode0   04H
#pragma ADDRESS    _SB__    0400H
_UBYTE protect,pmode0;
_UBYTE _SB__;

DEF_SBREGISTER;

#pragma entry start
void start(void);
extern void initsct(void);
extern void _init(void);
void exit(void);
void main(void);

#pragma section program interrupt

void start(void)
{
    _isp_   = &_istack_top;            // set interrupt stack pointer
    protect = 0x02;                    // change protect mode register
    pmode0  = 0x00;                    // set processor mode register
    protect = 0x00;                    // change protect mode register
    _sb_    = (char _far *)0x400;      // 400H fixation (Do not change)
    _asm("    fset    b");
    _sb_    = (char _far *)0x400;
    _asm("    fclr    b");
    _intb_  = (char _far *)VECTOR_ADR; // set variable vector's address

    initsct();                         // initlalize each sections

#ifdef __HEAP__
    heap_init();                       // initialize heap
#endif
#ifdef __STANDARD_IO__
    _init();                           // initialize standard I/O
#endif
    _fb_ = 0;                          // initialize FB registe for debugger
    main();                            // call main routine

    exit();                            // infinite loop
}

void exit(void)
{
    while(1);
}


