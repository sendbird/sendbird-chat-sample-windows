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

using namespace std;

class SBDCommand {
public:
    string cmd;
    string payload; // CMD+JSON
    string request_id;
    
public:
    SBDCommand() {};
    SBDCommand(string message);
    SBDCommand(string cmd, string payload, string request_id);

    bool IsAckRequired();
    bool HasRequestId();
    
    static SBDCommand *BuildCommand(string cmd, string json_payload, string request_id);
    
    static SBDCommand *BuildEnterChannel(SBDOpenChannel *channel);
    static SBDCommand *BuildExitChannel(SBDOpenChannel *channel);
    static SBDCommand *BuildUserMessage(SBDBaseChannel *channel, wstring message_text, wstring data, wstring custom_type, vector<wstring> target_languages);
    static SBDCommand *BuildUserMessageUpdate(SBDBaseChannel *channel, int64_t message_id, wstring message_text, wstring data, wstring custom_type);
    // TODO: Add thumbnails.
    static SBDCommand *BuildFileMessage(SBDBaseChannel *channel, wstring file_url, wstring file_name, wstring mime_type, int64_t size, wstring data, string request_id, wstring custom_type, bool require_auth, vector<SBDThumbnail> thumbnails);
    static SBDCommand *BuildFileMessageUpdate(SBDBaseChannel *channel, int64_t message_id, wstring data, wstring custom_type);
    static SBDCommand *BuildPing();
    static SBDCommand *BuildStartTyping(SBDGroupChannel *channel, int64_t start_at);
    static SBDCommand *BuildEndTyping(SBDGroupChannel *channel, int64_t end_at);
    static SBDCommand *BuildReadMessage(SBDGroupChannel *channel);
    static SBDCommand *BuildUpdateUserMessage(wstring channel_url, int64_t message_id, wstring message_text, wstring data, wstring custom_type);
    static SBDCommand *BuildUpdateFileMessage(wstring channel_url, int64_t message_id, wstring data, wstring custom_type);

	string GetFullCommand();
    
private:
    void Decode(string message);
};

#endif /* SENDBIRD_SBDCOMMAND_H_ */
