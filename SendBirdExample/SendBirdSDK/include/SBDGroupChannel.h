//
//  SBDGroupChannel.h
//  SendBird
//
//  Created by Jed Gyeong on 6/28/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDGROUPCHANNEL_H_
#define SENDBIRD_SBDGROUPCHANNEL_H_

#include <iostream>
#include <vector>
#include <map>

#include "SBDBaseChannel.h"
#include "SBDBaseMessage.h"
#include "SBDUser.h"

#include "SBDError.h"
#include "SBDReadStatus.h"
#include "SBDMember.h"
#include "SBDGroupChannelListQuery.h"

#include "SBDCrossThread.h"

class SBDUser;
class SBDGroupChannel;
class SBDGroupChannelListQuery;

using namespace std;

class SBDGetGroupChannelFromServerInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDGroupChannel *channel, SBDError *error) {};
};

class SBDGetGroupChannelInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDGroupChannel *channel, SBDError *error) {};
};

class SBDRefreshGroupChannelInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDCreateGroupChannelInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDGroupChannel *channel, SBDError *error) {};
};

class SBDUpdateGroupChannelInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDGroupChannel *channel, SBDError *error) {};
};

class SBDInviteUsersInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDHideGroupChannelInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDUnhideGroupChannelInterface : public SBDBaseInterface {
public:
	virtual void CompletionHandler(SBDError* error) {};
};

class SBDLeaveGroupChannelInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDAcceptInvitationInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDDeclineInvitationInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDMarkAllAsReadInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDResetMyHistoryInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDGetGroupChannelCountInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(uint64_t group_chanel_count, SBDError *error) {};
};

/**
 *  The `SBDGroupChannel` class represents a group channel which is a private chat. The user who wants to join the group channel has to be invited by another user who is already joined the channel. This class is derived from `SBDBaseChannel`. If the `SBDChannelInterface` is added, the user will automatically receive all messages from the group channels where the user belongs after connection. The `SBDGroupChannel` provides the features of general messaging apps.
 *
 *  * Typing indicator.
 *  * Read status for each message.
 *  * Unread message count in the channel.
 *  * Lastest message in the channel.
 *
 *  The `SBDGroupChannel` has a special property, `is_distinct`. The distinct property enabled group channel is always reuesd for same channel <span>members</span>. If a new member gets invited or the member left from the channel, then the distinct property disabled automatically. If you don't set distinct property on the channel, it always create new channel for the same <span>members</span>.
 *
 *  For more information, see [Group Channel](https://docs.sendbird.com/ios#group_channel).
 *
 */
class SBDGroupChannel : public SBDBaseChannel {
private:
    int64_t last_start_typing_ts;
    int64_t last_end_typing_ts;
    int64_t last_manual_mark_as_read_ts;
    map<wstring, SBDMember> member_map;
    map<wstring, int64_t> cached_read_receipt_status;
    map<wstring, int64_t> cached_typing_status;
    static SBDGroupChannel *Upsert(SBDGroupChannel *channel);
    void Overwrite(SBDGroupChannel *channel);
    
public:
    /**
     *  The Last message's ID of the channel.
     */
	int64_t last_message_id;
    
    /**
     *  Represents the channel is distinct or not.
     */
    bool is_distinct;
    
    /**
     *  Unread message count of the channel.
     */
    uint64_t unread_message_count;
    
    /**
     *  Channel <span>members</span>.
     */
    vector<SBDMember> members;
    
    /**
     *  The number of <span>members</span>.
     */
    uint64_t member_count;

	/**
	   The inviter.
	 */
	SBDUser inviter;

	/*
	 *  If there is a inviter, true.
	 */
	bool has_inviter;

	/**
	 * Checks this channel is hidden
	 */
	bool is_hidden;

	/**
	 * The hidden state
	 */
	SBDGroupChannelHiddenState hidden_state;

    /**
     *  Create a group channel with user IDs. The group channel is distinct.
     *
     *  @param user_ids           The user IDs to be <span>members</span> of the channel.
     *  @param name              The name of group channel.
     *  @param is_distinct        If true, the channel which has the same users is returned.
     *  @param cover_url          The cover image url of group channel.
     *  @param data              The custom data of group channel.
     *  @param custom_type        The custom type of group channel.
     *  @param completion_handler The handler interface to execute.
     */
    static void CreateChannel(vector<wstring> user_ids, const wstring& name, bool is_distinct, const wstring& cover_url, const wstring& data, const wstring& custom_type, SBDCreateGroupChannelInterface *completion_handler);
    
	/**
	 *  Create a group channel with user IDs with cover image file. The group channel is distinct.
	 *
	 *  @param user_ids           The user IDs to be <span>members</span> of the channel.
	 *  @param name              The name of group channel.
	 *  @param is_distinct        If true, the channel which has the same users is returned.
	 *  @param cover_image_file_path          The cover image file path of group channel.
	 *  @param file_mime_type	The file mime type.
	 *  @param data              The custom data of group channel.
	 *  @param custom_type        The custom type of group channel.
	 *  @param completion_handler The handler interface to execute.
	 */
	static void CreateChannel(const vector<wstring> user_ids, const wstring& name, bool is_distinct, const wstring& cover_image_file_path, const wstring& file_mime_type, const wstring& data, const wstring& custom_type, SBDCreateGroupChannelInterface *completion_handler);

    /**
     *  Updates a group channel.
     *
     *  @param name              The name of group channel.
     *  @param is_distinct        If true, the channel which has the same users is returned.
     *  @param cover_url          The cover image url of group channel.
     *  @param data              The custom data of group channel.
     *  @param custom_type        The custom type of group channel.
     *  @param completion_handler The handler interface to execute.
     */
    void UpdateChannel(const wstring& name, bool is_distinct, const wstring& cover_url, const wstring& data, const wstring& custom_type, SBDUpdateGroupChannelInterface *completion_handler);

	/**
	 *  Updates a group channel with file path.
	 *
	 *  @param name              The name of group channel.
	 *  @param is_distinct        If true, the channel which has the same users is returned.
	 *  @param cover_image_file_path          The cover image file path of group channel.
	 *  @param file_mime_type	The file mime type.
	 *  @param data              The custom data of group channel.
	 *  @param custom_type        The custom type of group channel.
	 *  @param completion_handler The handler interface to execute.
	 */
	void UpdateChannel(const wstring& name, bool is_distinct, const wstring& cover_image_file_path, const wstring& file_mime_type, const wstring& data, const wstring& custom_type, SBDUpdateGroupChannelInterface *completion_handler);

	/**
	*  Refreshes this channel instance.
	*
	*  @param completion_handler The handler interface to execute.
	*/
	void RefreshChannel(SBDRefreshGroupChannelInterface *completion_handler);

	/**
	*  Creates a query for my group channel list.
	*
	*  @return SBDGroupChannelListQuery instance for the current user.
	*/
	static SBDGroupChannelListQuery *CreateMyGroupChannelListQuery();

    /**
     *  Gets a group channel instance with channel URL from server asynchronously.
     *
     *  @param channel_url        The channel URL.
     *  @param completion_handler The handler interface to execute.
     */
    static void GetChannelFromServer(const wstring& channel_url, SBDGetGroupChannelFromServerInterface *completion_handler);
    
    /**
     *  Gets a group channel instance from channel URL asynchronously.
     *
     *  @param channel_url        The channel URL.
     *  @param completion_handler The handler interface to execute.
     */
    static void GetChannel(const wstring& channel_url, SBDGetGroupChannelInterface *completion_handler);
    
    /**
     *  Invites multiple users to the group channel.
     *
     *  @param users              The users to be invited.
     *  @param completion_handler The handler interface to execute.
     */
    void InviteUsers(const vector<SBDUser>& users, SBDInviteUsersInterface *completion_handler);
    
    /**
     *  Hides the group channel. The channel will be hid from the channel list, but it will be appeared again when the other user send a message in the channel.
     *
     *  @param hide_prev_messages The option to hide the previous message of this channel when the channel will be appeared again.
     *  @param completion_handler The handler interface to execute.
     */
    void HideChannel(bool hide_prev_messages, SBDHideGroupChannelInterface *completion_handler);

	/**
	*	Hides the group channel with the auto unhide option. The channel will be hid from the channel list. 
	*	If the allowAutoUnhide is YES, the channel will be appeared again when the other user send a message in the channel. 
	*	However, if the allowAutoUnhide is NO, the channel will be appeared by UnhideChannel method.
	*/
	void HideChannel(bool hide_prev_messages, bool allow_auto_unhide, SBDHideGroupChannelInterface* completion_handler);

    /**
	*	Unhides the group channel.
	*/
	void UnhideChannel(SBDUnhideGroupChannelInterface* completion_handler);

    /**
     *  Leaves the group channel. The channel will be disappeared from the channel list. If join the channel, the invitation is required.
     *
     *  @param completion_handler The handler block to execute.
     */
    void LeaveChannel(SBDLeaveGroupChannelInterface *completion_handler);
    
    /**
     *  Marks as read all group channels of the current user.
     *
     *  @param completion_handler The handler interface to execute.
     */
    static void MarkAllAsRead(SBDMarkAllAsReadInterface *completion_handler);
    
    /**
     *  Sends mark as read. The other <span>members</span> in the channel will receive an event. The event will be received in `ReadReceiptUpdated()` of `SBDChannelInterface`.
     */
    void MarkAsRead();
    
    /**
     *  Starts typing. The other <span>members</span> in the channel will receive an event. The event will be received in `TypingStatusUpdated()` of `SBDChannelInterface`.
	 *  NOT TESTED
     */
    void StartTyping();
    
    /**
     *  Ends typing. The other <span>members</span> in the channel will receive an event. The event will be received in `TypingStatusUpdated()` of `SBDChannelInterface`.
	 *  NOT TESTED
     */
    void EndTyping();

    /**
     *  Returns how many <span>members</span> haven't been read the given message yet.
     *
     *  @param message The message.
     *
     *  @return Number of unread member count. Zero if all <span>members</span> read the message.
     */
    int GetReadReceipt(SBDBaseMessage *message);
    
    /**
     *  Returns the timestamp of the last seen at the channel by user.
     *
     *  @param user The user
     *
     *  @return the timestamp of the last seen at.
     */
    int64_t GetLastSeenAt(SBDUser user);
    
    /**
     *  Returns the <span>members</span> who read the message.
     *
     *  @param message The message.
     *
     *  @return Members who read the message.
     */
    vector<SBDMember> GetReadMembers(SBDBaseMessage *message);
    
    /**
     *  Returns the <span>members</span> who didn't read the message.
     *
     *  @param message The message.
     *
     *  @return Members who don't read the message.
     */
    vector<SBDMember> GetUnreadMembers(SBDBaseMessage *message);
    
    /**
     *  Returns the read status.
     *
     *  @return The read status. If there's no data, it will be empty.
     */
    map<wstring, SBDReadStatus> GetReadStatus();
    
    /**
     *  If other users are typing in the channel, true is returned.
     *
     *  @return Returns true when other users are typing in this channel.
	 *  NOT TESTED
     */
    bool IsTyping();
    
    /**
     *  Returns the <span>members</span> who are typing now.
     *
     *  @return The <span>members</span> who are typing now.
	 *  NOT TESTED
     */
    vector<SBDMember> GetTypingMembers();
    
    /**
     *  Checks if the channel has a member that has `user_id`.
     *
     *  @param user_id User ID.
     *  @return If true, the channel has the member.
     */
    bool HasMember(const wstring& user_id);
    
    /**
     *  Gets member in the channel.
     *
     *  @param user_id User ID.
	 *  @param member_exist If the member exists, it is true.
     *  @return `SBDUser` object as a member. If there is a member who has the `user_id`, returns NULL.
     */
    SBDMember GetMember(const wstring& user_id, bool *member_exist);

    /**
     *  Accpets the invitation.
     *
     *  @param completion_handler The handler interface to execute.
     */
    void AcceptInvitation(SBDAcceptInvitationInterface *completion_handler);
    
    /**
     *  Declines the invitation.
     *
     *  @param completion_handler The handler interface to execute.
     */
    void DeclineInvitation(SBDDeclineInvitationInterface *completion_handler);

    /**
     *  Resets the history in this channel.
     *
     *  @param completion_handler The handler block to execute.
     */
    void ResetMyHistory(SBDResetMyHistoryInterface *completion_handler);
    
    /**
     *  Gets the group channel count.
     *
     *  @param member_state_filter The member state of the current user in the channels that are counted.
     *  @param completion_handler The handler interface to execute.
     */
    static void GetChannelCount(SBDMemberStateFilter member_state_filter, SBDGetGroupChannelCountInterface *completion_handler);

	/**
	 *  Gets the last message of this channel. If there is not the last message, NULL will be returned.
	 *
	 *  @return The last message object.
	 */
	const SBDBaseMessage *GetLastMessage();

	/**
	 *  Internal use only.
	 */
	static map<wstring, SBDGroupChannel *> cached_channels;

	/**
	 *  Internal use only.
	 */
	static pthread_mutex_t cached_channel_lock;

	/**
	 *  Internal use only.
	 */
	bool has_been_updated;

	/**
	 *  Internal use only.
	 */
	SBDGroupChannel(const string& dict);

	/**
	 *  Internal use only.
	 */
	~SBDGroupChannel();

	/**
	 *  Internal use only.
	 */
	static SBDGroupChannel *BuildAndUpsert(string dict);

	/**
	 *  Internal use only.
	 */
	void UpdateTypingStatus(SBDUser user, bool start);

	/**
	 *  Internal use only.
	 */
	void AddMember(SBDMember user);

	/**
	 *  Internal use only.
	 */
	void RemoveMember(SBDMember user);

	/**
	 *  Internal use only.
	 */
	void TypingStatusTimeout();

	/**
	 *  Internal use only.
	 */
	static void UpdateTypingStatus();

	/**
	 *  Internal use only.
	 */
	static void ClearCache();

	/**
	 *  Internal use only.
	 */
	static void RemoveChannelFromCache(const wstring& channel_url);

	/**
	 *  Internal use only.
	 */
	static SBDGroupChannel *GetChannelFromCache(const wstring& channel_url);

	/**
	*  Internal use only.
	*/
	void UpdateReadReceipt(const wstring& user_id, int64_t timestamp);

	/**
	 *  Internal use only.
	 */
	static bool HasChannelInCache(const wstring& channel_url);

	/**
	 *  Internal use only.
	 */
	static int64_t last_mark_all_as_read_ts;

	/**
	 *  Internal use only.
	 */
	void SetChannelHiddenState(SBDGroupChannelHiddenState hidden_state);

};

#endif /* SENDBIRD_SBDGROUPCHANNEL_H_ */
