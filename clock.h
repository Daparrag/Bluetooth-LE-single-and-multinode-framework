/*Clock.h*/

#ifndef _BLE_CLOCK_H_
#define _BLE_CLOCK_H_

void set_ticks(tClockTime Ctime);
void set_clock(tClockTime Ctime, tClockTime Cseconds);
void Clock_reset(void);
void Clock_Wait(uint32_t i);
tClockTime SClock_Time(void);
tClockTime Clock_Time(void)
void Clock_Init(void)
/*_BLE_CLOCK_H_*/