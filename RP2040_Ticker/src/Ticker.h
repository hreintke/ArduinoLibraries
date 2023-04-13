/*
 * Ticker.h
 *
 *  Created on: 12 Apr 2023
 *      Author: Herman
 */

#ifndef TICKER_H_
#define TICKER_H_

#include <functional>
#include <pico/time.h>

class Ticker {
public:
	using callback_t = std::function<void(void)>;
	enum tickerType_t {UNDEFINED, PERIODIC, ONCE};
	Ticker();
	~Ticker();

	struct repeating_timer repeatingTimer;
	struct repeating_timer* activeTimer;

	alarm_id_t activeAlarm;

	uint64_t activeInterval = 0;

	static bool repeatCallback(struct repeating_timer *t);
	static int64_t onceCallback(alarm_id_t alarmID, void* userdata);

	void attachPeriodicMs(uint32_t milliseconds, callback_t);
	void attachPeriodicUs(uint64_t microseconds, callback_t);
	void attachOnceMs(uint32_t milliseconds, callback_t);
	void attachOnceUs(uint64_t microseconds, callback_t);
	void restart();
	void detach();
	tickerType_t active();

	void resetMs(uint32_t interval, callback_t cb, tickerType_t tt = UNDEFINED);
	void resetUs(uint64_t interval, callback_t cb, tickerType_t tt = UNDEFINED);

	callback_t activeCallback;
	tickerType_t activeType = UNDEFINED;
};

#endif /* TICKER_H_ */
