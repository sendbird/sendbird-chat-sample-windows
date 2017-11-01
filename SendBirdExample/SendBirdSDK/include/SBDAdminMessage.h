//
//  SBDAdminMessage.h
//  SendBird
//
//  Created by Jed Gyeong on 6/28/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDADMINMESSAGE_H_
#define SENDBIRD_SBDADMINMESSAGE_H_

#include <iostream>

#include "SBDBaseMessage.h"
#include "SBDUser.h"


/**
 *  The `SBDAdminMessage` class represents a <span>message</span> which is sent by an admin via [Platform API](https://docs.sendbird.com/platform). The admin <span>message</span> doesn't have a sender. This <span>message</span> is sutable for a notice to inform the members in a group channel or the participants in an open channel. If you would like to know about the Platform API for the <span>message</span>, see [Send a message](https://docs.sendbird.com/platform#messages_3_send).
 */
class SBDAdminMessage : public SBDBaseMessage {
public:
	/**
	 *  Message text.
	 */
	wstring message;

	/**
	 *  Data of <span>message</span>.
	 */
	wstring data;

	/**
	 *  Custom message type.
	 */
	wstring custom_type;

	/**
	 *  Internal use only.
	 */
	SBDAdminMessage(string dict);

	/**
	 *  Internal use only.
	 */
	~SBDAdminMessage() {};

	/**
	*  Internal use only.
	*/
	void Overwrite(SBDAdminMessage *message);
};

#endif /* SENDBIRD_SBDADMINMESSAGE_H_ */
