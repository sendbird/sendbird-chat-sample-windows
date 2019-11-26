//
//  SBDBaseMessageParams.h
//  SendBird
//
//  Created by Miyoung Han on 10/10/19.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDBASEMESSAGEPARAMS_H_
#define SENDBIRD_SBDBASEMESSAGEPARAMS_H_

#include <iostream>
#include <vector>
#include "SBDTypes.h"

using namespace std;

/**
 *  The `SBDBaseMessageParams` class represents the base class which has parameters to create a channel or update a channel. The `SBDUserMessageParams`, the `SBDFileMessageParams` are derived from this class.
 */
class SBDBaseMessageParams {
public:
	/**
	 *  Message data. The default value is null.
	 */
	wstring data;

	/**
	 *  Customize message's type to filter. The default value is null.
	 */
	wstring custom_type;

	SBDMentionType mention_type;

	/**
	*	Can mention to specific users. If sends a message with this field. the message will be arrived to mentioned users. The default value is null.
	*/
	vector<wstring> mentioned_user_ids;
	
	/**
	*	Push option for message. If the option is default, push message will work with the receiver's push settings. If the option is suppress, push message will not be sent.
	*/
	SBDPushNotificationDeliveryOption pushnotification_delivery_option;

	/**
	*	Meta array keys.	
	*/
	vector<wstring> metaarray_keys;

	/**
	 *  Internal use only
	 */
	SBDBaseMessageParams();

	/**
	 *  Internal use only
	 */
	~SBDBaseMessageParams();

};

#endif /* SENDBIRD_SBDBASEMESSAGEPARAMS_H_ */