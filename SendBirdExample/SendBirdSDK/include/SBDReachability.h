//
//  SBDReachability.h
//  SendBird
//
//  Created by Jed Gyeong on 6/28/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDREACHABILITY_H_
#define SENDBIRD_SBDREACHABILITY_H_

#include <iostream>

typedef enum {
    SBDNotReachable = 0,
    SBDReachableViaWiFi,
    SBDReachableViaWWAN
} SBDNetworkStatus;


/**
 *  Internal use only.
 */
class SBDReachability {
public:
    bool StartNotifier();
    bool StopNotifier();
    SBDNetworkStatus GetCurrentReachabilityStatus();
    bool ConnectionRequired();
};

#endif /* SENDBIRD_SBDREACHABILITY_H_ */
