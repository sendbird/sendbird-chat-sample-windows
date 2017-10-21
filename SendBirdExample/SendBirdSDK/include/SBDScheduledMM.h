#pragma once
#include "SBDTimer.h"

class SBDScheduledMM : public SBDTimer {
public:
	SBDScheduledMM(DWORD time_interval, bool repeat) : SBDTimer(time_interval, repeat) {

	}

	void RunCallback();
};
