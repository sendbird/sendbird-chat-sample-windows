//
//  SBDUserMessageParams.h
//  SendBird
//
//  Created by Miyoung Han on 10/10/19.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDUSERMESSAGEPARAMS_H_
#define SENDBIRD_SBDUSERMESSAGEPARAMS_H_

#include <vector>

#include "SBDBaseMessageParams.h"

/**
 *  The `SBDUserMessageParams` class is used to send a user message in `SBDBaseChannel`. This is a child class of `SBDBaseMessageParams`.
 */
class SBDUserMessageParams : public SBDBaseMessageParams {
public:
	/**
	 *  Message text.
	 */
	wstring message;

	/**
	 *  The target languages that the message will be translated into.
	 */
	vector<wstring> target_languages;


	/**
	 *  Internal use only
	 */
	SBDUserMessageParams();

	
	/**
	 *  Internal use only
	 */
	~SBDUserMessageParams();

};

#endif /* SENDBIRD_SBDUSERMESSAGEPARAMS_H_ */