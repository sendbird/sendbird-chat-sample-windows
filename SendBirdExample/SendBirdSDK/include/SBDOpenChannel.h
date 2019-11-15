//
//  SBDOpenChannel.h
//  SendBird
//
//  Created by Jed Gyeong on 6/28/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDOPENCHANNEL_H_
#define SENDBIRD_SBDOPENCHANNEL_H_

#include <iostream>
#include <vector>
#include <map>

#include "SBDBaseChannel.h"
#include "SBDUser.h"
#include "SBDError.h"
#include "SBDApiClient.h"
#include "SBDOpenChannelListQuery.h"
#include "SBDUserListQuery.h"
#include "SBDCrossThread.h"

class SBDOpenChannel;
class SBDOpenChannelListQuery;

using namespace std;

class SBDEnterOpenChannelInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDExitOpenChannelInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDGetOpenChannelInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDOpenChannel *channel, SBDError *error) {};
};

class SBDCreateOpenChannelInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDOpenChannel *channel, SBDError *error) {};
};

class SBDUpdateOpenChannelInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDOpenChannel *channel, SBDError *error) {};
};

class SBDRefreshOpenChannelInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDBanUserInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDUnbanUserInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDMuteUserInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDUnmuteUserInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

/**
 *  The `SBDOpenChannel` class represents a public chat. This is a channel type which anyone can participate without a permission. It can handle thousands of users in one channel. This channel has participants instead of members of `SBDGroupChannel` and every participant is always online. A user can be included as an operator when a channel is created. The operator has a permission to mute, unmute, ban and unban participants. The muted user can see the messages in the channel, but can't say anything. The unbanned user can't enter the channel. If a user who is in the channel is banned, the user will be kicked from the channel.
 */
class SBDOpenChannel : public SBDBaseChannel {
private:
	static SBDOpenChannel *Upsert(SBDOpenChannel *channel);
	void Overwrite(SBDOpenChannel *channel);

public:
    /**
     *  The number of participants in this channel.
     */
    int64_t participant_count;
    
    /**
     *  The operators of this channel.
     */
    vector<SBDUser> operators;
    
    /**
     The frozen state of this channel.
     */
    bool is_frozen;

    /**
     *  Creates a query instance for open channel list.
     *
     *  @return The query instance for open channel list.
     */
    static SBDOpenChannelListQuery *CreateOpenChannelListQuery();

    /**
     *  Updates an open channel with properties.
     *
     *  @param name              The name of the channel.
     *  @param channel_url        The channel URL. If the length of url is zero, channel url will be determined randomly.
     *  @param cover_url          The cover image URL.
     *  @param data              The data for channel.
     *  @param operator_user_ids     The operator userIds of channel.
     *  @param custom_type        The custom type for channel.
     *  @param completion_handler The handler interface to execute.
     */
    static void CreateChannel(const wstring& name, const wstring& channel_url, const wstring& cover_url, const wstring& data, const vector<wstring>& operator_user_ids, const wstring& custom_type, SBDCreateOpenChannelInterface *completion_handler);
    
	/**
	 *  Updates an open channel with properties.
	 *
	 *  @param name              The name of the channel.
	 *  @param channel_url        The channel URL. If the length of url is zero, channel url will be determined randomly.
	 *  @param cover_image_file_path          The cover image file path.
	 *  @param file_mime_type          The file mime type.
	 *  @param data              The data for channel.
	 *  @param operator_user_ids     The operator userIds of channel.
	 *  @param custom_type        The custom type for channel.
	 *  @param completion_handler The handler interface to execute.
	 */
	static void CreateChannel(const wstring& name, const wstring& channel_url, const wstring& cover_image_file_path, const wstring& file_mime_type, const wstring& data, const vector<wstring>& operator_user_ids, const wstring& custom_type, SBDCreateOpenChannelInterface *completion_handler);

    /**
     *  Updates an open channel.
     *
     *  @param new_name              The name of the channel.
     *  @param new_cover_url          The cover image URL.
     *  @param new_data              The data for channel.
     *  @param new_operator_user_ids   The operator user IDs of channel.
     *  @param new_custom_type        The custom type for channel.
     *  @param completion_handler The handler interface to execute.
     */
    void UpdateChannel(const wstring& new_name, const wstring& new_cover_url, const wstring& new_data, const vector<wstring>& new_operator_user_ids, const wstring& new_custom_type, SBDUpdateOpenChannelInterface *completion_handler);
    
	/**
	 *  Updates an open channel.
	 *
	 *  @param new_name              The name of the channel.
	 *  @param new_cover_image_file_path          The cover image file path.
	 *  @param new_cover_file_mime_type          The file mime type.
	 *  @param new_data              The data for channel.
	 *  @param new_operator_user_ids   The operator user IDs of channel.
	 *  @param new_custom_type        The custom type for channel.
	 *  @param completion_handler The handler interface to execute.
	 */
	void UpdateChannel(const wstring& new_name, const wstring& new_cover_image_file_path, const wstring& new_cover_file_mime_type, const wstring& new_data, const vector<wstring>& new_operator_user_ids, const wstring& new_custom_type, SBDUpdateOpenChannelInterface *completion_handler);

    /**
     *  Gets an open channel instance from channel URL.
     *
     *  @param channel_url        The channel URL.
     *  @param completion_handler The handler interface to execute.
     */
    static void GetChannel(const wstring& channel_url, SBDGetOpenChannelInterface *completion_handler);
    
    /**
     *  Enters the channel.
     *
     *  @param completion_handler The handler interface to execute.
     */
    void Enter(SBDEnterOpenChannelInterface *completion_handler);
    
    /**
     *  Exits the channel.
     *
     *  @param completion_handler The handler interface to execute.
     */
    void Exit(SBDExitOpenChannelInterface *completion_handler);
    
    /**
     *  Creates a query instance for getting participant list of the channel instance.
     *
     *  @return SBDUserListQuery instance for the participant list of the channel instance.
     */
    SBDUserListQuery *CreateParticipantListQuery();
    
    /**
     *  Creates a query instance for getting muted user list of the channel instance.
     *
     *  @return SBDUserListQuery instance for the muted user list.
     */
    SBDUserListQuery *CreateMutedUserListQuery();
    
    /**
     *  Creates a query instance for getting banned user list of the channel instance.
     *
     *  @return SBDUserListQuery instance for the banned user list.
     */
    SBDUserListQuery *CreateBannedUserListQuery();
    
    /**
     *  Refreshes the channel information.
     *
     *  @param completion_handler The handler interface to execute.
     */
    void RefreshChannel(SBDRefreshOpenChannelInterface *completion_handler);
    
    /**
     *  Bans a user with the user object.
     *
     *  @param user              The user object.
     *  @param seconds           Duration for ban in seconds.
     *  @param completion_handler The handler interface to execute.
     */
    void BanUser(SBDUser user, int seconds, SBDBanUserInterface *completion_handler);
    
    /**
     *  Unbans a user with the user object.
     *
     *  @param user              The user object.
     *  @param completion_handler The handler interface to execute.
     */
    void UnbanUser(SBDUser user, SBDUnbanUserInterface *completion_handler);
    
    /**
     *  Mutes auser with the user object.
     *
     *  @param user              The user object.
     *  @param completion_handler The handler block to execute.
     */
    void MuteUser(SBDUser user, SBDMuteUserInterface *completion_handler);
    
    /**
     *  Unmutes a user with the user object.
     *
     *  @param user              The user object.
     *  @param completion_handler The handler block to execute.
     */
    void UnmuteUser(SBDUser user, SBDUnmuteUserInterface *completion_handler);
    
    /**
     *  Returns the user is an operator or not.
     *
     *  @param user The user object.
     *
     *  @return If true, the user is an operator.
     */
    bool IsOperator(SBDUser user);

	/**
	 *  Internal use only.
	 */
	static pthread_mutex_t cached_channels_lock;

	/**
	 *  Internal use only.
	 */
	static pthread_mutex_t entered_channels_lock;

	/**
	 *  Internal use only.
	 */
	static map<wstring, SBDOpenChannel *> cached_channels;

	/**
	 *  Internal use only.
	 */
	static map<wstring, SBDOpenChannel *> entered_channels;

	/**
	 *  Internal use only.
	 */
	static void ClearCache();

	/**
	 *  Internal use only.
	 */
	static void ClearEnteredChannels();

	/**
	 *  Internal use only.
	 */
	static void RemoveChannelFromCache(const wstring& channel_url);

	/**
	 *  Internal use only.
	 */
	SBDOpenChannel(const string& dict);

	/**
	 *  Internal use only.
	 */
	const map<wstring, SBDOpenChannel *> *GetEnteredChannels();

	/**
	 *  Internal use only.
	 */
	static SBDOpenChannel *BuildAndUpsert(const string& dict);

	/**
	 *  Internal use only.
	 */
	SBDOpenChannel();
};

#endif /* SENDBIRD_SBDOPENCHANNEL_H_ */
