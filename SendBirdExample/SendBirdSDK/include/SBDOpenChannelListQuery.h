//
//  SBDOpenChannelListQuery.h
//  SendBird
//
//  Created by Jed Gyeong on 6/29/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDOPENCHANNELLISTQUERY_H_
#define SENDBIRD_SBDOPENCHANNELLISTQUERY_H_

#include <iostream>
#include "SBDOpenChannel.h"
#include "SBDError.h"

using namespace std;

class SBDLoadNextOpenChannelListInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(vector<SBDOpenChannel *> channels, SBDError *error) {};
};

/**
 *  The `SBDOpenChannelListQuery` class is a query class for getting the list of open channels. The instance of this class is created by [`CreateOpenChannelListQuery()`]() in `SBDOpenChannel` class.
 */
class SBDOpenChannelListQuery {
private:
    wstring channel_url_filter;
    wstring channel_name_filter;
    wstring custom_type_filter;
    
public:
    /**
     *  Sets the number of channels per page.
     */
    int64_t limit;
    
    /**
     *  Shows if there is a next page.
     */
    bool has_next;
    
    /**
     *  Shows if the query is loading.
     */
    bool is_loading;
    
    /**
     *  Sets the channel URL filter.
     *
     *  @param channel_url The channel url to search.
     */
    void SetChannelUrlFilter(wstring channel_url);
    
    /**
     *  Sets the channel name filter.
     *
     *  @param channel_name The channel name to search.
     */
    void SetChannelNameFilter(wstring channel_name);
    
    /**
     *  Sets the custom type filter.
     *
     *  @param custom_type The custom type to search.
     */
    void SetCustomTypeFilter(wstring custom_type);
    
    /**
     *  Gets the list of channels. If this method is repeatedly called, it will retrieve the following pages of the channel list.
     *
     *  @param completion_handler The handler interface to execute.
     */
    void LoadNextPage(SBDLoadNextOpenChannelListInterface *completion_handler);

	/**
	 *  Internal use only.
	 */
	SBDOpenChannelListQuery();

	/**
	 *  Internal use only.
	 */
	string token;
};

#endif /* SENDBIRD_SBDOPENCHANNELLISTQUERY_H_ */
