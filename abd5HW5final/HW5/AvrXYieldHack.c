#include "AvrXYieldHack.h"

/*  HACK!! This is a hack to provide a functionality similar to the AvrXYield*/
/* AvrXYield() should allow yeilding to next process with same priority, but it is missing in the compiled OS.*/
TimerControlBlock timerYield;
