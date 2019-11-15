//
//  SBDReadStatus.h
//  SendBird
//
//  Created by Jed Gyeong on 8/21/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDREADSTATUS_H_
#define SENDBIRD_SBDREADSTATUS_H_

#include <iostream>
#include "SBDUser.h"

/**
 *  Internal use only.
 */
class SBDReadStatus {
public:
    SBDUser reader;
    int64_t timestamp;
    wstring channel_url;
    wstring channel_type;
    
    SBDReadStatus();
    SBDReadStatus(const string& dict);
};

#endif /* SENDBIRD_SBDREADSTATUS_H_ */
