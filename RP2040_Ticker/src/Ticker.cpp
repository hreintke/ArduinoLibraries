/*
 * Ticker.cpp
 *
 *  Created on: 12 Apr 2023
 *      Author: Herman
 */

#include "Ticker.h"
#include <pico/time.h>

Ticker::Ticker()
{
}

Ticker::~Ticker()
{
	detach();
}

bool Ticker::repeatCallback(struct repeating_timer *t)
{
	Ticker *tk = (Ticker*)(t->user_data);
	if (tk->activeCallback)
	{
		tk->activeCallback();
	}
	return true;
}

int64_t Ticker::onceCallback(alarm_id_t alarmID, void* userdata)
{
	Ticker *tk = (Ticker*)userdata;
	if (tk->activeCallback)
	{
		tk->activeCallback();
	}
	return 0;
}

void Ticker::attachPeriodicMs(uint32_t milliseconds, callback_t cb)
{
	attachPeriodicUs(milliseconds * 1000, cb);
}

void Ticker::attachOnceMs(uint32_t milliseconds, callback_t cb)
{
	attachOnceUs(milliseconds * 1000 , cb);
}

void Ticker::attachPeriodicUs(uint64_t microseconds, callback_t cb)
{
	detach();
	activeTimer = &repeatingTimer;
	activeCallback = cb;
	activeInterval = microseconds;
	activeType = PERIODIC;
	add_repeating_timer_us(microseconds, Ticker::repeatCallback, this, activeTimer);
}

void Ticker::attachOnceUs(uint64_t microseconds, callback_t cb)
{
	detach();
	activeCallback = cb;
	activeInterval = microseconds;
	activeType = ONCE;
	activeAlarm = add_alarm_in_us(microseconds, Ticker::onceCallback, this, true);
}

void Ticker::detach()
{
	if (activeTimer)
	{
		cancel_repeating_timer(activeTimer);
		activeTimer = nullptr;
	}
	if (activeAlarm)
	{
		cancel_alarm(activeAlarm);
		activeAlarm = 0;
	}
}

void Ticker::resetMs(uint32_t interval, callback_t cb, tickerType_t tt )  // tt default UNDEFINED -> keep current setting
{
	resetUs(interval * 1000, cb, tt);
}

void Ticker::resetUs(uint64_t interval, callback_t cb, tickerType_t tt ) // tt default UNDEFINED -> keep current setting
{
	detach();
	activeInterval = interval;
	activeCallback = cb;
	if (tt != UNDEFINED)
	{
		activeType = tt;
	}
}

void Ticker::restart()
{
	detach();
	switch (activeType) {

		case UNDEFINED:
			break;
		case PERIODIC:
			attachPeriodicUs(activeInterval, activeCallback);
			break;
		case ONCE:
			attachOnceUs(activeInterval, activeCallback);
			break;
	}
}

