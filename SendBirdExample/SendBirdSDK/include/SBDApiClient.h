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
    
    SBDGetThread(const wstring& uri, const wstring& params, SBDRequestInterface *completion_handler);

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
    
    SBDDeleteThread(const wstring& uri, const wstring& params, SBDRequestInterface *completion_handler);

	void RunCallback();
};

class SBDPostThread : public SBDApiClientThread {
public:
    wstring uri;
    string body;
    SBDRequestInterface *completion_handler;

    SBDPostThread(const wstring& uri, const string& body, SBDRequestInterface *completion_handler);

	void RunCallback();
};

class SBDPutThread : public SBDApiClientThread {
public:
    wstring uri;
    string body;
    SBDRequestInterface *completion_handler;
    
    SBDPutThread(const wstring& uri, const string& body, SBDRequestInterface *completion_handler);

	void RunCallback();
};

class SBDCreateOpenChannelMPHandler : public SBDApiClientThread {
public:
	wstring uri;

	wstring channel_url;
	wstring channel_name;
	wstring cover_image_file_path;
	wstring file_mime_type;
	wstring data;
	vector<wstring> operator_user_ids;
	wstring custom_type;

	SBDRequestInterface *completion_handler;

	SBDCreateOpenChannelMPHandler(const wstring& uri, const wstring& channel_url, const wstring& channel_name, const wstring& cover_image_file_path, const wstring& file_mime_type, const wstring& data, const vector<wstring>& operator_user_ids, const wstring& custom_type, SBDRequestInterface *completion_handler);

	void RunCallback();
};

class SBDUpdateOpenChannelMPHandler : public SBDApiClientThread {
public:
	wstring uri;
	wstring channel_name;
	wstring cover_image_file_path;
	wstring file_mime_type;
	wstring data;
	vector<wstring> operator_user_ids;
	wstring custom_type;

	SBDRequestInterface *completion_handler;

	SBDUpdateOpenChannelMPHandler(const wstring& uri, const wstring& channel_name, const wstring& cover_image_file_path, const wstring& file_mime_type, const wstring& data, const vector<wstring>& operator_user_ids, const wstring& custom_type, SBDRequestInterface *completion_handler);

	void RunCallback();
};

class SBDCreateGroupChannelMPHandler : public SBDApiClientThread {
public:
	wstring uri;

	wstring channel_name;
	bool is_distinct;
	wstring cover_image_file_path;
	wstring file_mime_type;
	wstring data;
	vector<wstring> user_ids;
	wstring custom_type;

	SBDRequestInterface *completion_handler;

	SBDCreateGroupChannelMPHandler(const wstring& uri, const vector<wstring>& user_id, const wstring& channel_name, bool is_distinct, const wstring& cover_image_file_path, const wstring& file_mime_type, const wstring& data, const wstring& custom_type, SBDRequestInterface *completion_handler);

	void RunCallback();
};

class SBDUpdateGroupChannelMPHandler : public SBDApiClientThread {
public:
	wstring uri;

	wstring channel_name;
	bool is_distinct;
	wstring cover_image_file_path;
	wstring file_mime_type;
	wstring data;
	wstring custom_type;

	SBDRequestInterface *completion_handler;

	SBDUpdateGroupChannelMPHandler(const wstring& uri, const wstring& channel_name, bool is_distinct, const wstring& cover_image_file_path, const wstring& file_mime_type, const wstring& data, const wstring& custom_type, SBDRequestInterface *completion_handler);

	void RunCallback();
};

class SBDUploadFileMPHandler : public SBDApiClientThread {
public:
	wstring uri;
	wstring file_path;
    const char* file_buf;
    wstring file_name;
    int64_t file_size;
	wstring file_mime_type;
	vector<SBDThumbnailSize> thumbnail_sizes;
	wstring channel_url;

	SBDRequestInterface *completion_handler;

	SBDUploadFileMPHandler(const wstring& uri, const wstring& file_path, const char* file_buf, const wstring& file_name, int64_t file_size,
        const wstring& file_mime_type, const vector<SBDThumbnailSize>& thumbnail_sizes, const wstring& channel_url, 
        SBDRequestInterface *completion_handler);

	void RunCallback();
};

struct SBDResponse {
    char *body;
    size_t len;
};

class SBDRequestInterface : public SBDBaseInterface {
public:
	virtual ~SBDRequestInterface() = 0;
    virtual void RequestCompletionHandler(const string& res, SBDError *error) {};
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
    string BuildBodyForUpdatingUserInfo(const wstring& nickname, const wstring& profile_url);
    string BuildBodyForCreatingUpdatingOpenChannel(const wstring& name, const wstring& channel_url, const wstring& cover_url, const wstring& data, const vector<wstring>& operator_user_ids, const wstring& custom_type);
    wstring BuildParamForLoadingUserList(const string& token, int64_t limit, const vector<wstring>& user_ids, const wstring& meta_data_key, const vector<wstring>& meta_data_values);
    
    string BuildBodyForCreatingMetaCounters(const map<wstring, int64_t>& meta_counters);
	wstring BuildParameterForGettingOpenChannelMetaCounters(const vector<wstring>& keys);
	string BuildBodyForUpdatingMetaCounters(const map<wstring, int64_t>& meta_counters, SBDChannelMetaCountersUpdateMode update_mode, bool upsert);
    
	string BuildBodyForCreatingAndUpdatingMetaData(const map<wstring, wstring>& meta_data);
	wstring BuildParameterForGettingChannelMetaData(const vector<wstring>& keys);
    
	wstring BuildParameterForLoadingMessagesByTimestamp(int64_t timestamp, int64_t prev_limit, int64_t next_limit, bool include, bool reverse, SBDBaseChannel *channel, SBDMessageTypeFilter message_type, const wstring& custom_type);
	wstring BuildParameterForLoadingMessagesByMessageId(int64_t message_id, int64_t prev_limit, int64_t next_limit, bool include, bool reverse, SBDBaseChannel *channel, SBDMessageTypeFilter message_type, const wstring& custom_type);
    
	string BuildBodyForBlockUser(const wstring& target_id);
	string BuildBodyForSettingAutoAcceptInvitation(bool auto_accept);
    
	wstring BuildParameterForGettingGroupChannel(bool include_member, bool include_read_receipt);
    
	string BuildBodyForCreatingUpdatingGroupChannel(const wstring& name, bool is_distinct, const wstring& cover_url, const wstring& data, const vector<wstring>& user_ids, const wstring& custom_type);
	string BuildBodyForInvitationUsers(const vector<wstring>& user_ids);
	string BuildBodyForHidingGroupChannel(const wstring& user_id, bool hide_prev_messages, bool allow_auto_unhide = true);

	string BuildBodyForLeavingGroupChannel(const wstring& user_id);

	string BuildBodyForBanUser(const wstring& user_id, const wstring& description, int seconds);
	string BuildBodyForMuteUser(const wstring& user_id);
    
	string BuildBodyForCreatingUpdatingUserMetaData(const map<wstring, wstring>& meta_data);
    
	wstring BuildParameterForLoadingOpenChannelList(const string& token, int64_t limit, const wstring& channel_name_filter, const wstring& channel_url_filter, const wstring& custom_type_filter);
	
	wstring BuildParameterForLoadingGroupChannelList(const string& token, const wstring& user_id, int64_t limit, bool include_member_list, SBDGroupChannelListOrder order, bool include_empty_channel, const vector<wstring>& channel_urls_filter, const vector<SBDUser>& users_filter_exact_match, const wstring& nickname_contains_filter, const vector<SBDUser>& users_filter_like_match, SBDGroupChannelListQueryType query_type, const wstring& custom_type_filter, SBDMemberStateFilter member_state_filter, const wstring& channel_name_filter, SBDChannelHiddenStateFilter channel_hidden_state_filter);
	wstring BuildParamForLoadingBlockedUserList(const string& token, int64_t limit);
	wstring BuildParamForLoadingLoadOpenChannelParticipantList(const string& token, int64_t limit);
	wstring BuildParamForLoadingLoadOpenChannelMutedUserList(const string& token, int64_t limit);
	wstring BuildParamForLoadingLoadOpenChannelBannedUserList(const string& token, int64_t limit);
	wstring BuildParameterForGettingGroupChannelCount(SBDMemberStateFilter member_state_filter);
    
    void Get(const wstring& uri, const wstring& params, SBDRequestInterface *completion_handler);
	void GetForRouting(SBDRequestInterface *completion_handler);
    void Delete(const wstring& uri, const wstring& params, SBDRequestInterface *completion_handler);
    void Put(const wstring& uri, const string& body, SBDRequestInterface *completion_handler);
    void Post(const wstring& uri, const string& body, SBDRequestInterface *completion_handler);
    
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
    
    static void Init(const wstring& application_id, const wstring& version);
    
    string GetSessionKey();
    void SetSessionKey(string session_key);
    void ClearSessionKey();
    string GetEKey();
    void SetEKey(string ekey);
    void ClearEKey();
    void GetHostUrl(SBDRequestInterface *completion_handler);
    string GetMimeType();
    void UpdateUserInfo(const wstring& user_id, const wstring& nickname, const wstring& profile_url, SBDRequestInterface *completion_handler);
	void UpdateCurrentUserInfoWithBinaryProfileImage(const wstring& user_id, const wstring& nickname, const wstring& profile_image_file_path, const wstring& type, SBDRequestInterface *completion_handler);
    
    void CreateOpenChannel(const wstring& name, const wstring& channel_url, const wstring& cover_url, const wstring& data, const vector<wstring>& operator_user_ids, const wstring& custom_type, SBDRequestInterface *completion_handler);
	void CreateOpenChannel(const wstring& name, const wstring& channel_url, const wstring& cover_image_file_path, const wstring& cover_image_file_name, const wstring& data, const vector<wstring>& operator_user_ids, const wstring& custom_type, SBDRequestInterface *completion_handler);
    void UpdateOpenChannel(const wstring& channel_url, const wstring& name, const wstring& cover_url, const wstring& data, const vector<wstring>& operator_user_ids, const wstring& custom_type, SBDRequestInterface *completion_handler);
	void UpdateOpenChannel(const wstring& channel_url, const wstring& name, const wstring& cover_image_file_path, const wstring& cover_image_file_mime_type, const wstring& data, const vector<wstring>& operator_user_ids, const wstring& custom_type, SBDRequestInterface *completion_handler);
    void GetOpenChannel(const wstring& channel_url, SBDRequestInterface *completion_handler);
    
    void CreateGroupChannel(const vector<wstring>& user_ids, const wstring& name, bool is_distinct, const wstring& cover_url, const wstring& data, const wstring& custom_type, SBDRequestInterface *completion_handler);
	void CreateGroupChannel(const vector<wstring>& user_ids, const wstring& name, bool is_distinct, const wstring& cover_image_file_path, const wstring& file_mime_type, const wstring& data, const wstring& custom_type, SBDRequestInterface *completion_handler);
    void UpdateGroupChannel(const wstring& channel_url, const wstring& name, bool is_distinct, const wstring& cover_url, const wstring& data, const wstring& custom_type, SBDRequestInterface *completion_handler);
	void UpdateGroupChannel(const wstring& channel_url, const wstring& name, bool is_distinct, const wstring& cover_image_file_path, const wstring& file_mime_type, const wstring& data, const wstring& custom_type, SBDRequestInterface *completion_handler);
    void GetGroupChannel(const wstring& channel_url, bool include_member, bool include_read_receipt, SBDRequestInterface *completion_handler);
    void InviteUsers(const wstring& channel_url, const vector<wstring>& user_ids, SBDRequestInterface *completion_handler);
    void HideGroupChannel(const wstring& channel_url, bool hide_prev_messages, SBDRequestInterface *completion_handler);
	void HideGroupChannelAllowAutoUnhide(const wstring& channel_url, bool hide_prev_messages, bool allow_auto_unhide, SBDRequestInterface* completion_handler);
	void UnhideGroupChannel(const wstring& channel_url, SBDRequestInterface* completion_handler);

	void LeaveGroupChannel(const wstring& channel_url, SBDRequestInterface *completion_handler);
    void MarkAllGroupChannelAsRead(SBDRequestInterface *completion_handler);

    void LoadUserList(const string& token, int64_t limit, const vector<wstring>& user_ids, const wstring& meta_data_key, const vector<wstring>& meta_data_values, SBDRequestInterface *completion_handler);
    void LoadBlockedUserList(const wstring& blocker_user_id, const string& token, int64_t limit, SBDRequestInterface *completion_handler);
    void LoadOpenChannelParticipantList(const wstring& channel_url, const string& token, int64_t limit, SBDRequestInterface *completion_handler);
    void LoadOpenChannelMutedUserList(const wstring& channel_url, const string& token, int64_t limit, SBDRequestInterface *completion_handler);
    void LoadOpenChannelBannedUserList(const wstring& channel_url, const string& token, int64_t limit, SBDRequestInterface *completion_handler);
    
    void DeleteMessage(SBDBaseChannel *channel, int64_t message_id, SBDRequestInterface *completion_handler);
    
    void CreateMetaCounters(SBDBaseChannel *channel, const map<wstring, int64_t>& meta_counters, SBDRequestInterface *completion_handler);
    void GetMetaCounters(SBDBaseChannel *channel, const vector<wstring>& keys, SBDRequestInterface *completion_handler);
    void GetAllMetaCounters(SBDBaseChannel *channel, SBDRequestInterface *completion_handler);
    void UpdateMetaCounters(SBDBaseChannel *channel, const map<wstring, int64_t>& meta_counters, SBDChannelMetaCountersUpdateMode update_mode, bool upsert, SBDRequestInterface *completion_handler);
    void DeleteMetaCounter(SBDBaseChannel *channel, const wstring& key, SBDRequestInterface *complete_handler);
    void DeleteAllMetaCounters(SBDBaseChannel *channel, SBDRequestInterface *complete_handler);
    
    void CreateMetaData(SBDBaseChannel *channel, const map<wstring, wstring>& meta_data, SBDRequestInterface *completion_handler);
    void GetMetaData(SBDBaseChannel *channel, const vector<wstring>& keys, SBDRequestInterface *completion_handler);
    void GetAllMetaData(SBDBaseChannel *channel, SBDRequestInterface *completion_handler);
    void UpdateMetaData(SBDBaseChannel *channel, const map<wstring, wstring>& meta_data, bool upsert, SBDRequestInterface *completion_handler);
    void DeleteMetaData(SBDBaseChannel *channel, const wstring& key, SBDRequestInterface *completion_handler);
    void DeleteAllMetaData(SBDBaseChannel *channel, SBDRequestInterface *completion_handler);
    
    void LoadMessagesByTimestamp(int64_t timestamp, int64_t prev_limit, int64_t next_limit, bool include, bool reverse, SBDBaseChannel *channel, SBDMessageTypeFilter message_type, const wstring& custom_type, SBDRequestInterface *completion_handler);
    void LoadMessagesByMessageId(int64_t message_id, int64_t prev_limit, int64_t next_limit, bool include, bool reverse, SBDBaseChannel *channel, SBDMessageTypeFilter message_type, const wstring& custom_type, SBDRequestInterface *completion_handler);
    
    void BlockUser(const wstring& blocker_id, const wstring& blockee_id, SBDRequestInterface *completion_handler);
    void UnblockUser(const wstring& blocker_id, const wstring& blockee_id, SBDRequestInterface *completion_handler);
    void SetAutoAcceptInvitation(bool auto_accept, SBDRequestInterface *completion_handler);
    void GetAutoAcceptInvitation(SBDRequestInterface *completion_handler);
    
    void AcceptInvitation(const wstring& channel_url, SBDRequestInterface *completion_handler);
    void DeclineInvitation(const wstring& channel_url, SBDRequestInterface *completion_handler);
    
    void BanUser(const wstring& user_id, const wstring& channel_url, const wstring& description, int seconds, SBDRequestInterface *completion_handler);
    void UnbanUser(const wstring& user_id, const wstring& channel_url, SBDRequestInterface *completion_handler);
    void MuteUser(const wstring& user_id, const wstring& channel_url, SBDRequestInterface *completion_handler);
    void UnmuteUser(const wstring& user_id, const wstring& channel_url, SBDRequestInterface *completion_handler);
    
    void CreateUserMetaData(const wstring& user_id, const map<wstring, wstring>& meta_data, SBDRequestInterface *completion_handler);
    void UpdateUserMetaData(const wstring& user_id, const map<wstring, wstring>& meta_data, SBDRequestInterface *completion_handler);
    void DeleteUserMetaData(const wstring& user_id, const wstring& key, SBDRequestInterface *completion_handler);
    void DeleteAllUserMetaData(const wstring& user_id, SBDRequestInterface *completion_handler);
    
    void LoadOpenChannelList(const string& token, int64_t limit, const wstring& channel_name_filter, const wstring& channel_url_filter, const wstring& custom_type_filter, SBDRequestInterface *completion_handler);
	void LoadGroupChannelList(const string& token, const wstring& user_id, int64_t limit, bool include_member_list, SBDGroupChannelListOrder order, bool include_empty_channel, const vector<wstring>& channel_urls_filter, const vector<SBDUser>& users_filter_exact_match, const wstring& nickname_contains_filter, const vector<SBDUser>& users_filter_like_match, SBDGroupChannelListQueryType query_type, const wstring& custom_type_filter, SBDMemberStateFilter member_state_filter, const wstring& channel_name_filter, SBDRequestInterface *completion_handler);
	void LoadGroupChannelList(const string& token, const wstring& user_id, int64_t limit, bool include_member_list, SBDGroupChannelListOrder order, bool include_empty_channel, const vector<wstring>& channel_urls_filter, const vector<SBDUser>& users_filter_exact_match, const wstring& nickname_contains_filter, const vector<SBDUser>& users_filter_like_match, SBDGroupChannelListQueryType query_type, const wstring& custom_type_filter, SBDMemberStateFilter member_state_filter, const wstring& channel_name_filter, SBDChannelHiddenStateFilter channel_hidden_state_filter, SBDRequestInterface* completion_handler);
	void ResetGroupChannelHistory(const wstring& channel_url, SBDRequestInterface *completion_handler);
    void GetGroupChannelCount(SBDMemberStateFilter member_state_filter, SBDRequestInterface *completion_handler);
	void UploadFile(const wstring& file_path, const wstring& type, const vector<SBDThumbnailSize>& thumbnail_sizes, 
        const wstring& channel_url, SBDRequestInterface *completion_handler);
    void UploadFile(const char* file_buf, const wstring& file_name, int64_t file_size, const wstring& type, 
        const vector<SBDThumbnailSize>& thumbnail_sizes, const wstring& channel_url, 
        SBDRequestInterface* completion_handler);
};

#endif /* SENDBIRD_SBDAPICLIENT_H_ */
