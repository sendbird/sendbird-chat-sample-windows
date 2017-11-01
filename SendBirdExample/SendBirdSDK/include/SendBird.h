//
//  SendBird.h
//  SendBird
//
//  Created by SendBird on 15/06/2017.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SENDBIRD_H_
#define SENDBIRD_SENDBIRD_H_

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <map>
#include <cstdio>
#include <memory>

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif

#include "SBDUser.h"
#include "SBDBaseChannel.h"
#include "SBDGroupChannel.h"
#include "SBDOpenChannelListQuery.h"
#include "SBDGroupChannelListQuery.h"
#include "SBDTypes.h"
#include "SBDUserListQuery.h"
#include "SBDInternalTypes.h"
#include "SBDWebSocketClient.h"
#include "SBDReachability.h"
#include "SBDApiClient.h"
#include "SBDError.h"
#include "SBDInterface.h"
#include "SBDAdminMessage.h"
#include "SBDUserMessage.h"
#include "SBDFileMessage.h"
#include "SBDTimer.h"
#include "SBDPreviousMessageListQuery.h"
#include "SBDScheduledMM.h"

#define SBD_NULL_STRING "SBD_NULL_STRING_f2648f84e7e941208c8d9df02230a7c8"
#define SBD_NULL_WSTRING L"SBD_NULL_STRING_f2648f84e7e941208c8d9df02230a7c8"
#define IsNullString(str) (!((str).compare(SBD_NULL_STRING)))
#define IsNullWString(str) (!((str).compare(SBD_NULL_WSTRING)))

using namespace std;

class SBDWebSocketClient;
class SBDLoginTimerHandler;

/**
 *  Represents operation options.
 */
class SBDOption {
public:
	/**
	 *  If set <code>true</code>, the sender information of `sender` of `SBDUserMessage` or `SBDFileMessage` such as nickname and profile url will be returned as the latest user's. Otherwise, the information will be the value of the message creation time.
	 */
    static bool user_member_as_message_sender;
};

class SBDReconnectionInterface {
	HANDLE started_handle;
	HANDLE succeeded_handle;
	HANDLE failed_handle;
	HANDLE cancelled_handle;

	static DWORD WINAPI CallStartedHandler(void *vargp);
	static DWORD WINAPI CallSucceededHandler(void *vargp);
	static DWORD WINAPI CallFailedHandler(void *vargp);
	static DWORD WINAPI CallCancelledHandler(void *vargp);

public:
	void CallStarted();
	void CallSucceeded();
	void CallFailed();
	void CallCancelled();

    virtual void Started() {};
    virtual void Succeeded() {};
    virtual void Failed() {};
    virtual void Cancelled() {};
};

class SBDReconnectTimerHandler : public SBDTimer {
public:
	SBDReconnectTimerHandler(DWORD time_interval, bool repeat);
    
    void RunCallback();
};

class SBDUpdateUserInfoInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDTypingStatusTimerHandler : public SBDTimer {
public:
	SBDTypingStatusTimerHandler(DWORD time_interval, bool repeat);
    
    void RunCallback();
};

class SBDBlockUserInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDUser blocked_user, SBDError *error) {};
};

class SBDUnblockUserInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDSetGroupChannelInvitationPreferenceInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(SBDError *error) {};
};

class SBDGetGroupChannelInvitationPreferenceInterface : public SBDBaseInterface {
public:
    virtual void CompletionHandler(bool is_auto_accept, SBDError *error) {};
};

/**
 *  Internal use only.
 */
class SBDAckTimerHandler : public SBDTimer {
public:
	string request_id;

	SBDAckTimerHandler(string request_id, bool repeat, DWORD time_interval) : SBDTimer(time_interval, repeat) {
		this->request_id = request_id;
	}

	void RunCallback() {

	}
};

/**
 *  Internal use only.
 */
class SBDAckTimerInfo {
public:
	SBDAckTimerHandler *ack_timer;
	SBDSendCommandInterface *completion_handler;
	SBDCommand *command;
};

/**
 *  The `SBDMain` is the core class for SendBird. This class is singletone instance which is initialized by Application ID.
 *  This class provides the methods for overall. The methods include `SBDReconnectionInterface` registration for receiving events are related to channels, `SBDReconnectionInterface` for managing the connection status, updating the current user's information, and blocking other users.
 */
class SBDMain {
private:
    wstring application_id;
    SBDUser current_user;
    
    // Handlers.
	pthread_mutex_t channel_handlers_mutex_lock;
    map<wstring, SBDChannelInterface *> channel_handlers;
    map<wstring, SBDReconnectionInterface *> reconnection_handlers;
    
    // WebSocket
	pthread_mutex_t websocket_client_mutex_lock;
    SBDWebSocketClient *websocket_client;

    bool reconnect_in_progress;
	pthread_mutex_t reconnect_timer_mutex_lock;
    SBDReconnectTimerHandler *reconnect_timer;

	pthread_mutex_t typing_status_timer_mutex_lock;
    SBDTypingStatusTimerHandler *typing_status_timer;

    bool host_reachability_on_off;
    SBDReachability *host_reachability;
    SBDNetworkStatus net_status;
    
    bool session_opened;
    bool debug_mode;
    
    bool has_current_user;

	SBDScheduledMM *scheduled_mm;
    
public:
	/**
	 *  Internal use only.
	 */
	pthread_mutex_t login_timer_mutex_lock;

	/**
	 *  Internal use only.
	 */
	SBDLoginTimerHandler *login_timer;

	/**
	 *  Shows the current log level.
	 */
    static SBDLogLevel log_level;
    
	/**
	 *  Internal use only.
	 */
	DWORD reconnect_delay;
    
	/**
	 *  Internal use only.
	 */
    uint8_t reconnect_count;

	/**
	 *  Internal use only.
	 */
	map<string, SBDAckTimerInfo *> ack_timer_map;

	/**
	 *  Internal use only.
	 */
	pthread_mutex_t ack_timer_map_mutex;

private:
    SBDMain();
    
public:
	/**
	 *  Gets the Application ID which was used for initialization.
	 *
	 *  @return The Application ID.
	 */
    static wstring GetApplicationId();
    
	/**
	 *  Retrieves the SDK version.
	 *
	 *  @return The SDK version.
	 */
    static wstring GetSDKVersion();
    
	/**
	 *  Gets initializing state.
	 *
	 *  @return If true, `SBDMain` instance is initialized.
	 */
    static bool IsInitialized();
    
	 /**
	 *  Initializes `SBDMain` singleton instance with SendBird Application ID. The Application ID is on SendBird dashboard. This method has to be run first in order to user SendBird.
	 *
	 *  @param applicationId The Applicatin ID of SendBird. It can be founded on SendBird Dashboard.
	 */
    static void Init(wstring application_id);
    
	/**
	 *  Gets the WebSocket server connection state.
	 *
	 *  @return `SBDWebSocketConnectionState`
	 *
	 *  - `SBDWebSocketConnecting` - Connecting to the chat server
	 *  - `SBDWebSocketOpen` - Connected to the chat server
	 *  - `SBDWebSocketClosing` - Disconnecting from the chat server
	 *  - `SBSWebSocketClosed` - Disconnected from the chat server
	 */
    static SBDWebSocketConnectionState GetConnectState();
    
	/**
	 *  Adds the `SBDReconnectionInterface`.
	 *
	 *  @param handler   `SBDReconnectionInterface` handler.
	 *  @param identifier The identifier for the handler.
	 */
    static void AddReconnectionHandler(SBDReconnectionInterface *handler, wstring identifier);
    
	/**
	 *  Removes the `SBDReconnectionInterface` by identifier.
	 *
	 *  @param identifier The identifier for the handler to be removed.
	 */
    static void RemoveReconnectionHandler(wstring identifier);
    
	/**
	 *  Removes all connection handlers;
	 */
    static void RemoveAllReconnectionHandlers();
    
	/**
	 *  Gets the handler for reconnection by indentifer.
	 *
	 *  @param identifier The identifier for handler.
	 *
	 *  @return `SBDReconnectionInterface` handler.
	 */
    static SBDReconnectionInterface *GetReconnectionHandler(wstring identifier);
    
	/**
	 *  Adds the `SBDChannelInterface`.
	 *
	 *  @param handler   `SBDChannelInterface` handler.
	 *  @param identifier The identifier for handler.
	 */
    static void AddChannelHandler(SBDChannelInterface *handler, wstring identifier);
    
	/**
	 *  Removes the `SBDChannelInterface` by identifier.
	 *
	 *  @param identifier The identifier for the handler to be removed.
	 */
    static void RemoveChannelHandler(wstring identifier);
    
	/**
	 *  Removes all channel handlers;
	 */
    static void RemoveAllChannelHandlers();
    
	/**
	 *  Gets the handler for channel by indentifer.
	 *
	 *  @param identifier The identifier for handler.
	 *
	 *  @return `SBDChannelInterface` handler.
	 */
    static SBDChannelInterface *GetChannelHandler(wstring identifier);

	/**
	 *  Performs a connection to SendBird with the user ID and the access token.
	 *
	 *  @param user_id            The user ID.
	 *  @param access_token       The access token. If the user doesn't have access token, set nil.
	 *  @param completion_handler The handler interface to execute. `user` is the object to represent the current user.
	 */
    static void Connect(wstring user_id, wstring access_token, SBDConnectInterface *completion_handler);

	/**
	 *  Disconnects from SendBird. If this method is invoked, the current user will be invalidated.
	 *
	 *  @param completion_handler The handler interface to execute.
	 */
	static void Disconnect(SBDDisconnectInterface *completion_handler);

	/**
	 *  Updates the current user's information.
	 *
	 *  @param nickname          New nickname.
	 *  @param profile_url        New profile image url.
	 *  @param completion_handler The handler interface to execute.
	 */
	static void UpdateCurrentUserInfo(wstring nickname, wstring profile_url, SBDUpdateUserInfoInterface *completion_handler);

	/**
	 *  Updates the current user's information.
	 *
	 *  @param nickname          New nickname.
	 *  @param profile_image_file_path        New profile image file path.
	 *  @param mime_type The image file's mime type.
	 *  @param completion_handler The handler interface to execute.
	 */
	static void UpdateCurrentUserInfoWithBinaryProfileImage(wstring nickname, wstring profile_image_file_path, wstring mime_type, SBDUpdateUserInfoInterface *completion_handler);

	/**
	 *  Creates `SBDUserListQuery` instance for getting a list of all users of this application.
	 *
	 *  @return `SBDUserListQuery` instance.
	 */
	static SBDUserListQuery *CreateAllUserListQuery();

	/**
	 *  Creates `SBDUserListQuery` instance for gettting a list of users of this application with user IDs.
	 *
	 *  @param user_ids The user IDs to get user objects.
	 *
	 *  @return `SBDUserListQuery` instance.
	 */
	static SBDUserListQuery *CreateUserListQuery(vector<wstring> user_ids);

	/**
	 *  Creates `SBDUserListQuery` instance for getting a list of blocked users by the current user.
	 *
	 *  @return `SBDUserListQuery` instance.
	 */
	static SBDUserListQuery *CreateBlockedUserListQuery();

	/**
	 *  Blocks the specified user.
	 *
	 *  @param user              The user to be blocked.
	 *  @param completion_handler The handler interface to execute. `blockedUser` is the blocked user by the current user.
	 */
	static void BlockUser(SBDUser user, SBDBlockUserInterface *completion_handler);

	/**
	 *  Unblocks the specified user.
	 *
	 *  @param userId            The user ID which was blocked.
	 *  @param completion_handler The handler interface to execute.
	 */
	static void UnblockUser(SBDUser user, SBDUnblockUserInterface *completion_handler);

	/**
	 *  Starts reconnection explictly. The `SBDReconnectionInterface` handlers will be invoked by the reconnection process.
	 *  @return Returns true if there is the data to be used for reconnection.
	 */
	static bool Reconnect();

	/**
	 *  Sets group channel invitation preference for auto acceptance.
	 *  @param auto_accept If true, the current user will accept the group channel invitation automatically.
	 *  @param completion_handler The handler interface to execute.
	 */
	static void SetChannelInvitationPreference(bool auto_accept, SBDSetGroupChannelInvitationPreferenceInterface *completion_handler);

	/**
	 *  Gets group channel inviation preference for auto acceptance.
	 *  @param completion_handler The handler interface to execute.
	 */
	static void GetChannelInvitationPreference(SBDGetGroupChannelInvitationPreferenceInterface *completion_handler);

	/**
	 *  Internal use only.
	 */
    static void Disconnect(bool logout, SBDDisconnectInterface *completion_handler);
    
	/**
	 * Internal use only.
	 */
    static void ClearCache();
    
	/**
	 *  Internal use only.
	 */
    static void ClearEnteredChannels();
    
	/**
	 *  Internal use only.
	 */
    static void RemoveChannelInCache(wstring channel_url);
    
	/**
	 *  Internal use only.
	 */
    void ReceiveMessage(string message, bool reconnect);
    
	/**
	 *  Internal use only.
	 */
    void Send(SBDCommand *command, SBDSendCommandInterface *completion_handler);
    
	/**
	 *  Internal use only.
	 */
    static void ReconnectTaskComplete();
    
	/**
	 *  Internal use only.
	 */
    static void ClearCurrentUser();
    
	/**
	 *  Internal use only.
	 */
	static void Reconnect(DWORD timeout);
    
	/**
	 *  Internal use only.
	 */
    static void ConnectToWebSocketServer(bool reconnect, wstring user_id, wstring access_token, string session_key, SBDConnectToWebSocketServerInterface *completion_handler);
    
	/**
	 * Internal use only.
	 */
	static void SetHosts(wstring api_host, wstring ws_host);

	/**
	 * Internal use only.
	 */
    static wstring GetApiServer();

	/**
	 * Internal use only.
	 */
	static wstring GetCustomApiServer();

	/**
	 *  Internal use only.
	 */
	static wstring GetApiHostScheme();

	/**
	 *  Internal use only.
	 */
	static wstring GetApiHost();

	/**
	 *  Internal use only.
	 */
	static int GetApiPort();
    
	/**
	 *  Internal use only.
	 */
    static wstring GetWsServer();

	/**
	* Internal use only.
	*/
	static wstring GetCustomWsServer();

	/**
	 *  Internal use only.
	 */
	static wstring GetWsHostScheme();

	/**
	 *  Internal use only.
	 */
	static wstring GetWsHost();

	/**
	 *  Internal use only.
	 */
	static int GetWsPort();
    
	/**
	 *  Internal use only.
	 */
    static void UpdateInterface(SBDReachability *reachability);

	/**
	 *  Internal use only.
	 */
    static void SetWebSocket();
    
	/**
	 *  Internal use only.
	 */
    static SBDWebSocketClient *GetWebSocket();
    
	/**
	 *  Internal use only.
	 */
    static void DeleteWebSocket();
    
	/**
	 *  Internal use only.
	 */
    static map<wstring, SBDChannelInterface *> *GetChannelHandlers();
    
	/**
	 *  Internal use only.
	 */
    static bool HasCurrentUser();
    
	/**
	 *  Internal use only.
	 */
    static SBDUser *GetCurrentUser();
    
	/**
	 *  Internal use only.
	 */
    static void SetCurrentUser(SBDUser user);
    
	/**
	 *  Internal use only.
	 */
    static void SetLoginTimer(wstring user_id, SBDConnectToWebSocketServerInterface *completion_handler);
    
	/**
	 *  Internal use only.
	 */
    static void StartLoginTimer();
    
	/**
	 *  Internal use only.
	 */
    static void InvalidateLoginTimer();
    
	/**
	 *  Internal use only.
	 */
    static void IncreaseReconnectCount();
    
	/**
	 *  Internal use only.
	 */
    static void ResetReconnectCount();
    
	/**
	 *  Internal use only.
	 */
    static uint8_t GetReconnectCount();
    
	/**
	 *  Internal use only.
	 */
	static DWORD GetReconnectDelay();
    
	/**
	 *  Internal use only.
	 */
    static void IncreaseReconnectDelay();
    
	/**
	 *  Internal use only.
	 */
    static void ResetReconnectDelay();
    
	/**
	 *  Internal use only.
	 */
    static void SetReconnectionProgress(bool is_in_progress);
    
	/**
	 *  Internal use only.
	 */
    static bool GetReconnectionProgress();
    
	/**
	 *  Internal use only.
	 */
    static map<wstring, SBDReconnectionInterface *> *GetReconnectionHandlers();

	/**
	 *  Internal use only.
	 */
	static void SetReconnectTimer(DWORD time_interval);
    
	/**
	 *  Internal use only.
	 */
    static void StartReconnectTimer();
    
	/**
	 *  Internal use only.
	 */
    static void InvalidateReconnectTimer();

	/**
	 *  Internal use only.
	 */
	static void StopReconnectTimer();

	/**
	 *  Internal use only.
	 */
	static void InvalidateAckTimers();

	/**
	 *  Internal use only.
	 */
	static SBDMain& GetInstance();

	/**
	 *  Internal use only.
	 */
	SBDMain(SBDMain const&) = delete;

	/**
	 *  Internal use only.
	 */
	void operator=(SBDMain const&) = delete;

	/**
	 *  Internal use only.
	 */
	//static void Log(SBDLogLevel log_level, const string fmt_str, ...);

	/**
	 *  Internal use only.
	 */
	static void Connect(wstring user_id, wstring access_token, wstring api_host, wstring ws_host, SBDConnectInterface *completion_handler);

	/**
	*  Internal use only.
	*/
	static map<SBDBaseInterface *, time_t> callbacks;

	/**
	 *  Internal use only.
	 */
	static void AddCallback(SBDBaseInterface *);

	/**
	 *  Internal use only.
	 */
	static void ReleasesCallbacks();

	/**
	 *  Internal use only.
	 */
	static pthread_mutex_t callback_list_lock;
};

#endif /* SENDBIRD_SBDMAIN_H_ */
