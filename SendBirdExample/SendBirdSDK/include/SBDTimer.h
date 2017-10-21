//
//  SBDTimer.h
//  SendBird
//
//  Created by Jed Gyeong on 7/14/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDTIMER_H_
#define SENDBIRD_SBDTIMER_H_

#include <iostream>
#include <stdint.h>
#include <Windows.h>
#include "SBDCrossThread.h"

using namespace std;

/**
 *  Internal use only.
 */
class SBDTimer {
private:
	HANDLE thread;
    static DWORD WINAPI Timer(void *vargp);
    
public:
	SBDTimer::SBDTimer(DWORD time_interval, bool repeat);
    ~SBDTimer();
    
    void Start();
    void End();
    void Invalidate();
    
    virtual void RunCallback() {};

	DWORD interval;
    bool repeat;
	bool stop;
};

#endif /* SENDBIRD_SBDTIMER_H_ */
