//
//  SBDUserListQuery.h
//  SendBird
//
//  Created by Jed Gyeong on 6/29/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDUSERLISTQUERY_H_
#define SENDBIRD_SBDUSERLISTQUERY_H_

#include <iostream>
#include <vector>

#include "SBDUser.h"
#include "SBDTypes.h"
#include "SBDInternalTypes.h"
#include "SBDBaseChannel.h"

class SBDLoadNextUserListInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(const vector<SBDUser>& users, SBDError *error) {};
};

/**
 *  The `SBDUserListQuery` class is a query class for getting the list of all users, participants, blocked users, muted users and banned users. Each type of the query is created by the class or the instance that is related to it.
 *
 *  * The query for the all users is created by [`CreateAllUserListQuery()`]() of `SBDMain` class.
 *  * The query for the users of the specified user IDs is created by [`CreateUserListQuery()`]() of `SBDMain` class.
 *  * The query for the blocked users is created by [`CreateBlockedUserListQuery()`]() of `SBDMain` class.
 *  * The query for the participants in the specified open <span>channel</span> is created by [`CreateParticipantListQuery()`]() of the `SBDOpenChannel` instance.
 *  * The query for the muted users is created by [`CreateMutedUserListQuery()`]() of the `SBDOpenChannel` instance.
 *  * The query for the banned users is created by [`CreateBannedUserListQuery()`]() of the `SBDOpenChannel` instance.
 *
 */
class SBDUserListQuery {
public:
	/**
	 *  The channel instance related to query.
	 */
    SBDBaseChannel *channel;

	/**
	 *  Query type. It is defined in `SBDUserListQueryType`.
	 */
    SBDUserListQueryType query_type;

	/**
	 *  Sets the number of users per page.
	 */
    int64_t limit;

	/**
	 *  Shows if there is a next page
	 */
    bool has_next;

	/**
	 *  Shows if the query is loading.
	 */
    bool is_loading;
    
	/**
	 *  Sets meta data filter.
	 *
	 *  @param key The key of the meta data to use for filter.
	 *  @param values The values of the meta data to use for filter.
	 */
    void SetMetaDataFilter(const wstring& key, const vector<wstring>& values);
    
	/**
	 *  Gets the list of users. If this method is repeatedly called, it will retrieve the following pages of the user list.
	 *
	 *  @param completion_handler The handler interface to execute. The `users` is the array of `SBDUser` instances.
	 */
    void LoadNextPage(SBDLoadNextUserListInterface *completion_handler);
    
	/**
	 *  Internal use only.
	 */
	string token;

	/**
	 *  Internal use only.
	 */
	wstring meta_data_key;

	/**
	 *  Internal use only.
	 */
	vector<wstring> meta_data_values;

	/**
	 *  Internal use only.
	 */
	vector<wstring> user_ids;

	/**
	 *  Internal use only.
	 */
	SBDUserListQuery(SBDUserListQueryType query_type, SBDBaseChannel *channel);

	/**
	 *  Internal use only.
	 */
	SBDUserListQuery(const vector<wstring>& user_ids);

private:
    void LoadAllUserListNextPage(SBDLoadNextUserListInterface *completion_handler);
    void LoadBlockedUserListNextPage(SBDLoadNextUserListInterface *completion_handler);
    void LoadParticipantListNextPage(SBDLoadNextUserListInterface *completion_handler);
    void LoadMutedUserListNextPage(SBDLoadNextUserListInterface *completion_handler);
    void LoadBannedUserListNextPage(SBDLoadNextUserListInterface *completion_handler);
    void LoadFilteredUserListNextPage(SBDLoadNextUserListInterface *completion_handler);
};

#endif /* SENDBIRD_SBDUSERLISTQUERY_H_ */
