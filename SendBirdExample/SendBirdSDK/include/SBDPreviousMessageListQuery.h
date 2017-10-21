//
//  SBDPreviousMessageListQuery.h
//  SendBird
//
//  Created by Jed Gyeong on 6/29/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDPREVIOUSMESSAGELISTQUERY_H_
#define SENDBIRD_SBDPREVIOUSMESSAGELISTQUERY_H_

#include <iostream>
#include <vector>
#include "SBDBaseChannel.h"

class SBDLoadPreviousMessageListInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(vector<SBDBaseMessage *> messages, SBDError *error) {};
};

/**
 *  An object which retrieves messages from the given channel. The instance of this class is created by [`createPreviousMessageListQuery`]() in `SBDBaseChannel` class.
 */
class SBDPreviousMessageListQuery {
private:
    SBDBaseChannel *channel;
    
public:
    /**
     *  Shows if the query is loading. It is true if the query is loading, otherwise returns false.
     */
    bool is_loading;
    
    /**
     *  Loads previous message with limit.
     *
     *  @param limit             The number of messages per page.
     *  @param reverse           If yes, the latest message is the index 0.
     *  @param completion_handler The handler interface to execute.
     */
    void LoadNextPage(int limit, bool reverse, SBDLoadPreviousMessageListInterface *completion_handler);

	/**
	 *  Internal use only.
	 */
	SBDPreviousMessageListQuery(SBDBaseChannel *channel);

	/**
	 *  Internal use only.
	 */
	int64_t minimum_timestamp;
};

#endif /* SENDBIRD_SBDPREVIOUSMESSAGELISTQUERY_H_ */
