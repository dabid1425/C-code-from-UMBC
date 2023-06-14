/*
 * AvrXYieldHack.h
 *
 * Created: 11/27/2012 11:18:37 AM
 *  Author: Kirit
 */ 


#ifndef AVRXYIELDHACK_H_
#define AVRXYIELDHACK_H_

//#define _SFR_ASM_COMPAT 1
#include "avrx/avrx-io.h"
#include "avrx/avrx-signal.h"
#include "avrx/avrx.h"
#include "avrx/serialio.h"       // From AvrX...
#include "avrx_hardware_custom.h"

extern TimerControlBlock timerYield;

#define AvrXYieldHack(x) AvrXDelay(&timerYield, 2); //at least 2 is required for the delay




#endif /* AVRXYIELDHACK_H_ */