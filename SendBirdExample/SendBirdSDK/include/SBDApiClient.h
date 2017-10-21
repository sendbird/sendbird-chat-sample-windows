//
//  SBDApiClient.h
//  SendBird
//
//  Created by Jed Gyeong on 6/15/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDAPICLIENT_H_
#define SENDBIRD_SBDAPICLIENT_H_

#include <iostream>

#ifdef __APPLE__
#include <curl/curl.h>
#elif defined(_WIN32) || (_WIN64)
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif
#include <Windows.h>
#include <WinInet.h>
#pragma comment( lib, "wininet" )
#endif

#include "SBDTypes.h"
#include "SBDInternalTypes.h"
#include "SBDBaseChannel.h"
#include "SBDOpenChannel.h"
#include "SBDGroupChannel.h"
#include "SBDError.h"
#include "SBDInterface.h"

using namespace std;

class SBDRequestInterface;

class SBDApiClientThread : public SBDBaseInterface {
private:
	HANDLE thread;
	static DWORD WINAPI Handler(void *vargp);
    
public:
	SBDApiClientThread();
	virtual ~SBDApiClientThread() = 0;

    void Start();
	virtual void RunCallback() {};
};

class SBDGetThread : public SBDApiClientThread {
public:
    wstring uri;
    wstring params;
    SBDRequestInterface *completion_handler;
    
    SBDGetThread(wstring uri, wstring params, SBDRequestInterface *completion_handler);

	void RunCallback();
};

class SBDGetForRoutingThread : public SBDApiClientThread {
public:
	SBDRequestInterface *completion_handler;

	SBDGetForRoutingThread(SBDRequestInterface *completion_handler);

	void RunCallback();
};

class SBDDeleteThread : public SBDApiClientThread {
public:
    wstring uri;
    wstring params;
    SBDRequestInterface *completion_handler;
    
    SBDDeleteThread(wstring uri, wstring params, SBDRequestInterface *completion_handler);

	void RunCallback();
};

class SBDPostThread : public SBDApiClientThread {
public:
    wstring uri;
    string body;
    SBDRequestInterface *completion_handler;

    SBDPostThread(wstring uri, string body, SBDRequestInterface *completion_handler);

	void RunCallback();
};

class SBDPutThread : public SBDApiClientThread {
public:
    wstring uri;
    string body;
    SBDRequestInterface *completion_handler;
    
    SBDPutThread(wstring uri, string body, SBDRequestInterface *completion_handler);

	void RunCallback();
};

struct SBDResponse {
    char *body;
    size_t len;
};

class SBDRequestInterface : public SBDBaseInterface {
public:
	virtual ~SBDRequestInterface() = 0;
    virtual void RequestCompletionHandler(string res, SBDError *error) {};
};

class SBDApiClient {
private:
    wstring application_id;
    wstring sdk_version;
    
    string session_key;
    string ekey;
    
public:
#ifdef __APPLE__
    CURL *session;
#elif defined(_WIN32) || (_WIN64)
	HINTERNET hResource;
#endif
    wstring ws_host;
    wstring api_host;
    time_t routing_updated_at;
    
private:
    SBDApiClient() {}
    
    string DefaultBody();
    string DefaultParam();
    string BuildBodyForUpdatingUserInfo(wstring nickname, wstring profile_url);
    string BuildBodyForCreatingUpdatingOpenChannel(wstring name, wstring channel_url, wstring cover_url, wstring data, vector<wstring> operator_user_ids, wstring custom_type);
    wstring BuildParamForLoadingUserList(string token, int64_t limit, vector<wstring> user_ids, wstring meta_data_key, vector<wstring> meta_data_values);
    
    string BuildBodyForCreatingMetaCounters(map<wstring, int64_t> meta_counters);
	wstring BuildParameterForGettingOpenChannelMetaCounters(vector<wstring> keys);
	string BuildBodyForUpdatingMetaCounters(map<wstring, int64_t> meta_counters, SBDChannelMetaCountersUpdateMode update_mode, bool upsert);
    
	string BuildBodyForCreatingAndUpdatingMetaData(map<wstring, wstring> meta_data);
	wstring BuildParameterForGettingChannelMetaData(vector<wstring> keys);
	string BuildBodyForUpdatingMetaData(map<wstring, wstring> meta_data, bool upsert);
    
	wstring BuildParameterForLoadingMessagesByTimestamp(int64_t timestamp, int64_t prev_limit, int64_t next_limit, bool include, bool reverse, SBDBaseChannel *channel, SBDMessageTypeFilter message_type, wstring custom_type);
	wstring BuildParameterForLoadingMessagesByMessageId(int64_t message_id, int64_t prev_limit, int64_t next_limit, bool include, bool reverse, SBDBaseChannel *channel, SBDMessageTypeFilter message_type, wstring custom_type);
    
	string BuildBodyForBlockUser(wstring target_id);
	string BuildBodyForSettingAutoAcceptInvitation(bool auto_accept);
    
	wstring BuildParameterForGettingGroupChannel(bool include_member, bool include_read_receipt);
    
	string BuildBodyForCreatingUpdatingGroupChannel(wstring name, bool is_distinct, wstring cover_url, wstring data, vector<wstring> user_ids, wstring custom_type);
	string BuildBodyForInvitationUsers(vector<wstring> user_ids);
	string BuildBodyForHidingGroupChannel(wstring user_id, bool hide_prev_messages);
	string BuildBodyForLeavingGroupChannel(wstring user_id);

	string BuildBodyForBanUser(wstring user_id, wstring description, int seconds);
	string BuildBodyForMuteUser(wstring user_id);
    
	string BuildBodyForCreatingUpdatingUserMetaData(map<wstring, wstring> meta_data);
    
	wstring BuildParameterForLoadingOpenChannelList(string token, int64_t limit, wstring channel_name_filter, wstring channel_url_filter, wstring custom_type_filter);
	wstring BuildParameterForLoadingGroupChannelList(string token, wstring user_id, int64_t limit, bool include_member_list, SBDGroupChannelListOrder order, bool include_empty_channel, vector<wstring> channel_urls_filter, vector<SBDUser> users_filter_exact_match, wstring nickname_contains_filter, vector<SBDUser> users_filter_like_match, SBDGroupChannelListQueryType query_type, wstring custom_type_filter, SBDMemberStateFilter member_state_filter, wstring channel_name_filter);
	wstring BuildParamForLoadingBlockedUserList(string token, int64_t limit);
	wstring BuildParamForLoadingLoadOpenChannelParticipantList(string token, int64_t limit);
	wstring BuildParamForLoadingLoadOpenChannelMutedUserList(string token, int64_t limit);
	wstring BuildParamForLoadingLoadOpenChannelBannedUserList(string token, int64_t limit);
	wstring BuildParameterForGettingGroupChannelCount(SBDMemberStateFilter member_state_filter);
    
    void Get(wstring uri, wstring params, SBDRequestInterface *completion_handler);
	void GetForRouting(SBDRequestInterface *completion_handler);
    void Delete(wstring uri, wstring params, SBDRequestInterface *completion_handler);
    void Put(wstring uri, string body, SBDRequestInterface *completion_handler);
    void Post(wstring uri, string body, SBDRequestInterface *completion_handler);
    
public:
    static SBDApiClient& GetInstance() {
        static bool initialized = false;
        static SBDApiClient instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        
        if (!initialized) {
            instance.ClearSessionKey();
        }
        
        initialized = true;
        
        return instance;
    }
    
    SBDApiClient(SBDApiClient const&) = delete;
    void operator=(SBDApiClient const&) = delete;
    
    static void Init(wstring application_id, wstring version);
    
    string GetSessionKey();
    void SetSessionKey(string session_key);
    void ClearSessionKey();
    string GetEKey();
    void SetEKey(string ekey);
    void ClearEKey();
    void GetHostUrl(SBDRequestInterface *completion_handler);
    string GetMimeType();
    void UpdateUserInfo(wstring user_id, wstring nickname, wstring profile_url, SBDRequestInterface *completion_handler);
    
    
    void CreateOpenChannel(wstring name, wstring channel_url, wstring cover_url, wstring data, vector<wstring> operator_user_ids, wstring custom_type, SBDRequestInterface *completion_handler);
    void UpdateOpenChannel(wstring channel_url, wstring name, wstring cover_url, wstring data, vector<wstring> operator_user_ids, wstring custom_type, SBDRequestInterface *completion_handler);
    void GetOpenChannel(wstring channel_url, SBDRequestInterface *completion_handler);
    
    void CreateGroupChannel(vector<wstring> user_ids, wstring name, bool is_distinct, wstring cover_url, wstring data, wstring custom_type, SBDRequestInterface *completion_handler);
    void UpdateGroupChannel(wstring channel_url, wstring name, bool is_distinct, wstring cover_url, wstring data, wstring custom_type, SBDRequestInterface *completion_handler);
    void GetGroupChannel(wstring channel_url, bool include_member, bool include_read_receipt, SBDRequestInterface *completion_handler);
    void InviteUsers(wstring channel_url, vector<wstring> user_ids, SBDRequestInterface *completion_handler);
    void HideGroupChannel(wstring channel_url, bool hide_prev_messages, SBDRequestInterface *completion_handler);
    void LeaveGroupChannel(wstring channel_url, SBDRequestInterface *completion_handler);
    void MarkAllGroupChannelAsRead(SBDRequestInterface *completion_handler);

    void LoadUserList(string token, int64_t limit, vector<wstring> user_ids, wstring meta_data_key, vector<wstring> meta_data_values, SBDRequestInterface *completion_handler);
    void LoadBlockedUserList(wstring blocker_user_id, string token, int64_t limit, SBDRequestInterface *completion_handler);
    void LoadOpenChannelParticipantList(wstring channel_url, string token, int64_t limit, SBDRequestInterface *completion_handler);
    void LoadOpenChannelMutedUserList(wstring channel_url, string token, int64_t limit, SBDRequestInterface *completion_handler);
    void LoadOpenChannelBannedUserList(wstring channel_url, string token, int64_t limit, SBDRequestInterface *completion_handler);
    
    void DeleteMessage(SBDBaseChannel *channel, int64_t message_id, SBDRequestInterface *completion_handler);
    
    void CreateMetaCounters(SBDBaseChannel *channel, map<wstring, int64_t> meta_counters, SBDRequestInterface *completion_handler);
    void GetMetaCounters(SBDBaseChannel *channel, vector<wstring> keys, SBDRequestInterface *completion_handler);
    void GetAllMetaCounters(SBDBaseChannel *channel, SBDRequestInterface *completion_handler);
    void UpdateMetaCounters(SBDBaseChannel *channel, map<wstring, int64_t> meta_counters, SBDChannelMetaCountersUpdateMode update_mode, bool upsert, SBDRequestInterface *completion_handler);
    void DeleteMetaCounter(SBDBaseChannel *channel, wstring key, SBDRequestInterface *complete_handler);
    void DeleteAllMetaCounters(SBDBaseChannel *channel, SBDRequestInterface *complete_handler);
    
    void CreateMetaData(SBDBaseChannel *channel, map<wstring, wstring> meta_data, SBDRequestInterface *completion_handler);
    void GetMetaData(SBDBaseChannel *channel, vector<wstring> keys, SBDRequestInterface *completion_handler);
    void GetAllMetaData(SBDBaseChannel *channel, SBDRequestInterface *completion_handler);
    void UpdateMetaData(SBDBaseChannel *channel, map<wstring, wstring> meta_data, bool upsert, SBDRequestInterface *completion_handler);
    void DeleteMetaData(SBDBaseChannel *channel, wstring key, SBDRequestInterface *completion_handler);
    void DeleteAllMetaData(SBDBaseChannel *channel, SBDRequestInterface *completion_handler);
    
    void LoadMessagesByTimestamp(int64_t timestamp, int64_t prev_limit, int64_t next_limit, bool include, bool reverse, SBDBaseChannel *channel, SBDMessageTypeFilter message_type, wstring custom_type, SBDRequestInterface *completion_handler);
    void LoadMessagesByMessageId(int64_t message_id, int64_t prev_limit, int64_t next_limit, bool include, bool reverse, SBDBaseChannel *channel, SBDMessageTypeFilter message_type, wstring custom_type, SBDRequestInterface *completion_handler);
    
    void BlockUser(wstring blocker_id, wstring blockee_id, SBDRequestInterface *completion_handler);
    void UnblockUser(wstring blocker_id, wstring blockee_id, SBDRequestInterface *completion_handler);
    void SetAutoAcceptInvitation(bool auto_accept, SBDRequestInterface *completion_handler);
    void GetAutoAcceptInvitation(SBDRequestInterface *completion_handler);
    
    void AcceptInvitation(wstring channel_url, SBDRequestInterface *completion_handler);
    void DeclineInvitation(wstring channel_url, SBDRequestInterface *completion_handler);
    
    void BanUser(wstring user_id, wstring channel_url, wstring description, int seconds, SBDRequestInterface *completion_handler);
    void UnbanUser(wstring user_id, wstring channel_url, SBDRequestInterface *completion_handler);
    void MuteUser(wstring user_id, wstring channel_url, SBDRequestInterface *completion_handler);
    void UnmuteUser(wstring user_id, wstring channel_url, SBDRequestInterface *completion_handler);
    
    void CreateUserMetaData(wstring user_id, map<wstring, wstring> meta_data, SBDRequestInterface *completion_handler);
    void UpdateUserMetaData(wstring user_id, map<wstring, wstring> meta_data, SBDRequestInterface *completion_handler);
    void DeleteUserMetaData(wstring user_id, wstring key, SBDRequestInterface *completion_handler);
    void DeleteAllUserMetaData(wstring user_id, SBDRequestInterface *completion_handler);
    
    void LoadOpenChannelList(string token, int64_t limit, wstring channel_name_filter, wstring channel_url_filter, wstring custom_type_filter, SBDRequestInterface *completion_handler);
    void LoadGroupChannelList(string token, wstring user_id, int64_t limit, bool include_member_list, SBDGroupChannelListOrder order, bool include_empty_channel, vector<wstring> channel_urls_filter, vector<SBDUser> users_filter_exact_match, wstring nickname_contains_filter, vector<SBDUser> users_filter_like_match, SBDGroupChannelListQueryType query_type, wstring custom_type_filter, SBDMemberStateFilter member_state_filter, wstring channel_name_filter, SBDRequestInterface *completion_handler);
    void ResetGroupChannelHistory(wstring channel_url, SBDRequestInterface *completion_handler);
    void GetGroupChannelCount(SBDMemberStateFilter member_state_filter, SBDRequestInterface *completion_handler);
};

#endif /* SENDBIRD_SBDAPICLIENT_H_ */
