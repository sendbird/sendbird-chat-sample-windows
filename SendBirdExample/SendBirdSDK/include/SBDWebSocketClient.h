//
//  SBDWebSocketClient.h
//  SendBird
//
//  Created by Jed Gyeong on 6/29/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDWEBSOCKETCLIENT_H_
#define SENDBIRD_SBDWEBSOCKETCLIENT_H_

#include <iostream>
#include <cstring>

#include "SendBird.h"
#include "SBDCommand.h"
#include "SBDTypes.h"
#include "SBDError.h"
#include "SBDInterface.h"
#include "SBDTimer.h"

using namespace std;

class SBDCommand;
class SBDPingerHandler;
class SBDWatchdogHandler;
class SBDWebSocketClient;

enum demo_protocols {
    SBD_PROTOCOL_SENDBIRD,
    /* always last */
    SBD_PROTOCOL_COUNT
};

class SBDWebSocketClientInterface : public SBDBaseInterface {
public:
    bool reconnect;
    SBDError *error;
    SBDConnectToWebSocketServerInterface *completion_handler;
    
    virtual void WebSocketClientInterfaceOnOpen() {};
    virtual void WebSocketClientInterfaceOnMessage(string message) {};
    virtual void WebSocketClientInterfaceOnClose() {};
    virtual void WebSocketClientInterfaceOnError(SBDError *error) {};
};

class SBDWebSocketSendCommandInterface : public SBDBaseInterface {
public:
    virtual void SBDWebSocketCompletionHandler(SBDError *error) {};
};

class SBDPingerHandler : public SBDTimer {
public:
    SBDWebSocketClient *websocket_client;

	SBDPingerHandler(DWORD time_interval, bool repeat, SBDWebSocketClient *websocket_client) : SBDTimer(time_interval, repeat) {
        this->websocket_client = websocket_client;
    }
    
    void RunCallback();
};


class SBDWatchdogHandler : public SBDTimer {
public:
    SBDWebSocketClient *websocket_client;
    SBDPingerHandler *pinger;

	SBDWatchdogHandler(DWORD time_interval, bool repeat, SBDWebSocketClient *websocket_client, SBDPingerHandler *pinger) : SBDTimer(time_interval, repeat) {
        this->websocket_client = websocket_client;
    }
    
    void RunCallback();
};

class SBDWebSocketProperty {

};

/**
 *  Internal use only.
 */
class SBDWebSocketClient {
private:
    pthread_t tid;
	string host;
    SBDWatchdogHandler *watchdog;
    SBDPingerHandler *pinger;
    pthread_mutex_t pinger_lock;
	SBDWebSocketConnectionState connection_state;

public:
    wstring user_id;
    wstring access_token;

	SBDWebSocketProperty *websocket_property;
    
    explicit SBDWebSocketClient();
    
    ~SBDWebSocketClient();

    void StartWebSocket(const wstring& host);
    SBDWebSocketClientInterface *event_handler;
    SBDDisconnectInterface *disconnect_completion_handler;
    
	time_t last_active_seconds;
    
    bool explict_disconnect;
    
    void Disconnect(SBDDisconnectInterface *completion_handler);
    void Connect();
    void ForceDisconnect();
    void SendCommand(string full_command, SBDWebSocketSendCommandInterface *completion_handler);
    void SetConnectState(SBDWebSocketConnectionState state);
    SBDWebSocketConnectionState GetConnectState();
    void StartWatchdog();
    void StopWatchdog();
    void StartPinger();
    void StopPinger();
    void Quit();
	void Active();
    
    static void *WebSocketThreadHandler(void *vargp);
};

#endif /* SENDBIRD_SBDWEBSOCKETCLIENT_H_ */
