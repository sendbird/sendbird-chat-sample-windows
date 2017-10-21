//
//  SBDChannelEvent.h
//  SendBird
//
//  Created by Jed Gyeong on 6/28/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDCHANNELEVENT_H_
#define SENDBIRD_SBDCHANNELEVENT_H_

#include <iostream>

#include "SBDTypes.h"
#include "SBDInternalTypes.h"

using namespace std;

/*!
 SendBird internal use only.
 */
class SBDChannelEvent {
private:
    wstring channel_type;
    
public:
	/**
	 *  Internal use only.
	 */
    SBDChannelEventCategory channel_event_category;
    
	/**
	 *  Internal use only.
	 */
	string data;
    
	/**
	 *  Internal use only.
	 */
    wstring channel_url;
    
	/**
	 *  Internal use only.
	 */
    uint64_t participant_count;

	/**
	 *  Internal use only.
	 */
    bool IsOpenChannel();
    
	/**
	 *  Internal use only.
	 */
    bool IsGroupChannel();

	/**
	 *  Internal use only.
	 */
	SBDChannelEvent(string dict);
};

#endif /* SENDBIRD_SBDCHANNELEVENT_H_ */
