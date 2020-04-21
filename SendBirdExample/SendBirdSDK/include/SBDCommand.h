//
//  SBDCommand.h
//  SendBird
//
//  Created by Jed Gyeong on 6/28/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDCOMMAND_H_
#define SENDBIRD_SBDCOMMAND_H_

#include <iostream>
#include <vector>

#include "SendBird.h"
#include "SBDUserMessageParams.h"

using namespace std;

class SBDCommand {
public:
    string cmd;
    string payload; // CMD+JSON
    string request_id;
    
public:
    SBDCommand() {};
    SBDCommand(const string& message);
    SBDCommand(const string& cmd, const string& payload, const string& request_id);

    bool IsAckRequired();
    bool HasRequestId();
    int64_t GetTimestamp();
    
    static SBDCommand *BuildCommand(const string& cmd, const string& json_payload, const string& request_id);
    
    static SBDCommand *BuildEnterChannel(SBDOpenChannel *channel);
    static SBDCommand *BuildExitChannel(SBDOpenChannel *channel);
    static SBDCommand *BuildUserMessage(SBDBaseChannel *channel, const wstring& message_text, const wstring& data, const wstring& custom_type, const vector<wstring>& target_languages);
	static SBDCommand *BuildUserMessage(SBDBaseChannel* channel, const SBDUserMessageParams params);
    static SBDCommand *BuildUserMessageUpdate(SBDBaseChannel *channel, int64_t message_id, const wstring& message_text, const wstring& data, const wstring& custom_type);
    // TODO: Add thumbnails.
    static SBDCommand *BuildFileMessage(SBDBaseChannel *channel, const wstring& file_url, const wstring& file_name, const wstring& mime_type, int64_t size, const wstring& data, const string& request_id, const wstring& custom_type, bool require_auth, const vector<SBDThumbnail>& thumbnails);
    static SBDCommand *BuildFileMessageUpdate(SBDBaseChannel *channel, int64_t message_id, const wstring& data, const wstring& custom_type);
    static SBDCommand *BuildPing();
    static SBDCommand *BuildStartTyping(SBDGroupChannel *channel, int64_t start_at);
    static SBDCommand *BuildEndTyping(SBDGroupChannel *channel, int64_t end_at);
    static SBDCommand *BuildReadMessage(SBDGroupChannel *channel);
    static SBDCommand *BuildUpdateUserMessage(const wstring& channel_url, int64_t message_id, const wstring& message_text, const wstring& data, const wstring& custom_type);
	
	// TODO: MYH.
	static SBDCommand *BuildUpdateUserMessage(const wstring& channel_url, int64_t message_id, const SBDUserMessageParams& params);
	static SBDCommand *BuildUpdateUserMessage(const wstring& channel_url, int64_t message_id, const vector<SBDMessageMetaArray>& metaarrays, SBDMetaArrayUpdateMode mode, bool upsert);

	static SBDCommand *BuildUpdateFileMessage(const wstring& channel_url, int64_t message_id, const wstring& data, const wstring& custom_type);

	string GetFullCommand();
    
private:
    void Decode(const string& message);
};

#endif /* SENDBIRD_SBDCOMMAND_H_ */
