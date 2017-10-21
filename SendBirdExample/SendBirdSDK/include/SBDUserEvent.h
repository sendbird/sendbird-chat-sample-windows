//
//  SBDUserEvent.h
//  SendBird
//
//  Created by Jed Gyeong on 8/21/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDUSEREVENT_H_
#define SENDBIRD_SBDUSEREVENT_H_

#include <iostream>
#include "SBDInternalTypes.h"
#include "SBDUser.h"

/**
 *  Internal use only.
 */
class SBDUserEvent {
public:
	string data;
    SBDUserEventCategory user_event_category;
    SBDUser blocker;
	bool has_blocker;
    SBDUser blockee;
	bool has_blockee;
    int64_t ts;
    
    SBDUserEvent(string dict);
};

#endif /* SENDBIRD_SBDUSEREVENT_H_ */
