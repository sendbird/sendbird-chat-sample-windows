//
//  SBDBaseChannel.h
//  SendBird
//
//  Created by Jed Gyeong on 6/28/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDBASECHANNEL_H_
#define SENDBIRD_SBDBASECHANNEL_H_

#include <iostream>
#include <vector>
#include <map>

#include "SBDUser.h"
#include "SBDBaseMessage.h"
#include "SBDUserMessage.h"
#include "SBDAdminMessage.h"
#include "SBDFileMessage.h"
#include "SBDError.h"

#include "SBDTypes.h"

#include "SBDCrossThread.h"

using namespace std;

class SBDPreviousMessageListQuery;
class SBDThumbnailSize;
class SBDThumbnail;
class SBDFileMessage;
class SBDUserMessage;
class SBDGroupChannel;
class SBDOpenChannel;


/**
 *  An object that adopts the `SBDChannelInterface` class is responsible for receiving the events in the channel. Some of virtual methods are common for the `SBDBaseChannel`. However, there are virtual methods for the `SBDOpenChannel` and `SBDGroupChannel` exclusive. The `SBDChannelInterface` can be added by [`void AddChannelHandler(SBDChannelInterface *handler, string identifier)`]() in `SBDMain`. Every `SBDChannelInterface` method which is added is going to receive events.
 *
 *  @warning If the object that adopts the `SBDChannelInterface` class is invalid, the delegate has to be removed by the identifier via [`void RemoveChannelHandler(string identifier)`]() in `SBDMain`. If you miss this, it will cause the crash.
 *
 *  This class deals with the below events.
 *
 *  * Receives a message in the [`SBDBaseChannel`]().
 *  * Receives an event when a message is updated in the [`SBDBaseChannel`]().
 *  * Receives an event when a member read a message in the [`SBDGroupChannel`]().
 *  * Receives an event when a member typed something in the [`SBDGroupChannel`]().
 *  * Receives an event when a new member joined the [`SBDGroupChannel`]().
 *  * Receives an event when a member left from the [`SBDGroupChannel`]().
 *  * Receives an event when a participant entered the [`SBDOpenChannel`]().
 *  * Receives an event when a participant exited the [`SBDOpenChannel`]().
 *  * Receives an event when a participant was muted or unmuted in the [`SBDOpenChannel`]().
 *  * Receives an event when a participant was banned or unbanned in the [`SBDOpenChannel`]().
 *  * Receives an event when the [`SBDOpenChannel`]() was frozen or unfrozen.
 *  * Receives an event when the property of the [`SBDBaseChannel`]() was changed.
 *  * Receives an event when the [`SBDBaseChannel`]() was deleted.
 *  * Receives an event when a message in the [`SBDBaseChannel`]() was deleted.
 *  * Receives an event when meta data in the [`SBDBaseChannel`]() was changed.
 *  * Receives an event when meta counters in the [`SBDBaseChannel`]() were changed.
 */
class SBDChannelInterface {
public:
    /**
     *  A callback when a message is received.
     *
     *  @param channel The channel where the message is received.
     *  @param message The received message.
     */
    virtual void MessageReceived(SBDBaseChannel *channel, SBDBaseMessage *message) {};
    
    /**
     A callback when a message is updated.
     
     @param channel The channel where the message is updated.
     @param message The updated message.
     */
    virtual void MessageUpdated(SBDBaseChannel *channel, SBDBaseMessage *message) {};
    
    /**
     *  A callback when read receipts updated.
     *
     *  @param channel The group channel where the read receipt updated.
     */
    virtual void ReadReceiptUpdated(SBDGroupChannel *channel) {};
    
    
    /**
     *  A callback when user sends typing status.
     *
     *  @param channel The group channel where the typing status updated.
     */
    virtual void TypingStatusUpdated(SBDGroupChannel *channel) {};
    
    /**
     A callback when users are invited by inviter.
     
     @param channel The group channel where the invitation is occured.
     @param invitees Invitees.
     @param inviter Inviter. It can be NULL.
     */
    virtual void InvitationReceived(SBDGroupChannel *channel, vector<SBDUser> invitees, SBDUser inviter) {};
    
    /**
     A callback when user declined the invitation.
     
     @param channel The group channel where the invitation is occured.
     @param invitee Inviter. It can be NULL.
     @param inviter Invitee.
     */
    virtual void InvitationDeclined(SBDGroupChannel *channel, SBDUser invitee, SBDUser inviter) {};

    /**
     *  A callback when new member joined to the group channel.
     *
     *  @param channel The group channel.
     *  @param user   The new user joined to the channel.
     */
    virtual void UserJoined(SBDGroupChannel *channel, SBDUser user) {};
    
    /**
     *  A callback when current member left from the group channel.
     *
     *  @param channel The group channel.
     *  @param user   The member left from the channel.
     */
    virtual void UserLeft(SBDGroupChannel *channel, SBDUser user) {};
    
    /**
     *  A callback when a user enter an open channel.
     *
     *  @param channel The open channel.
     *  @param user   The user
     */
    virtual void UserEntered(SBDOpenChannel *channel, SBDUser user) {};
    
    /**
     *  A callback when a user exit an open channel.
     *
     *  @param channel The open channel.
     *  @param user   The user.
     */
    virtual void UserExited(SBDOpenChannel *channel, SBDUser user) {};
    
    /**
     *  A callback when a user was muted in the open channel.
     *
     *  @param channel The open channel.
     *  @param user   The user who was muted.
     */
    virtual void UserMuted(SBDOpenChannel *channel, SBDUser user) {};
    
    /**
     *  A callback when a user was unmuted in the open channel.
     *
     *  @param channel The open channel.
     *  @param user   The user who was unmuted.
     */
    virtual void UserUnmuted(SBDOpenChannel *channel, SBDUser user) {};
    
    /**
     *  A callback when a user was banned in the open channel.
     *
     *  @param channel The open channel.
     *  @param user   The user who was banned.
     */
    virtual void UserBanned(SBDOpenChannel *channel, SBDUser user) {};
    
    /**
     *  A callback when a user was unbanned in the open channel.
     *
     *  @param channel The open channel.
     *  @param user   The user who was unbanned.
     */
    virtual void UserUnbanned(SBDOpenChannel *channel, SBDUser user) {};
    
    /**
     *  A callback when an open channel was frozen.
     *
     *  @param channel The open channel.
     */
    virtual void ChannelFrozen(SBDOpenChannel *channel) {};
    
    /**
     *  A callback when an open channel was unfrozen.
     *
     *  @param channel The open channel.
     */
    virtual void ChannelUnfrozen(SBDOpenChannel *channel) {};
    
    /**
     *  A callback when an open channel was changed.
     *
     *  @param channel The open channel.
     */
    virtual void ChannelChanged(SBDBaseChannel *channel) {};
    
    /**
     *  A callback when an open channel was deleted.
     *
     *  @param channel_url The open channel.
     */
    virtual void ChannelDeleted(wstring channel_url, SBDChannelType channel_type) {};
    
    /**
     *  A callback when a message was removed in the channel.
     *
     *  @param channel    The base channel.
     *  @param message_id The message ID which was removed.
     */
    virtual void MessageDeleted(SBDBaseChannel *channel, uint64_t message_id) {};
    
    /**
     A callback when meta data was created in the channel.
     
     @param channel The channel that the meta data was created.
     @param created_meta_data The created meta data.
     */
	virtual void ChannelMetaDataCreated(SBDBaseChannel *channel, map<wstring, wstring> created_meta_data) {};
    
    /**
     A callback when meta data was updated in the channel.
     
     @param channel The channel that the meta data was updated.
     @param updated_meta_data The updated meta data.
     */
	virtual void ChannelMetaDataUpdated(SBDBaseChannel *channel, map<wstring, wstring> updated_meta_data) {};
    
    /**
     A callback when meta data was deleted in the channel.
     
     @param channel The channel that the meta data was deleted.
     @param deleted_meta_data The keys of the deleted meta data.
     */
	virtual void ChannelMetaDataDeleted(SBDBaseChannel *channel, vector<wstring> deleted_meta_data) {};
    
    /**
     A callback when meta counters were created in the channel.
     
     @param channel The channel that the meta counters were created.
     @param created_meta_counters The created meta counters.
     */
	virtual void ChannelMetaCountersCreated(SBDBaseChannel *channel, map<wstring, int64_t> created_meta_counters) {};
    
    /**
     A callback when meta counters were updated in the channel.
     
     @param channel The channel that the meta counters were updated.
     @param updated_meta_counters The updated meta counters.
     */
	virtual void ChannelMetaCountersUpdated(SBDBaseChannel *channel, map<wstring, int64_t> updated_meta_counters) {};
    
    /**
     A callback when meta counters were deleted in the channel.
     
     @param channel The channel that the meta counters were deleted.
     @param deleted_meta_counters The keys of the deleted meta counters.
     */
	virtual void ChannelMetaCountersDeleted(SBDBaseChannel *channel, vector<wstring> deleted_meta_counters) {};
};

class SBDSendUserMessageInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDUserMessage *user_message, SBDError *error) {};
};

class SBDSendFileMessageInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDFileMessage *file_message, SBDError *error) {};
};

class SBDCreateChannelMetaCountersInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(map<wstring, int64_t> meta_counters, SBDError *error) {};
};

class SBDGetChannelMetaCountersInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(map<wstring, int64_t> meta_counters, SBDError *error) {};
};

class SBDGetChannelAllMetaCountersInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(map<wstring, int64_t> meta_counters, SBDError *error) {};
};

class SBDUpdateChannelMetaCountersInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(map<wstring, int64_t> meta_counters, SBDError *error) {};
};

class SBDIncreaseChannelMetaCountersInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(map<wstring, int64_t> meta_counters, SBDError *error) {};
};

class SBDDecreaseChannelMetaCountersInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(map<wstring, int64_t> meta_counters, SBDError *error) {};
};

class SBDDeleteChannelMetaCounterInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDDeleteChannelAllMetaCountersInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDCreateChannelMetaDataInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(map<wstring, wstring> meta_data, SBDError *error) {};
};

class SBDGetChannelMetaDataInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(map<wstring, wstring> meta_data, SBDError *error) {};
};

class SBDGetChannelAllMetaDataInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(map<wstring, wstring> meta_data, SBDError *error) {};
};

class SBDUpdateChannelMetaDataInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(map<wstring, wstring> meta_data, SBDError *error) {};
};

class SBDDeleteChannelMetaDataInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDDeleteChannelAllMetaDataInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDDeleteMessageInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDUpdateUserMessageInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDUserMessage *user_message, SBDError *error) {};
};

class SBDUpdateFileMessageInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDFileMessage *file_message, SBDError *error) {};
};

class SBDGetMessagesInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(vector<SBDBaseMessage *> messages, SBDError *error) {};
};

class SBDCopyUserMessageInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDUserMessage *user_message, SBDError *error) {};
};

class SBDCopyFileMessageInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDFileMessage *file_message, SBDError *error) {};
};

/**
 *  The `SBDBaseChannel` class represents the channel where users chat each other. The `SBDOpenChannel` and the `SBDGroupChannel` are derived from this class. This class provides the common methods for the `SBDOpenChannel` and the `SBDGroupChannel`.
 *
 *  * Send a user message to the channel.
 *  * Send a file message to the channel.
 *  * Delete a message of the channel.
 *  * Create a query for loading messages of the channel.
 *  * Manipulate meta counters and meta <span>data</span> of the channel.
 *
 *  The channel objects are maintained as a single instance in an application. If you create or get channels from the same channel URL, they must be the same instances.
 */
class SBDBaseChannel {
public:
    /**
     *  The channel URL.
     */
    wstring channel_url;
    
    /**
     *  The name of channel.
     */
    wstring name;
    
    /**
     *  The channel cover image URL.
     */
    wstring cover_url;
    
    /**
     *  The timestamp when the channel is created.
     */
    int64_t created_at;
    
    /**
     *  The custom date of the channel.
     */
    wstring data;
    
    /**
     *  The custom type of the channel.
     */
    wstring custom_type;
    
    /**
     *  Checks the channel type. If true, this channel is a group channel.
     */
    bool is_group_channel;
    
    /**
     *  Checks the channel type. If true, this channel is an open channel.
     */
    bool is_open_channel;
    
    /**
     *  Sends a user message without <span>data</span>.
     *
     *  @param message           The message text.
     *  @param data           The message <span>data</span>.
     *  @param custom_type     Custom message type.
     *  @param target_languages   The target languages that the message will be translated into.
     *  @param completion_handler The handler interface to execute.
     *
     *  @return Returns the temporary user message with a request ID. It doesn't have a message ID.
     */
    SBDUserMessage *SendUserMessage(wstring message, wstring data, wstring custom_type, vector<wstring> target_languages, SBDSendUserMessageInterface *completion_handler);
    
    // TODO: SendFileMessage with binary data.
    
    /**
     *  Sends a file message with file URL and <span>custom message type</span>.
     *
     *  @param url               File URL.
     *  @param filename          File<span>name</span>.
     *  @param type              The type of file.
     *  @param size              File size.
     *  @param data              Custom <span>data</span>.
     *  @param custom_type        Custom message type.
     *  @param completion_handler The handler interface to execute.
     *
     *  @return Returns the temporary file message with a request ID. It doesn't have a message ID.
     */
    SBDFileMessage *SendFileMessage(wstring url, wstring filename, int64_t size, wstring type, wstring data, wstring custom_type, SBDSendFileMessageInterface *completion_handler);
    
    /**
     *  Deletes a message. The message's sender has to be the current user.
     *
     *  @param message           The message to be deleted.
     *  @param completion_handler The handler interface to execute.
     */
    void DeleteMessage(SBDBaseMessage *message, SBDDeleteMessageInterface *completion_handler);
    
    /**
     *  Updates a user message. The message text, data, and custom type can be updated.
     *
     *  @param message       The user message to be updated.
     *  @param message_text       New message text.
     *  @param data              New data.
     *  @param custom_type        New custom type.
     *  @param completion_handler The handler interface to execute.
     */
    void UpdateUserMessage(SBDUserMessage *message, wstring message_text, wstring data, wstring custom_type, SBDUpdateUserMessageInterface *completion_handler);
    
    /**
     *  Updates a file message. The data and custom type can be updated.
     *
     *  @param message       The file message to be updated.
     *  @param data              New data.
     *  @param custom_type        New custom type.
     *  @param completion_handler The handler interface to execute.
     */
    void UpdateFileMessage(SBDFileMessage *message, wstring data, wstring custom_type, SBDUpdateFileMessageInterface *completion_handler);
    
    /**
     *  Creates `SBDPreviousMessageListQuery` instance for getting the previous messages list of the channel.
     *
     *  @return Returns the message list of the channel.
     */
    SBDPreviousMessageListQuery *CreatePreviousMessageQuery();

    /**
     *  Creates the meta counters for the channel.
     *
     *  @param meta_counters      The meta counters to be set.
     *  @param completion_handler The handler interface to execute.
     */
    void CreateMetaCounters(map<wstring, int64_t> meta_counters, SBDCreateChannelMetaCountersInterface *completion_handler);
    
    /**
     *  Gets the meta counters with keys for the channel.
     *
     *  @param keys              The keys to get meta counters.
     *  @param completion_handler The handler interface to execute.
     */
    void GetMetaCounters(vector<wstring> keys, SBDGetChannelMetaCountersInterface *completion_handler);
    
    /**
     *  Gets all meta counters for the channel.
     *
     *  @param completion_handler The handler interface to execute.
     */
    void GetAllMetaCounters(SBDGetChannelAllMetaCountersInterface *completion_handler);
    
    /**
     *  Updates the meta counters for the channel.
     *
     *  @param meta_counters      The meta counters to be updated.
     *  @param completion_handler The handler interface to execute.
     */
    void UpdateMetaCounters(map<wstring, int64_t> meta_counters, SBDUpdateChannelMetaCountersInterface *completion_handler);
    
    /**
     *  Increases the meta counters for the channel.
     *
     *  @param meta_counters      The meta counters to be increased.
     *  @param completion_handler The handler interface to execute.
     */
    void IncreaseMetaCounters(map<wstring, int64_t> meta_counters, SBDIncreaseChannelMetaCountersInterface *completion_handler);
    
    /**
     *  Decreases the meta counters for the channel.
     *
     *  @param meta_counters      The meta counters to be decreased.
     *  @param completion_handler The handler interface to execute.
     */
    void DecreaseMetaCounters(map<wstring, int64_t> meta_counters, SBDDecreaseChannelMetaCountersInterface *completion_handler);
    
    /**
     *  Deletes the meta counters with key for the channel.
     *
     *  @param key               The key to be deleted.
     *  @param completion_handler The handler interface to execute.
     */
    void DeleteMetaCounter(wstring key, SBDDeleteChannelMetaCounterInterface *completion_handler);
    
    /**
     *  Deletes all meta counters for the channel.
     *
     *  @param completion_handler The handler interface to execute.
     */
    void DeleteAllMetaCounters(SBDDeleteChannelAllMetaCountersInterface *completion_handler);
    
    // Meta Data
    /**
     *  Creates the meta <span>data</span> for the channel.
     *
     *  @param meta_data          The meta <span>data</span> to be set.
     *  @param completion_handler The handler interface to execute.
     */
    void CreateMetaData(map<wstring, wstring> meta_data, SBDCreateChannelMetaDataInterface *completion_handler);
    
    /**
     *  Gets the meta <span>data</span> for the channel.
     *
     *  @param keys              The keys to get meta <span>data</span>.
     *  @param completion_handler The handler interface to execute.
     */
    void GetMetaData(vector<wstring> keys, SBDGetChannelMetaDataInterface *completion_handler);
    
    /**
     *  Gets all meta <span>data</span> for the channel.
     *
     *  @param completion_handler The handler interface to execute.
     */
    void GetAllMetaData(SBDGetChannelAllMetaDataInterface *completion_handler);
    
    /**
     *  Updates the meta <span>data</span> for the channel.
     *
     *  @param meta_data          The meta <span>data</span> to be updated.
     *  @param completion_handler The handler interface to execute.
     */
    void UpdateMetaData(map<wstring, wstring> meta_data, SBDUpdateChannelMetaDataInterface *completion_handler);
    
    /**
     *  Deletes meta <span>data</span> with key for the channel.
     *
     *  @param key               The key to be deleted.
     *  @param completion_handler The handler interface to execute.
     */
    void DeleteMetaData(wstring key, SBDDeleteChannelMetaDataInterface *completion_handler);
    
    /**
     *  Deletes all meta <span>data</span> for the channel.
     *
     *  @param completion_handler The handler interface to execute.
     */
    void DeleteAllMetaData(SBDDeleteChannelAllMetaDataInterface *completion_handler);
    
    /**
     *  Gets the next messages by the timestamp with a limit and ordering.
     *
     *  @param timestamp         The standard timestamp to load messages.
     *  @param next_limit             The limit for the number of messages. The returned messages could be more than this number if there are messages which have the same timestamp.
     *  @param reverse           If yes, the latest message is the index 0.
     *  @param message_type       Message type to filter messages.
     *  @param custom_type        Custom type to filter messages. If filtering isn't required, set `SBD_NULL_STRING`.
     *  @param completion_handler The handler interface to execute.
     */
    void GetNextMessagesByTimestamp(int64_t timestamp, int64_t next_limit, bool reverse, SBDMessageTypeFilter message_type, wstring custom_type, SBDGetMessagesInterface *completion_handler);
    
    /**
     *  Gets the previous messages by the timestamp with a limit and ordering.
     *
     *  @param timestamp         The standard timestamp to load messages.
     *  @param prev_limit             The limit for the number of messages. The returned messages could be more than this number if there are messages which have the same timestamp.
     *  @param reverse           If yes, the latest message is the index 0.
     *  @param message_type       Message type to filter messages.
     *  @param custom_type        Custom type to filter messages. If filtering isn't required, set `SBD_NULL_STRING`.
     *  @param completion_handler The handler interface to execute.
     */
    void GetPreviousMessagesByTimestamp(int64_t timestamp, int64_t prev_limit, bool reverse, SBDMessageTypeFilter message_type, wstring custom_type, SBDGetMessagesInterface *completion_handler);
    
    /**
     *  Gets the previous and next message by the timestamp with a limit and ordering.
     *
     *  @param timestamp         The standard timestamp to load messages.
     *  @param prev_limit         The previous limit for the number of messages. The returned messages could be more than this number if there are messages which have the same timestamp.
     *  @param next_limit         The next limit for the number of messages. The returned messages could be more than this number if there are messages which have the same timestamp.
     *  @param reverse           If yes, the latest message is the index 0.
     *  @param message_type       Message type to filter messages.
     *  @param custom_type        Custom type to filter messages. If filtering isn't required, set `SBD_NULL_STRING`.
     *  @param completion_handler The handler interface to execute.
     */
    void GetMessagesByTimestamp(int64_t timestamp, int64_t prev_limit, int64_t next_limit, bool reverse, SBDMessageTypeFilter message_type, wstring custom_type, SBDGetMessagesInterface *completion_handler);
    
    /**
     *  Gets the next messages by the message ID with a limit and ordering.
     *
     *  @param message_id         The standard message ID to load messages.
     *  @param next_limit             The limit for the number of messages. The returned messages could be more than this number if there are messages which have the same timestamp.
     *  @param reverse           If yes, the latest message is the index 0.
     *  @param message_type       Message type to filter messages.
     *  @param custom_type        Custom type to filter messages. If filtering isn't required, set `SBD_NULL_STRING`.
     *  @param completion_handler The handler interface to execute.
     */
    void GetNextMessagesByMessageId(int64_t message_id, int64_t next_limit, bool reverse, SBDMessageTypeFilter message_type, wstring custom_type, SBDGetMessagesInterface *completion_handler);
    
    /**
     *  Gets the previous messages by the message ID with a limit and ordering.
     *
     *  @param message_id         The standard message ID to load messages.
     *  @param prev_limit             The limit for the number of messages. The returned messages could be more than this number if there are messages which have the same timestamp.
     *  @param reverse           If yes, the latest message is the index 0.
     *  @param message_type       Message type to filter messages.
     *  @param custom_type        Custom type to filter messages. If filtering isn't required, set `SBD_NULL_STRING`.
     *  @param completion_handler The handler interface to execute.
     */
    void GetPreviousMessagesByMessageId(int64_t message_id, int64_t prev_limit, bool reverse, SBDMessageTypeFilter message_type, wstring custom_type, SBDGetMessagesInterface *completion_handler);
    
    /**
     *  Gets the previous and next message by the message ID with a limit and ordering.
     *
     *  @param message_id         The standard message ID to load messages.
     *  @param prev_limit         The previous limit for the number of messages. The returned messages could be more than this number if there are messages which have the same timestamp.
     *  @param next_limit         The next limit for the number of messages. The returned messages could be more than this number if there are messages which have the same timestamp.
     *  @param reverse           If yes, the latest message is the index 0.
     *  @param message_type       Message type to filter messages.
     *  @param custom_type        Custom type to filter messages. If filtering isn't required, set `SBD_NULL_STRING`.
     *  @param completion_handler The handler interface to execute.
     */
    void GetMessagesByMessageId(int64_t message_id, int64_t prev_limit, int64_t next_limit, bool reverse, SBDMessageTypeFilter message_type, wstring custom_type, SBDGetMessagesInterface *completion_handler);
    
    /**
     *  Copies a user message to the target channel.
     *
     *  @param message User message object.
     *  @param target_channel Target channel object.
     *  @param completion_handler The handler interface to execute. The `userMessage` is a user message which is returned from the SendBird server. The message has a message ID.
     *  @return Returns the temporary user message with a request ID. It doesn't have a message ID.
     */
    SBDUserMessage *CopyUserMessage(SBDUserMessage *message, SBDBaseChannel *target_channel, SBDCopyUserMessageInterface *completion_handler);
    
    /**
     *  Copies a file message to the target channel.
     *  
     *  @param message File message object.
     *  @param target_channel Target channel object.
     *  @param completion_handler The handler interface to execute. The `fileMessage` is a user message which is returned from the SendBird server. The message has a message ID.
     *  @return Returns the temporary file message with a request ID. It doesn't have a message ID.
     */
    SBDFileMessage *CopyFileMessage(SBDFileMessage *message, SBDBaseChannel *target_channel, SBDCopyFileMessageInterface *completion_handler);

	/**
	 *  Internal use only
	 */
	SBDBaseChannel(string dict);

	/**
	 *  Internal use only
	 */
	SBDBaseChannel();
};

#endif /* SENDBIRD_SBDBASECHANNEL_H_ */
