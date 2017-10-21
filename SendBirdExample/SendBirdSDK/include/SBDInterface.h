#pragma once

//
//  SBDInterface.h
//  SendBird
//
//  Created by Jed Gyeong on 7/12/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDINTERFACE_H_
#define SENDBIRD_SBDINTERFACE_H_

#include "SBDUser.h"
#include "SBDError.h"
#include "SBDBaseInterface.h"
#include <Windows.h>

class SBDCommand;

class SBDConnectInterface : public SBDBaseInterface {
private:
	HANDLE thread;

	static DWORD WINAPI Handler(void *vargp);
    
public:
    SBDError *error;
    SBDUser user;
	
	SBDConnectInterface();
	virtual ~SBDConnectInterface() = 0;

	void RunCompletionHandler(SBDUser user, SBDError *error);

    virtual void CompletionHandler(SBDUser user, SBDError *error) {};
};

class SBDDisconnectInterface : public SBDBaseInterface {
private:
	HANDLE thread;

	static DWORD WINAPI Handler(void *vargp);
    
public:
	SBDDisconnectInterface();
	virtual ~SBDDisconnectInterface() = 0;

	void RunCompletionHandler();

    virtual void CompletionHandler() {};
};

class SBDConnectToWebSocketServerInterface : public SBDBaseInterface {
private:
	HANDLE thread;
    
	static DWORD WINAPI Handler(void *vargp);

public:
    SBDUser user;
    SBDError *error;
    SBDConnectInterface *connect_completion_handler;
	SBDConnectToWebSocketServerInterface();
	virtual ~SBDConnectToWebSocketServerInterface() = 0;

	void RunCompletionHandler(SBDUser user, SBDError *error);
    
    virtual void ConnectToWebSocketServerCompletionHandler(SBDUser user, SBDError *error) {};
};

class SBDSendCommandInterface : public SBDBaseInterface {
private:
	HANDLE thread;

	static DWORD WINAPI Handler(void *vargp);

public:
    SBDCommand *command;
    SBDError *error;

	SBDSendCommandInterface();
	virtual ~SBDSendCommandInterface() = 0;

	void RunCompletionHandler(SBDCommand *command, SBDError *error);

	virtual void CompletionHandler(SBDCommand *command, SBDError *error) {};
};

#endif /* SENDBIRD_SBDINTERFACE_H_ */
