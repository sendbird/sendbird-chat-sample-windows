//
//  SBDError.h
//  SendBird
//
//  Created by Jed Gyeong on 6/30/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDERROR_H_
#define SENDBIRD_SBDERROR_H_

#include <iostream>

#include "SBDTypes.h"

using namespace std;

/**
 *  SendBird error class.
 */
class SBDError {
public:
	/**
	 *  The error message.
	 */
    wstring message;

	/**
	 *  The error code.
	 */
    int64_t code;

	/**
	 *  Internal use only.
	 */
	SBDError(string msg, int64_t c);
};

#endif /* SENDBIRD_SBDERROR_H_ */
