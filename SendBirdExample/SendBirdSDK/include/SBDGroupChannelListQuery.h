//
//  SBDGroupChannelListQuery.h
//  SendBird
//
//  Created by Jed Gyeong on 6/29/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDGROUPCHANNELLISTQUERY_H_
#define SENDBIRD_SBDGROUPCHANNELLISTQUERY_H_

#include <iostream>
#include "SBDUser.h"
#include "SBDGroupChannel.h"

class SBDLoadNextGroupChannelListInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(vector<SBDGroupChannel *> channels, SBDError *error) {};
};

/**
 *  The `SBDGroupChannelListQuery` class is a query class for getting the list of group channels. The instance of this class is created by [`CreateMyGroupChannelListQuery()`]() in `SBDGroupChannel` class.
 */
class SBDGroupChannelListQuery {
private:
    SBDUser user;
    vector<SBDUser> users_filter_exact_match;
    vector<SBDUser> users_filter_like_match;
    wstring nickname_contains_filter;
    SBDMemberStateFilter member_state_filter;
    wstring custom_type_filter;
    vector<wstring> channel_urls_filter;
    wstring channel_name_filter;
    
public:
    /**
     *  Sets the number of channels per page.
     */
    int64_t limit;
    
    /**
     *  If the value is true, the channel list includes empty channel.
     */
    bool include_empty_channel;
    
    /**
     *  If the value is true, the channel object of the list includes members list.
     */
    bool include_member_list;
    
    /**
     *  Sets the order of the list. The order is defined in `SBDGroupChannelListOrder`.
     */
    SBDGroupChannelListOrder order;
    
    /**
     *  Shows if there is a next page
     */
    bool has_next;
    
    /**
     *  Sets query type for `include_member_list`.
     */
    SBDGroupChannelListQueryType query_type;

	/**
	 *  Shows if the query is loading.
	 */
	bool is_loading;

	/**
	*	Sets to filter channels by the hidden state. 
	*	The default value is SBDChannelHiddenStateFilterUnhiddenOnly.
	*/
	SBDChannelHiddenStateFilter channel_hidden_state_filter;
    
    /**
     *  Sets the custom type filter.
     *
     *  @param custom_type The custom type to search.
     */
    void SetCustomTypeFilter(const wstring& custom_type);
    
    /**
     *  Sets the member state filter.
     *
     *  @param member_state_filter The member state to search.
     */
    void SetMemberStateFilter(SBDMemberStateFilter member_state_filter);
    
    /**
     *  Sets <code>SBDGroupChannel</code> URLs filter. <code>SBDGroupChannel</code> list containing only and exactly the passed <code>SBDGroupChannel</code> URLs will be returned. This does not cooperate with other filters.
     * 
     *  @param channel_urls The channel urls to search.
     */
    void SetChannelUrlsFilter(const vector<wstring>& channel_urls);
    
    /**
     *  Sets a filter to return only channels that contains the specified group channel name.
     *
     *  @param channel_name The channel name to search. The query will return the channels include `channel_name`.
     */
    void SetChannelNameContainsFilter(const wstring& channel_name);
    
    /**
     *  Sets the filter with nickname. The group channels which have the member that has nickname are returned by `loadNextPageWithCompletionHandler:`(LIKE search).
     *
     *  @param nickname Nickname to search.
     */
    void SetNicknameContainsFilter(const wstring& nickname);
   
    /**
     *  Sets the filter with users. The group channels which have the members that contain users are returned by `LoadNextPage()`.
     *
     *  @param users    Users to search.
     *  @param query_type Logical condition applied to filter. If you pass `SBDGroupChannelListQueryTypeAnd` to `query_type` and A, B to `users`, the channels whose members containing A and B will be returned. If `SBDGroupChannelListQueryTypeOr` is set, the members of the queried channels will be A or B.
	 */
    void SetUsersIncludeFilter(const vector<SBDUser>& users, SBDGroupChannelListQueryType query_type);
    
    /**
     *  Sets the filter with users. The group channels which have the members that have users are returned by `LoadNextPage()`. The channels have the `users` members only.
     *
     *  @param users    Users to search.
     */
    void SetUsersExactFilter(const vector<SBDUser>& users);

	/**
	*	Sets to filter channels by the hidden state. The default value is SBDChannelHiddenStateFilterUnhiddenOnly.
	*
	*	@param channel_hidden_state_filter
	*/
	void SetChannelHiddenStateFilter(const SBDChannelHiddenStateFilter channel_hidden_state_filter);
    
    /**
     *  Gets the list of channels. If this method is repeatedly called, it will retrieve the following pages of the channel list.
     *
     *  @param completion_handler The handler block to execute. The `channels` is the array of `SBDGroupChannel` instances.
     */
    void LoadNextPage(SBDLoadNextGroupChannelListInterface *completion_handler);

	/**
	 *  Internal use only.
	 */
	string token;

	/**
	 *  Internal use only.
	 */
	SBDGroupChannelListQuery(SBDUser user);
};

#endif /* SENDBIRD_SBDGROUPCHANNELLISTQUERY_H_ */
