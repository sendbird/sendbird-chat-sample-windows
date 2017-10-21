#pragma once

//
//  SBDUser.h
//  SendBird
//
//  Created by Jed Gyeong on 6/28/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDUSER_H_
#define SENDBIRD_SBDUSER_H_

#include <iostream>
#include <map>

#include "SBDTypes.h"
#include "SBDError.h"
#include "SBDBaseInterface.h"

using namespace std;

class SBDCreateUserMetaDataInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(map<wstring, wstring> meta_data, SBDError *error) {};
};

class SBDUpdateUserMetaDataInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(map<wstring, wstring> meta_data, SBDError *error) {};
};

class SBDDeleteUserMetaDataInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDDeleteUserAllMetaDataInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

/**
 *  The `SBDUser` class represents a user. The user is identified by the `user_id`, so the `user_id` has to be unique. The `user_id`, `nickname` and `profile_url` are valid in every `SBDUser` instance, however the `connection_status` and `GetLastSeenAt()` is valid in `SBDUser` instance from `SBDUserListQuery`.
 */
class SBDUser {
public:
    /**
     *  User ID. This has to be unique.
     */
    wstring user_id;
    
    /**
     *  User nickname.
     */
    wstring nickname;
    
    /**
     *  Profile image url.
     */
    wstring profile_url;
    
    /**
     *  User connection status. This is defined in `SBDUserConnectionStatus`.
     */
    SBDUserConnectionStatus connection_status;
    
    /**
     *  The lastest time when the user became offline.
     */
    int64_t last_seen_at;

    /**
     *  Meta data.
     */
    map<wstring, wstring> meta_data;
    
    /**
     *  Creates the meta <span>data</span> for the current user.
     *
     *  @param meta_data          The meta <span>data</span> to be set.
     *  @param completion_handler The handler interface to execute. `meta_data` is the meta <span>data</span> which are set on SendBird server.
     */
    void CreateMetaData(map<wstring, wstring> meta_data, SBDCreateUserMetaDataInterface *completion_handler);
    
    /**
     *  Updates the meta <span>data</span> for the current user.
     *
     *  @param meta_data          The meta <span>data</span> to be updated.
     *  @param completion_handler The handler interface to execute. `meta_data` is the meta <span>data</span> which are updated on SendBird server.
     */
    void UpdateMetaData(map<wstring, wstring> meta_data, SBDUpdateUserMetaDataInterface *completion_handler);
    
    /**
     *  Deletes meta <span>data</span> with key for the current user.
     *
     *  @param key               The key to be deleted.
     *  @param completion_handler The handler interface to execute.
     */
    void DeleteMetaData(wstring key, SBDDeleteUserMetaDataInterface *completion_handler);
    
    /**
     *  Deletes all meta <span>data</span> for the current user.
     *
     *  @param completion_handler The handler interface to execute.
     */
    void DeleteAllMetaData(SBDDeleteUserAllMetaDataInterface *completion_handler);

	/**
	 *  Internal use only.
	 */
	SBDUser();

	/**
	 *  Internal use only.
	 */
	SBDUser(string dict);

	/**
	 *  Internal use only.
	 */
	~SBDUser();
};


#endif /* SENDBIRD_SBDUSER_H_ */
