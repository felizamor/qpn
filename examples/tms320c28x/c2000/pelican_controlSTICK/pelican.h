/*****************************************************************************
* Model: pelican.qm
* File:  ./pelican.h
*
* This file has been generated automatically by QP Modeler (QM).
* DO NOT EDIT THIS FILE MANUALLY.
*
* Please visit www.state-machine.com/qm for more information.
*****************************************************************************/
#ifndef pelican_h
#define pelican_h

enum PelicanSignals {
    PEDS_WAITING_SIG = Q_USER_SIG,
    OFF_SIG,
    ON_SIG,
    TERMINATE_SIG
};

/* active objects ................................................*/
/* @(/1/2) .................................................................*/
/** 
* constructor
*/
void Pelican_ctor(void);


extern struct PelicanTag AO_Pelican;

#endif /* pelican_h */
