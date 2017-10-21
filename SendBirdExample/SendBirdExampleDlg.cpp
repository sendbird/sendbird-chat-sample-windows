
// SendBirdExampleDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "afx.h"
#include "SendBirdExample.h"
#include "SendBirdExampleDlg.h"
#include "afxdialogex.h"
#include "SendBird.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_SEND_USER_MESSAGE					(WM_USER + 1)
#define WM_RECV_USER_MESSAGE					(WM_USER + 2)
#define WM_CREATE_OPEN_CHANNEL					(WM_USER + 3)
#define WM_LOAD_PREV_MESSAGES					(WM_USER + 4)
#define WM_CLEAR_ALL_CONTROLS					(WM_USER + 5)
#define WM_REFRESH_GROUP_CHANNELS				(WM_USER + 6)
#define WM_DISPLAY_STATUS_MESSAGE				(WM_USER + 7)
#define WM_CREATE_GROUP_CHANNEL					(WM_USER + 8)
#define WM_ENTERED_OR_JOINED_CHANNEL			(WM_USER + 9)
#define WM_LOAD_MORE_PARTICIPANT_LIST			(WM_USER + 10)
#define WM_REFRESH_GROUPCHANNEL_MEMBER_LIST		(WM_USER + 11)
#define WM_EXIT_OPENCHANNEL						(WM_USER + 12)
#define WM_BAN_USER								(WM_USER + 13)
#define WM_LOAD_MORE_BAN_USER_LIST				(WM_USER + 14)
#define WM_UNBAN_USER							(WM_USER + 15)
#define WM_MUTE_USER							(WM_USER + 16)
#define WM_LOAD_MORE_MUTE_USER_LIST				(WM_USER + 17)
#define WM_UNMUTE_USER							(WM_USER + 18)
#define WM_LOAD_MORE_BLOCK_USER_LIST			(WM_USER + 19)
#define WM_UNBLOCK_USER							(WM_USER + 20)
#define WM_BLOCK_USER							(WM_USER + 21)
#define WM_LEAVE_GROUP_CHANNEL					(WM_USER + 22)
#define WM_HIDE_GROUP_CHANNEL					(WM_USER + 23)
#define WM_UPDATE_TYPING_STATUS					(WM_USER + 24)

// CAboutDlg dialog used for App About

class StatusParam {
public:
	wstring status_message;

	StatusParam(wstring status_message) {
		this->status_message = status_message;
	}

	~StatusParam() {
		TRACE("Destructing StatusParam");
	}
};

class MessageParam {
public:
	SBDBaseMessage *message;

	MessageParam(SBDBaseMessage *message) {
		this->message = message;
	}
};

class CurrentChannelParam {
public:
	wstring current_channel_info;

	CurrentChannelParam(wstring current_channel_info) {
		this->current_channel_info = current_channel_info;
	}
};

class OpenChannelParam {
public:
	SBDOpenChannel *open_channel;

	OpenChannelParam(SBDOpenChannel *open_channel) {
		this->open_channel = open_channel;
	}
};

class GroupChannelParam {
public:
	SBDGroupChannel *group_channel;

	GroupChannelParam(SBDGroupChannel *group_channel) {
		this->group_channel = group_channel;
	}
};


class MessageListParam {
public:
	vector<SBDBaseMessage *> messages;

	MessageListParam(vector<SBDBaseMessage *> messages) {
		this->messages.resize((int)messages.size());
		std::copy(messages.begin(), messages.end(), this->messages.begin());
	}
};

class GroupChannelListParam {
public:
	vector<SBDGroupChannel *> group_channels;

	GroupChannelListParam(vector<SBDGroupChannel *> group_channels) {
		this->group_channels.resize((int)group_channels.size());
		std::copy(group_channels.begin(), group_channels.end(), this->group_channels.begin());
	}
};

class UserListParm {
public:
	vector<SBDUser> users;

	UserListParm(vector<SBDUser> users) {
		this->users.resize((int)users.size());
		std::copy(users.begin(), users.end(), this->users.begin());
	}
};

class UserParam {
public:
	SBDUser user;

	UserParam(SBDUser user) {
		this->user = user;
	}
};

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSendBirdExampleDlg dialog
class SendBirdReconnectEventHandler : public SBDReconnectionInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdReconnectEventHandler(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void Started() {
		PostMessage(this->dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Start to reconnect.")));
	}

	void Succeeded() {
		PostMessage(this->dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Reconnection is succeeded.")));
	}

	void Failed() {
		PostMessage(this->dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Reconnection is failed.")));
	}

	void Cancelled() {
		PostMessage(this->dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Reconnection is cancelled.")));
	}

};

class SendBirdChannelEventHandler : public SBDChannelInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdChannelEventHandler(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void MessageReceived(SBDBaseChannel *channel, SBDBaseMessage *message) {
		if (this->dlg->m_CurrGroupChannel != NULL) {
			if (!this->dlg->m_CurrGroupChannel->channel_url.compare(channel->channel_url)) {
				MessageParam *param = new MessageParam(message);
				PostMessage(this->dlg->GetSafeHwnd(), WM_RECV_USER_MESSAGE, 0, (LPARAM)param);
				
				return;
			}
		}
		else if (this->dlg->m_CurrOpenChannel != NULL) {
			if (!this->dlg->m_CurrOpenChannel->channel_url.compare(channel->channel_url)) {
				MessageParam *param = new MessageParam(message);
				PostMessage(this->dlg->GetSafeHwnd(), WM_RECV_USER_MESSAGE, 0, (LPARAM)param);

				return;
			}
		}

		delete message;
	}

	void MessageUpdated(SBDBaseChannel *channel, SBDBaseMessage *message) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"MessageUpdated.")));

		if (this->dlg->m_CurrGroupChannel != NULL) {
			if (!this->dlg->m_CurrGroupChannel->channel_url.compare(channel->channel_url)) {

			}
		}
		else if (this->dlg->m_CurrOpenChannel != NULL) {
			if (!this->dlg->m_CurrOpenChannel->channel_url.compare(channel->channel_url)) {

			}
		}
	}

	void ReadReceiptUpdated(SBDGroupChannel *channel) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"ReadReceiptUpdated.")));

		if (this->dlg->m_CurrGroupChannel != NULL) {
			if (!this->dlg->m_CurrGroupChannel->channel_url.compare(channel->channel_url)) {

			}
		}
	}

	void TypingStatusUpdated(SBDGroupChannel *channel) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"TypingStatusUpdated.")));
		if (this->dlg->m_CurrGroupChannel != NULL) {
			if (!this->dlg->m_CurrGroupChannel->channel_url.compare(channel->channel_url)) {
				GroupChannelParam *param = new GroupChannelParam(channel);
				PostMessage(dlg->GetSafeHwnd(), WM_UPDATE_TYPING_STATUS, 0, (LPARAM)param);
			}
		}
	}

	void InvitationReceived(SBDGroupChannel *channel, vector<SBDUser> invitees, SBDUser inviter) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"InvitationReceived.")));
	}

	void InvitationDeclined(SBDGroupChannel *channel, SBDUser invitee, SBDUser inviter) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"InvitationDeclined.")));
	}

	void UserJoined(SBDGroupChannel *channel, SBDUser user) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"UserJoined.")));
	}

	void UserLeft(SBDGroupChannel *channel, SBDUser user) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"UserLeft.")));
	}

	void UserEntered(SBDOpenChannel *channel, SBDUser user) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"UserEntered.")));
	}

	void UserExited(SBDOpenChannel *channel, SBDUser user) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"UserExited.")));
	}

	void UserMuted(SBDOpenChannel *channel, SBDUser user) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"UserMuted.")));
	}

	void UserUnmuted(SBDOpenChannel *channel, SBDUser user) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"UserUnmuted.")));
	}

	void UserBanned(SBDOpenChannel *channel, SBDUser user) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"UserBanned.")));
	}

	void UserUnbanned(SBDOpenChannel *channel, SBDUser user) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"UserUnbanned.")));
	}

	void ChannelFrozen(SBDOpenChannel *channel) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"ChannelFrozen.")));
	}

	void ChannelUnfrozen(SBDOpenChannel *channel) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"ChannelUnfrozen.")));
	}

	void ChannelChanged(SBDBaseChannel *channel) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"ChannelChanged.")));
	}

	void ChannelDeleted(wstring channel_url, SBDChannelType channel_type) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"ChannelDeleted.")));
	}

	void MessageDeleted(SBDBaseChannel *channel, uint64_t message_id) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"MessageDeleted.")));
	}

	void ChannelMetaDataCreated(SBDBaseChannel *channel, map<wstring, wstring> created_meta_data) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"ChannelMetaDataCreated.")));
	}

	void ChannelMetaDataUpdated(SBDBaseChannel *channel, map<wstring, wstring> updated_meta_data) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"ChannelMetaDataUpdated.")));
	}

	void ChannelMetaDataDeleted(SBDBaseChannel *channel, vector<wstring> deleted_meta_data) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"ChannelMetaDataDeleted.")));
	}

	void ChannelMetaCountersCreated(SBDBaseChannel *channel, map<wstring, int64_t> created_meta_counters) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"ChannelMetaCountersCreated.")));
	}

	void ChannelMetaCountersUpdated(SBDBaseChannel *channel, map<wstring, int64_t> updated_meta_counters) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"ChannelMetaCountersUpdated.")));
	}

	void ChannelMetaCountersDeleted(SBDBaseChannel *channel, vector<wstring> deleted_meta_counters) {
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"ChannelMetaCountersDeleted.")));
	}
};


CSendBirdExampleDlg::CSendBirdExampleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SENDBIRDEXAMPLE_DIALOG, pParent)
	, m_UserId(_T(""))
	, m_Nickname(_T(""))
	, m_ChatMessageText(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_MinMessageTimestamp = INT64_MAX;
	m_MyGroupChannelListQuery = NULL;
	m_OpenChannelListQuery = NULL;
	m_ParticipantListQuery = NULL;
	m_GroupChannelMemberListQuery = NULL;

	m_IsSenderSelected = false;
	m_IsParticipantSelected = false;
	m_IsMutedUserSelected = false;
	m_IsBannedUserSelected = false;
	m_IsMemberSelected = false;
	m_IsBlockedUserSelected = false;

	SBDMain::AddChannelHandler(new SendBirdChannelEventHandler(this), L"CSendBirdExampleDlg");
	SBDMain::AddReconnectionHandler(new SendBirdReconnectEventHandler(this), L"CSendBirdExampleDlg");
}

void CSendBirdExampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USER_ID, m_UserIdEdit);
	DDX_Control(pDX, IDC_EDIT_NICKNAME, m_NicknameEdit);

	DDX_Control(pDX, IDC_LIST_CHAT_MSG, m_ListBoxChatMsg);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_EditChatMessage);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_ChatMessageText);
	DDX_Control(pDX, IDC_LIST_OPENCHANNEL, m_ListBoxOpenChannel);
	DDX_Control(pDX, IDC_LIST_STATUS, m_StatusListBox);
	DDX_Control(pDX, IDC_LIST_OPENCHANNEL_OPS, m_ListOpenChannelOps);
	DDX_Control(pDX, IDC_LIST_GROUPCHANNEL_INVITEES, m_ListGroupChannelInvitees);
	DDX_Control(pDX, IDC_EDIT_NEW_OPENCHANNEL_NAME, m_EditCreateOpenChannelName);
	DDX_Control(pDX, IDC_EDIT_OPENCHANNEL_ADD_OP, m_EditOpUserID);
	DDX_Control(pDX, IDC_LISTBOX_GROUPCHANNEL, m_ListBoxGroupChannels);
	DDX_Control(pDX, IDC_EDIT_NEW_GROUPCHANNEL_INVITEE, m_EditBoxNewInviteeId);
	DDX_Control(pDX, IDC_EDIT_NEW_GROUPCHANNEL_NAME, m_EditNewGroupChannelName);
	DDX_Control(pDX, IDC_LISTBOX_PARTICIPANTS, m_ListBoxParticipants);
	DDX_Control(pDX, IDC_LISTBOX_GROUPCHANNEL_MEMBERS, m_ListBoxGroupChannelMembers);
	DDX_Control(pDX, IDC_LISTBOX_BANNED_USERS, m_ListBoxBannedUsers);
	DDX_Control(pDX, IDC_LISTBOX_GROUPCHANNEL4, m_ListBoxMutedUsers);
	DDX_Control(pDX, IDC_LISTBOX_BLOCKED_LIST, m_ListBoxBlockedUsers);
	DDX_Control(pDX, IDC_STEXT_CHANNEL_NAME, m_STextChannelName);
	DDX_Control(pDX, IDC_STEXT_TYPING, m_STextTypingMembers);
}

BEGIN_MESSAGE_MAP(CSendBirdExampleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSendBirdExampleDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CONNECT, &CSendBirdExampleDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_SEND, &CSendBirdExampleDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_REFRESH_OPENCHANNEL, &CSendBirdExampleDlg::OnBnClickedBtnRefreshOpenchannel)
	ON_BN_CLICKED(IDC_BTN_LOADMORE_OPENCHANNEL, &CSendBirdExampleDlg::OnBnClickedBtnLoadmoreOpenchannel)
	ON_BN_CLICKED(IDC_BTN_CREATE_NEW_OPENCHANNEL, &CSendBirdExampleDlg::OnBnClickedBtnCreateNewOpenchannel)
	ON_LBN_DBLCLK(IDC_LIST_OPENCHANNEL, &CSendBirdExampleDlg::OnLbnDblclkListOpenchannel)
	ON_BN_CLICKED(IDC_BTN_ADD_OPS, &CSendBirdExampleDlg::OnBnClickedBtnAddOps)
	ON_BN_CLICKED(IDC_BTN_LOADPREVMSG, &CSendBirdExampleDlg::OnBnClickedBtnLoadprevmsg)

	ON_MESSAGE(WM_SEND_USER_MESSAGE, &CSendBirdExampleDlg::UpdateUISendMessage)
	ON_MESSAGE(WM_RECV_USER_MESSAGE, &CSendBirdExampleDlg::UpdateUIReceiveMessage)
	ON_MESSAGE(WM_CREATE_OPEN_CHANNEL, &CSendBirdExampleDlg::UpdateUIInsertOpenChannelTopPostion)
	ON_MESSAGE(WM_LOAD_PREV_MESSAGES, &CSendBirdExampleDlg::UpdateUILoadPreviousMessages)
	ON_MESSAGE(WM_CLEAR_ALL_CONTROLS, &CSendBirdExampleDlg::UpdateUIClearAllControls)
	ON_MESSAGE(WM_REFRESH_GROUP_CHANNELS, &CSendBirdExampleDlg::UpdateUIRefreshMyGroupChannelList)
	ON_MESSAGE(WM_DISPLAY_STATUS_MESSAGE, &CSendBirdExampleDlg::UpdateUIDisplayStatusMessage)
	ON_MESSAGE(WM_CREATE_GROUP_CHANNEL, &CSendBirdExampleDlg::UpdateUIInsertGroupChannelTopPostion)
	ON_MESSAGE(WM_ENTERED_OR_JOINED_CHANNEL, &CSendBirdExampleDlg::UpdateUIEnteredOrJoinedChannel)
	ON_MESSAGE(WM_LOAD_MORE_PARTICIPANT_LIST, &CSendBirdExampleDlg::UpdateUILoadMoreParticipantList)
	ON_MESSAGE(WM_REFRESH_GROUPCHANNEL_MEMBER_LIST, &CSendBirdExampleDlg::UpdateUIRefreshGroupChannelMemberList)
	ON_MESSAGE(WM_EXIT_OPENCHANNEL, &CSendBirdExampleDlg::UpdateUIExitOpenChannel)
	ON_MESSAGE(WM_BAN_USER, &CSendBirdExampleDlg::UpdateUIBanUser)
	ON_MESSAGE(WM_LOAD_MORE_BAN_USER_LIST, &CSendBirdExampleDlg::UpdateUILoadMoreBanUserList)
	ON_MESSAGE(WM_UNBAN_USER, &CSendBirdExampleDlg::UpdateUIUnbanUser)
	ON_MESSAGE(WM_MUTE_USER, &CSendBirdExampleDlg::UpdateUIMuteUser)
	ON_MESSAGE(WM_LOAD_MORE_MUTE_USER_LIST, &CSendBirdExampleDlg::UpdateUILoadMoreMuteUserList)
	ON_MESSAGE(WM_UNMUTE_USER, &CSendBirdExampleDlg::UpdateUIUnmuteUser)
	ON_MESSAGE(WM_LOAD_MORE_BLOCK_USER_LIST, &CSendBirdExampleDlg::UpdateUILoadMoreBlockUserList)
	ON_MESSAGE(WM_UNBLOCK_USER, &CSendBirdExampleDlg::UpdateUIUnblockUser)
	ON_MESSAGE(WM_BLOCK_USER, &CSendBirdExampleDlg::UpdateUIBlockUser)
	ON_MESSAGE(WM_LEAVE_GROUP_CHANNEL, &CSendBirdExampleDlg::UpdateUILeaveHideGroupChannel)
	ON_MESSAGE(WM_UPDATE_TYPING_STATUS, &CSendBirdExampleDlg::UpdateUITypingStatus)
	
	
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, &CSendBirdExampleDlg::OnBnClickedBtnDisconnect)
	ON_BN_CLICKED(IDC_BTN_REFRESH_GROUPCHANNEL, &CSendBirdExampleDlg::OnBnClickedBtnRefreshGroupchannel)
	ON_BN_CLICKED(IDC_BTN_ADD_INVITEES, &CSendBirdExampleDlg::OnBnClickedBtnAddInvitees)
	ON_BN_CLICKED(IDC_BTN_CREATE_NEW_GROUPCHANNEL, &CSendBirdExampleDlg::OnBnClickedBtnCreateNewGroupchannel)
	ON_LBN_DBLCLK(IDC_LISTBOX_GROUPCHANNEL, &CSendBirdExampleDlg::OnLbnDblclkListboxGroupchannel)
	ON_BN_CLICKED(IDC_BTN_REFRESH_PARTICIPANTS, &CSendBirdExampleDlg::OnBnClickedBtnRefreshParticipants)
	ON_BN_CLICKED(IDC_BTN_LOADMORE_PARTICIPANTS, &CSendBirdExampleDlg::OnBnClickedBtnLoadmoreParticipants)
	ON_BN_CLICKED(IDC_BTN_REFRESH_MEMBERS, &CSendBirdExampleDlg::OnBnClickedBtnRefreshMembers)
	ON_BN_CLICKED(IDC_BTN_EXIT_OPENCHANNEL, &CSendBirdExampleDlg::OnBnClickedBtnExitOpenchannel)
	ON_LBN_DBLCLK(IDC_LIST_CHAT_MSG, &CSendBirdExampleDlg::OnLbnDblclkListChatMsg)
	ON_BN_CLICKED(IDC_BTN_BAN_USER, &CSendBirdExampleDlg::OnBnClickedBtnBanUser)
	ON_BN_CLICKED(IDC_BTN_REFRESH_BANNED, &CSendBirdExampleDlg::OnBnClickedBtnRefreshBanned)
	ON_BN_CLICKED(IDC_BTN_LOADMORE_BANNED, &CSendBirdExampleDlg::OnBnClickedBtnLoadmoreBanned)
	ON_LBN_DBLCLK(IDC_LISTBOX_BANNED_USERS, &CSendBirdExampleDlg::OnLbnDblclkListboxBannedUsers)
	ON_BN_CLICKED(IDC_BTN_UNBAN_USER, &CSendBirdExampleDlg::OnBnClickedBtnUnbanUser)
	ON_BN_CLICKED(IDC_BTN_MUTE_USER, &CSendBirdExampleDlg::OnBnClickedBtnMuteUser)
	ON_BN_CLICKED(IDC_BTN_REFRESH_MUTED, &CSendBirdExampleDlg::OnBnClickedBtnRefreshMuted)
	ON_BN_CLICKED(IDC_BTN_LOADMORE_MUTED, &CSendBirdExampleDlg::OnBnClickedBtnLoadmoreMuted)
	ON_LBN_DBLCLK(IDC_LISTBOX_MUTED_USERS, &CSendBirdExampleDlg::OnLbnDblclkListboxMutedUsers)
	ON_BN_CLICKED(IDC_BTN_REFRESH_MUTED, &CSendBirdExampleDlg::OnBnClickedBtnRefreshMuted)
	ON_BN_CLICKED(IDC_BTN_LOADMORE_MUTED, &CSendBirdExampleDlg::OnBnClickedBtnLoadmoreMuted)
	ON_BN_CLICKED(IDC_BTN_UNMUTE_USER, &CSendBirdExampleDlg::OnBnClickedBtnUnmuteUser)
	ON_BN_CLICKED(IDC_BTN_RECONNECT, &CSendBirdExampleDlg::OnBnClickedBtnReconnect)
	ON_BN_CLICKED(IDC_BTN_LOAD_MORE_BLOCKED_LIST, &CSendBirdExampleDlg::OnBnClickedBtnLoadMoreBlockedList)
	ON_BN_CLICKED(IDC_BTN_REFRESH_BLOCKED_LIST, &CSendBirdExampleDlg::OnBnClickedBtnRefreshBlockedList)
	ON_BN_CLICKED(IDC_BTN_UNBLOCK_USER, &CSendBirdExampleDlg::OnBnClickedBtnUnblockUser)
	ON_BN_CLICKED(IDC_BTN_BLOCK_USER, &CSendBirdExampleDlg::OnBnClickedBtnBlockUser)
	ON_BN_CLICKED(IDC_BTN_LEAVE_GROUPCHANNEL, &CSendBirdExampleDlg::OnBnClickedBtnLeaveGroupchannel)
	ON_BN_CLICKED(IDC_BTN_HIDE_GROUPCHANNEL, &CSendBirdExampleDlg::OnBnClickedBtnHideGroupchannel)
	ON_EN_CHANGE(IDC_EDIT_MESSAGE, &CSendBirdExampleDlg::OnEnChangeEditMessage)
	ON_LBN_DBLCLK(IDC_LISTBOX_BLOCKED_LIST, &CSendBirdExampleDlg::OnLbnDblclkListboxBlockedList)
END_MESSAGE_MAP()

// CSendBirdExampleDlg message handlers

BOOL CSendBirdExampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSendBirdExampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSendBirdExampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSendBirdExampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSendBirdExampleDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

class SendBirdUpdateCurrentUserHandler : public SBDUpdateUserInfoInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdUpdateCurrentUserHandler(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	~SendBirdUpdateCurrentUserHandler() {
		TRACE("Destructing SendBirdUpdateCurrentUserHandler");
	}

	void CompletionHandler(SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Updating user info is error.")));

			return;
		}

		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"The current user info has been updated.")));
	}
};

class SendBirdConnectHandler : public SBDConnectInterface {
public:
	CString nickname;
	CSendBirdExampleDlg *dlg;

	SendBirdConnectHandler(CString nickname, CSendBirdExampleDlg *dlg) {
		this->nickname = nickname;
		this->dlg = dlg;
	}

	~SendBirdConnectHandler() {
		TRACE("Destructing SendBirdConnectHandler");
	}

	void CompletionHandler(SBDUser user, SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"The connection is failed.")));

			return;
		}

		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"The connection is succeeded.")));

		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"User info is updating.")));
		wstring nicknamew((LPCTSTR)nickname);
		SBDMain::UpdateCurrentUserInfo(nicknamew, SBD_NULL_WSTRING, new SendBirdUpdateCurrentUserHandler(this->dlg));
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnConnect()
{
	m_UserIdEdit.GetWindowText(m_UserId);
	m_NicknameEdit.GetWindowText(m_Nickname);
	wstring user_idw((LPCTSTR)m_UserId);
	//SBDMain::Connect(user_idw, SBD_NULL_WSTRING, L"http://api-intoz.sendbird.com:9800", L"ws://ws-intoz.sendbird.com:9700", new SendBirdConnectHandler(m_Nickname, this));
	DisplayStatus(L"Connecting...");
	SBDMain::Connect(user_idw, SBD_NULL_WSTRING, new SendBirdConnectHandler(m_Nickname, this));
}

class SendBirdOpenChannelSendUserMessageHandler : public SBDSendUserMessageInterface {
public:
	CSendBirdExampleDlg *dlg;
	SendBirdOpenChannelSendUserMessageHandler(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler(SBDUserMessage *user_message, SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Sending user message is failed.")));

			return;
		}

		MessageParam *param = new MessageParam(user_message);
		PostMessage(this->dlg->GetSafeHwnd(), WM_SEND_USER_MESSAGE, 0, (LPARAM)param);
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnSend()
{
	m_EditChatMessage.GetWindowText(m_ChatMessageText);
	m_EditChatMessage.SetWindowText(L"");
	
	if (m_CurrOpenChannel) {
		m_CurrOpenChannel->SendUserMessage(wstring(m_ChatMessageText), wstring(m_ChatMessageText), L"", vector<wstring>(), new SendBirdOpenChannelSendUserMessageHandler(this));
		DisplayStatus(L"The user message is sent in the open channel.");
	}
	else if (m_CurrGroupChannel) {
		m_CurrGroupChannel->EndTyping();
		m_CurrGroupChannel->SendUserMessage(wstring(m_ChatMessageText), wstring(m_ChatMessageText), L"", vector<wstring>(), new SendBirdOpenChannelSendUserMessageHandler(this));
		DisplayStatus(L"The user message is sent in the group channel.");
	}
	else {
		DisplayStatus(L"Channel selection is required.");
	}
}

class SendBirdOpenChannelListQueryHandler : public SBDLoadNextOpenChannelListInterface {
public:
	CSendBirdExampleDlg *dlg;
	SendBirdOpenChannelListQueryHandler(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler(vector<SBDOpenChannel *> channels, SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Open channel list query is error.")));

			return;
		}

		for (size_t i = 0; i < channels.size(); i++) {
			dlg->m_OpenChannelList.push_back(channels[i]);
			wstring channel_name_w = channels[i]->name;

			dlg->m_ListBoxOpenChannel.InsertString(-1, (LPCTSTR)(channel_name_w.c_str()));
			dlg->m_ListBoxOpenChannel.SetCurSel(dlg->m_ListBoxOpenChannel.GetCount() - 1);
		}

		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Open channel list was queried.")));
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnRefreshOpenchannel()
{
	if (m_OpenChannelListQuery != NULL) {
		delete m_OpenChannelListQuery;
		m_OpenChannelListQuery = NULL;
	}

	DisplayStatus(L"Open channel list is querying.");
	m_OpenChannelList.clear();
	m_ListBoxOpenChannel.ResetContent();
	m_OpenChannelListQuery = SBDOpenChannel::CreateOpenChannelListQuery();
	m_OpenChannelListQuery->limit = 20;
	m_OpenChannelListQuery->LoadNextPage(new SendBirdOpenChannelListQueryHandler(this));
}

void CSendBirdExampleDlg::OnBnClickedBtnLoadmoreOpenchannel()
{
	if (m_OpenChannelListQuery == NULL) {
		DisplayStatus(L"There isn't a query instance. Refresh first.");

		return;
	}

	DisplayStatus(L"Querying the open channel list.");
	m_OpenChannelListQuery->LoadNextPage(new SendBirdOpenChannelListQueryHandler(this));
}

class SendBirdOpenChannelCreateHandler : public SBDCreateOpenChannelInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdOpenChannelCreateHandler(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler(SBDOpenChannel *channel, SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Creating open channel was failed.")));

			this->dlg->DisplayStatus(L"Creating open channel is failed.");

			return;
		}
		
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"The open channel was created.")));

		OpenChannelParam *param = new OpenChannelParam(channel);
		PostMessage(dlg->GetSafeHwnd(), WM_CREATE_OPEN_CHANNEL, 0, (LPARAM)param);
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnCreateNewOpenchannel()
{
	CString channel_name;
	m_EditCreateOpenChannelName.GetWindowText(channel_name);
	m_EditCreateOpenChannelName.SetWindowText(L"");

	if (channel_name.GetLength() == 0) {
		DisplayStatus(L"Channel name is required.");

		return;
	}

	vector<wstring> op_ids;
	for (int i = 0; i < m_ListOpenChannelOps.GetCount(); i++) {
		CString op_id;
		m_ListOpenChannelOps.GetText(i, op_id);
		op_ids.push_back(op_id.GetString());
	}
	m_ListOpenChannelOps.ResetContent();

	DisplayStatus(L"Creating an open channel.");
	SBDOpenChannel::CreateChannel(wstring(channel_name), SBD_NULL_WSTRING, SBD_NULL_WSTRING, SBD_NULL_WSTRING, op_ids, SBD_NULL_WSTRING, new SendBirdOpenChannelCreateHandler(this));
}

class SendBirdOpenChannelEnterHandler : public SBDEnterOpenChannelInterface {
public:
	CSendBirdExampleDlg *dlg;
	SBDOpenChannel *open_channel;

	SendBirdOpenChannelEnterHandler(CSendBirdExampleDlg *dlg, SBDOpenChannel *open_channel) {
		this->dlg = dlg;
		this->open_channel = open_channel;
	}

	void CompletionHandler(SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Entering the channel is failed.")));

			return;
		}

		dlg->SetCurrentChannel(this->open_channel, SBDChannelTypeOpen);
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Entered the channel.")));
	}
};

void CSendBirdExampleDlg::OnLbnDblclkListOpenchannel()
{
	int cur_sel_pos = m_ListBoxOpenChannel.GetCurSel();

	DisplayStatus(Utils::FormattedString(L"Entering the channel(%s).", m_OpenChannelList[cur_sel_pos]->name.c_str()));

	m_OpenChannelList[cur_sel_pos]->Enter(new SendBirdOpenChannelEnterHandler(this, m_OpenChannelList[cur_sel_pos]));
}

void CSendBirdExampleDlg::OnBnClickedBtnAddOps()
{
	// TODO: Add your control notification handler code here
	CString op_id;
	m_EditOpUserID.GetWindowText(op_id);
	m_EditOpUserID.SetWindowText(L"");

	if (op_id.GetLength() == 0) {
		return;
	}

	m_ListOpenChannelOps.InsertString(-1, op_id);
}

class SendBirdLoadPreviousMessageHandler : public SBDGetMessagesInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdLoadPreviousMessageHandler(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler(vector<SBDBaseMessage *> messages, SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Message loading error.")));

			return;
		}

		MessageListParam *param = new MessageListParam(messages);
		PostMessage(this->dlg->GetSafeHwnd(), WM_LOAD_PREV_MESSAGES, 0, (LPARAM)param);
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnLoadprevmsg()
{
	if (this->m_CurrOpenChannel) {
		DisplayStatus(L"Querying the previous messages.");
		this->m_CurrOpenChannel->GetPreviousMessagesByTimestamp(m_MinMessageTimestamp, 30, false, SBDMessageTypeFilterAll, SBD_NULL_WSTRING, new SendBirdLoadPreviousMessageHandler(this));
	}
	else if (this->m_CurrGroupChannel) {
		DisplayStatus(L"Querying the previous messages.");
		this->m_CurrGroupChannel->GetPreviousMessagesByTimestamp(m_MinMessageTimestamp, 30, false, SBDMessageTypeFilterAll, SBD_NULL_WSTRING, new SendBirdLoadPreviousMessageHandler(this));
	}
	else {
		DisplayStatus(L"Channel selection is required.");
	}
}

class SendBirdDisconnectHandler : public SBDDisconnectInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdDisconnectHandler(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler() {
		// Clear all controls and variables.
		PostMessage(dlg->GetSafeHwnd(), WM_CLEAR_ALL_CONTROLS, 0, 0);
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnDisconnect()
{
	SBDMain::Disconnect(new SendBirdDisconnectHandler(this));
}

class SendBirdLoadMyGroupChannelList : public SBDLoadNextGroupChannelListInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdLoadMyGroupChannelList(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler(vector<SBDGroupChannel *> channels, SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"My group channel query is failed.")));

			return;
		}
		
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"My group channel list was queried.")));
		PostMessage(dlg->GetSafeHwnd(), WM_REFRESH_GROUP_CHANNELS, 0, (LPARAM)(new GroupChannelListParam(channels)));
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnRefreshGroupchannel()
{
	if (m_MyGroupChannelListQuery != NULL) {
		delete m_MyGroupChannelListQuery;
		m_MyGroupChannelListQuery = NULL;
	}

	DisplayStatus(L"Querying my group channel list.");
	m_ListBoxGroupChannels.ResetContent();
	m_MyGroupChannelListQuery = SBDGroupChannel::CreateMyGroupChannelListQuery();
	m_MyGroupChannelListQuery->limit = 20;
	m_MyGroupChannelListQuery->LoadNextPage(new SendBirdLoadMyGroupChannelList(this));
}

void CSendBirdExampleDlg::OnBnClickedBtnAddInvitees()
{
	CString invitee_id;
	m_EditBoxNewInviteeId.GetWindowText(invitee_id);
	m_EditBoxNewInviteeId.SetWindowText(L"");

	if (invitee_id.GetLength() == 0) {
		return;
	}

	m_ListGroupChannelInvitees.InsertString(-1, invitee_id);
}

class SendBirdCreateGroupChannelHandler : public SBDCreateGroupChannelInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdCreateGroupChannelHandler(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler(SBDGroupChannel *channel, SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Creating a group channel is failed.")));

			return;
		}

		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Group channel is created.")));
		PostMessage(dlg->GetSafeHwnd(), WM_CREATE_GROUP_CHANNEL, 0, (LPARAM)(new GroupChannelParam(channel)));
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnCreateNewGroupchannel()
{
	CString channel_name;
	m_EditNewGroupChannelName.GetWindowText(channel_name);
	m_EditNewGroupChannelName.SetWindowText(L"");

	if (channel_name.GetLength() == 0) {
		DisplayStatus(L"Channel name is required.");

		return;
	}

	vector<wstring> invitee_ids;
	for (int i = 0; i < m_ListGroupChannelInvitees.GetCount(); i++) {
		CString invitee_id;
		m_ListGroupChannelInvitees.GetText(i, invitee_id);
		invitee_ids.push_back(invitee_id.GetString());
	}
	m_ListGroupChannelInvitees.ResetContent();

	DisplayStatus(L"Creating a group channel.");
	SBDGroupChannel::CreateChannel(invitee_ids, wstring(channel_name), false, SBD_NULL_WSTRING, SBD_NULL_WSTRING, SBD_NULL_WSTRING, new SendBirdCreateGroupChannelHandler(this));
}

void CSendBirdExampleDlg::OnLbnDblclkListboxGroupchannel()
{
	int cur_sel_pos = m_ListBoxGroupChannels.GetCurSel();

	SetCurrentChannel(m_GroupChannelList[cur_sel_pos], SBDChannelTypeGroup);

	DisplayStatus(L"Querying the previous messages.");
	m_MinMessageTimestamp = INT64_MAX;
	this->m_CurrGroupChannel->GetPreviousMessagesByTimestamp(m_MinMessageTimestamp, 30, false, SBDMessageTypeFilterAll, SBD_NULL_WSTRING, new SendBirdLoadPreviousMessageHandler(this));
}

class SendBirdLoadMoreParticipantListHandler : public SBDLoadNextUserListInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdLoadMoreParticipantListHandler(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler(vector<SBDUser> users, SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Loading participant list is failed.")));

			return;
		}

		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Participant list was queried.")));
		PostMessage(dlg->GetSafeHwnd(), WM_LOAD_MORE_PARTICIPANT_LIST, 0, (LPARAM)(new UserListParm(users)));
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnRefreshParticipants()
{
	if (m_ParticipantListQuery != NULL) {
		delete m_ParticipantListQuery;
		m_ParticipantListQuery = NULL;
	}

	if (!m_CurrOpenChannel) {
		return;
	}

	DisplayStatus(L"Querying participant list.");
	m_ListBoxParticipants.ResetContent();
	
	m_ParticipantListQuery = m_CurrOpenChannel->CreateParticipantListQuery();
	m_ParticipantListQuery->limit = 3;
	m_ParticipantListQuery->LoadNextPage(new SendBirdLoadMoreParticipantListHandler(this));
}

void CSendBirdExampleDlg::OnBnClickedBtnLoadmoreParticipants()
{
	if (m_ParticipantListQuery == NULL) {
		return;
	}

	DisplayStatus(L"Querying participant list.");
	m_ParticipantListQuery->LoadNextPage(new SendBirdLoadMoreParticipantListHandler(this));
}

class SendBirdRefreshMemberListQueryHandler : public SBDRefreshGroupChannelInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdRefreshMemberListQueryHandler(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler(SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Channel refresh is failed.")));

			return;
		}

		PostMessage(dlg->GetSafeHwnd(), WM_REFRESH_GROUPCHANNEL_MEMBER_LIST, 0, 0);
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnRefreshMembers()
{
	if (m_GroupChannelMemberListQuery != NULL) {
		delete m_GroupChannelMemberListQuery;
		m_GroupChannelMemberListQuery = NULL;
	}

	if (!m_CurrGroupChannel) {
		return;
	}

	m_CurrGroupChannel->RefreshChannel(new SendBirdRefreshMemberListQueryHandler(this));
}

class SendBirdExitOpenChannelHandler : public SBDExitOpenChannelInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdExitOpenChannelHandler(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler(SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Channel exit is failed.")));

			return;
		}

		dlg->ExitCurrentChannel();
		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Exited from the open channel.")));
		PostMessage(dlg->GetSafeHwnd(), WM_EXIT_OPENCHANNEL, 0, 0);
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnExitOpenchannel()
{
	if (!m_CurrOpenChannel) {
		return;
	}

	DisplayStatus(L"Trying to exit from the open channel.");
	m_CurrOpenChannel->Exit(new SendBirdExitOpenChannelHandler(this));
}

void CSendBirdExampleDlg::OnLbnDblclkListChatMsg()
{
	int cur_sel_pos = m_ListBoxChatMsg.GetCurSel();
	m_SelectedMessage = m_Messages[cur_sel_pos];
	
	wstring status_msg = L"";
	if (m_SelectedMessage->message_type == SBDMessageTypeUser) {
		SBDUserMessage *user_message = (SBDUserMessage *)m_SelectedMessage;
		status_msg = Utils::FormattedString(L"<%s> is selected.", user_message->message.c_str());
		m_SelectedSender = user_message->GetSender();
		m_IsSenderSelected = true;
	}
	else if (m_SelectedMessage->message_type == SBDMessageTypeFile) {
		SBDFileMessage *file_message = (SBDFileMessage *)m_SelectedMessage;
		status_msg = Utils::FormattedString(L"<%s> is selected.", file_message->url.c_str());
		m_SelectedSender = file_message->GetSender();
		m_IsSenderSelected = true;
	}
	else if (m_SelectedMessage->message_type == SBDMessageTypeFile) {
		SBDAdminMessage *admin_message = (SBDAdminMessage *)m_SelectedMessage;
		status_msg = Utils::FormattedString(L"<%s> is selected.", admin_message->message.c_str());
		m_IsSenderSelected = false;
	}

	DisplayStatus(status_msg);
}

class SendBirdBanUserHandler : public SBDBanUserInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdBanUserHandler(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler(SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"User ban is failed.")));

			return;
		}

		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"The user is banned.")));
		PostMessage(dlg->GetSafeHwnd(), WM_BAN_USER, 0, 0);
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnBanUser()
{
	if (m_CurrOpenChannel) {
		if (m_IsSenderSelected) {
			wstring status_msg = Utils::FormattedString(L"Trying to ban %s for 60 seconds.", m_SelectedSender.nickname.c_str());
			DisplayStatus(status_msg);
			m_CurrOpenChannel->BanUser(m_SelectedSender, 60, new SendBirdBanUserHandler(this));
		}
	}
}

class SendBirdLoadMoreBannedUserListQuery : public SBDLoadNextUserListInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdLoadMoreBannedUserListQuery(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler(vector<SBDUser> users, SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"User ban list loading is failed.")));

			return;
		}

		PostMessage(dlg->GetSafeHwnd(), WM_LOAD_MORE_BAN_USER_LIST, 0, (LPARAM)(new UserListParm(users)));
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnRefreshBanned()
{
	if (!m_CurrOpenChannel) {
		return;
	}

	if (m_BannedUserListQuery != NULL) {
		delete m_BannedUserListQuery;
		m_BannedUserListQuery = NULL;
	}
	m_ListBoxBannedUsers.ResetContent();
	m_BannedUserList.clear();
	m_BannedUserListQuery = m_CurrOpenChannel->CreateBannedUserListQuery();
	m_BannedUserListQuery->limit = 2;
	m_BannedUserListQuery->LoadNextPage(new SendBirdLoadMoreBannedUserListQuery(this));
}

void CSendBirdExampleDlg::OnBnClickedBtnLoadmoreBanned()
{
	if (!m_CurrOpenChannel) {
		return;
	}

	if (m_BannedUserListQuery == NULL) {
		return;
	}

	m_BannedUserListQuery->LoadNextPage(new SendBirdLoadMoreBannedUserListQuery(this));
}

void CSendBirdExampleDlg::OnLbnDblclkListboxBannedUsers()
{
	int cur_sel_pos = m_ListBoxBannedUsers.GetCurSel();
	m_SelectedBannedUser = m_BannedUserList[cur_sel_pos];
	m_IsBannedUserSelected = true;

	wstring status_msg = L"";;
	status_msg = Utils::FormattedString(L"Banned user, <%s> is selected.", m_SelectedBannedUser.nickname.c_str());

	DisplayStatus(status_msg);
}

class SendBirdUnbanUserHandler : public SBDUnbanUserInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdUnbanUserHandler(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler(SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"User unban is failed.")));

			return;
		}

		PostMessage(dlg->GetSafeHwnd(), WM_UNBAN_USER, 0, 0);
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnUnbanUser()
{
	if (m_CurrOpenChannel) {
		if (m_IsBannedUserSelected) {
			m_CurrOpenChannel->UnbanUser(m_SelectedBannedUser, new SendBirdUnbanUserHandler(this));
		}
	}
}

class SendBirdMuteUserHandler : public SBDMuteUserInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdMuteUserHandler(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler(SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"User mute is failed.")));

			return;
		}

		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"The user is muted.")));
		PostMessage(dlg->GetSafeHwnd(), WM_MUTE_USER, 0, 0);
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnMuteUser()
{
	if (m_CurrOpenChannel) {
		if (m_IsSenderSelected) {
			wstring status_msg = Utils::FormattedString(L"Trying to mute %s", m_SelectedSender.nickname.c_str());
			DisplayStatus(status_msg);
			m_CurrOpenChannel->MuteUser(m_SelectedSender, new SendBirdMuteUserHandler(this));
		}
	}
}

void CSendBirdExampleDlg::OnLbnDblclkListboxMutedUsers()
{
	int cur_sel_pos = m_ListBoxMutedUsers.GetCurSel();
	m_SelectedMutedUser = m_MutedUserList[cur_sel_pos];
	m_IsMutedUserSelected = true;

	wstring status_msg = L"";;
	status_msg = Utils::FormattedString(L"Muted user, <%s> is selected.", m_SelectedMutedUser.nickname.c_str());

	DisplayStatus(status_msg);
}

class SendBirdLoadMoreMutedUserListQuery : public SBDLoadNextUserListInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdLoadMoreMutedUserListQuery(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler(vector<SBDUser> users, SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"User mute list loading is failed.")));

			return;
		}

		PostMessage(dlg->GetSafeHwnd(), WM_LOAD_MORE_MUTE_USER_LIST, 0, (LPARAM)(new UserListParm(users)));
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnRefreshMuted()
{
	if (!m_CurrOpenChannel) {
		return;
	}

	if (m_MutedUserListQuery != NULL) {
		delete m_MutedUserListQuery;
		m_MutedUserListQuery = NULL;
	}

	m_ListBoxMutedUsers.ResetContent();
	m_MutedUserList.clear();

	m_MutedUserListQuery = m_CurrOpenChannel->CreateMutedUserListQuery();
	m_MutedUserListQuery->limit = 1;
	m_MutedUserListQuery->LoadNextPage(new SendBirdLoadMoreMutedUserListQuery(this));
}

void CSendBirdExampleDlg::OnBnClickedBtnLoadmoreMuted()
{
	if (m_CurrOpenChannel) {
		return;
	}

	if (m_MutedUserListQuery == NULL) {
		return;
	}

	m_MutedUserListQuery->LoadNextPage(new SendBirdLoadMoreMutedUserListQuery(this));
}

class SendBirdUnmuteUserHandler : public SBDUnmuteUserInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdUnmuteUserHandler(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler(SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"User unmute is failed.")));

			return;
		}

		PostMessage(dlg->GetSafeHwnd(), WM_UNMUTE_USER, 0, 0);
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnUnmuteUser()
{
	if (m_CurrOpenChannel) {
		if (m_IsMutedUserSelected) {
			m_CurrOpenChannel->UnmuteUser(m_SelectedMutedUser, new SendBirdUnmuteUserHandler(this));
		}
	}
}

void CSendBirdExampleDlg::OnBnClickedBtnReconnect()
{
	DisplayStatus(L"Start to reconnect explictly.");
	SBDMain::Reconnect();
}

class SendBirdLoadMoreBlockedUserListQuery : public SBDLoadNextUserListInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdLoadMoreBlockedUserListQuery(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler(vector<SBDUser> users, SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"User block list loading is failed.")));

			return;
		}

		PostMessage(dlg->GetSafeHwnd(), WM_LOAD_MORE_BLOCK_USER_LIST, 0, (LPARAM)(new UserListParm(users)));
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnLoadMoreBlockedList()
{
	if (m_BlockedUserListQuery == NULL) {
		return;
	}

	m_BlockedUserListQuery->LoadNextPage(new SendBirdLoadMoreBlockedUserListQuery(this));
}


void CSendBirdExampleDlg::OnBnClickedBtnRefreshBlockedList()
{
	// TODO: Add your control notification handler code here
	if (m_BlockedUserListQuery != NULL) {
		delete m_BlockedUserListQuery;
		m_BlockedUserListQuery = NULL;
	}

	m_ListBoxBlockedUsers.ResetContent();
	m_BlockedUserList.clear();

	m_BlockedUserListQuery = SBDMain::CreateBlockedUserListQuery();
	m_BlockedUserListQuery->limit = 2;
	m_BlockedUserListQuery->LoadNextPage(new SendBirdLoadMoreBlockedUserListQuery(this));
}

class SendBirdUnblockUserHandler : public SBDUnblockUserInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdUnblockUserHandler(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler(SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"User ublock is failed.")));

			return;
		}

		PostMessage(dlg->GetSafeHwnd(), WM_UNBLOCK_USER, 0, 0);
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnUnblockUser()
{
	// TODO: Add your control notification handler code here
	if (m_IsBlockedUserSelected) {
		SBDMain::UnblockUser(m_SelectedBlockedUser, new SendBirdUnblockUserHandler(this));
	}
}

class SendBirdBlockUserHandler : public SBDBlockUserInterface {
public:
	CSendBirdExampleDlg *dlg;

	SendBirdBlockUserHandler(CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
	}

	void CompletionHandler(SBDUser blocked_user, SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"User block is failed.")));

			return;
		}

		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"The user is blocked.")));
		PostMessage(dlg->GetSafeHwnd(), WM_BLOCK_USER, 0, 0);
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnBlockUser()
{
	if (m_IsSenderSelected != NULL) {
		wstring status_msg = Utils::FormattedString(L"Trying to block %s", m_SelectedSender.nickname.c_str());
		DisplayStatus(status_msg);
		SBDMain::BlockUser(m_SelectedSender, new SendBirdBlockUserHandler(this));
	}
}

class SendBirdLeaveGroupChannelHandler : public SBDLeaveGroupChannelInterface {
public:
	CSendBirdExampleDlg *dlg;
	SBDGroupChannel *group_channel;

	SendBirdLeaveGroupChannelHandler(SBDGroupChannel *group_channel, CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
		this->group_channel = group_channel;
	}

	void CompletionHandler(SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"To leave group channel is failed.")));

			return;
		}

		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Left from the group channel.")));
		PostMessage(dlg->GetSafeHwnd(), WM_LEAVE_GROUP_CHANNEL, 0, (LPARAM)(new GroupChannelParam(this->group_channel)));
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnLeaveGroupchannel()
{
	if (m_CurrGroupChannel) {
		m_CurrGroupChannel->LeaveChannel(new SendBirdLeaveGroupChannelHandler(m_CurrGroupChannel, this));
	}
}

class SendBirdHideGroupChannelHandler : public SBDHideGroupChannelInterface {
public:
	CSendBirdExampleDlg *dlg;
	SBDGroupChannel *group_channel;

	SendBirdHideGroupChannelHandler(SBDGroupChannel *group_channel, CSendBirdExampleDlg *dlg) {
		this->dlg = dlg;
		this->group_channel = group_channel;
	}

	void CompletionHandler(SBDError *error) {
		if (error != NULL) {
			PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"To hide group channel is failed.")));

			return;
		}

		PostMessage(dlg->GetSafeHwnd(), WM_DISPLAY_STATUS_MESSAGE, 0, (LPARAM)(new StatusParam(L"Hid from the group channel.")));
		PostMessage(dlg->GetSafeHwnd(), WM_HIDE_GROUP_CHANNEL, 0, (LPARAM)(new GroupChannelParam(this->group_channel)));
	}
};

void CSendBirdExampleDlg::OnBnClickedBtnHideGroupchannel()
{
	if (m_CurrGroupChannel) {
		m_CurrGroupChannel->HideChannel(false, new SendBirdHideGroupChannelHandler(m_CurrGroupChannel, this));
	}
}

void CSendBirdExampleDlg::DisplayStatus(wstring status) {
	m_StatusListBox.InsertString(-1, status.c_str());
	m_StatusListBox.SetCurSel(m_StatusListBox.GetCount() - 1);
}

void CSendBirdExampleDlg::DisplayMessageAtLastPosition(wstring message) {
	m_ListBoxChatMsg.InsertString(-1, message.c_str());
	m_ListBoxChatMsg.SetCurSel(m_ListBoxChatMsg.GetCount() - 1);
}

void CSendBirdExampleDlg::DisplayMessageAtTopPosition(wstring message) {
	m_ListBoxChatMsg.InsertString(0, message.c_str());
}

void CSendBirdExampleDlg::DisplayGroupChannelAtLastPosition(SBDGroupChannel *channel) {
	wostringstream unread_count_str;
	wostringstream member_count_str;

	unread_count_str << channel->unread_message_count;
	wstring unread_count_w(unread_count_str.str());

	member_count_str << channel->member_count;
	wstring member_count_w(member_count_str.str());

	wstring display_msg = Utils::FormattedString(L"(%s/%s) %s", unread_count_w.c_str(), member_count_w.c_str(), channel->name.c_str());

	m_ListBoxGroupChannels.InsertString(-1, display_msg.c_str());
}

void CSendBirdExampleDlg::DisplayGroupChannelAtTopPosition(SBDGroupChannel *channel) {
	wostringstream unread_count_str;
	wostringstream member_count_str;

	unread_count_str << channel->unread_message_count;
	wstring unread_count_w(unread_count_str.str());

	member_count_str << channel->member_count;
	wstring member_count_w(member_count_str.str());

	wstring display_msg = Utils::FormattedString(L"(%s/%s) %s", unread_count_w.c_str(), member_count_w.c_str(), channel->name.c_str());

	m_ListBoxGroupChannels.InsertString(0, display_msg.c_str());
}

void CSendBirdExampleDlg::DisplayOpenChannelAtTopPosition(SBDOpenChannel *channel) {
	m_ListBoxOpenChannel.InsertString(0, (LPCTSTR)(channel->name.c_str()));
	m_OpenChannelList.insert(m_OpenChannelList.begin(), channel);
}

void CSendBirdExampleDlg::DisplayParticipantAtLastPosition(SBDUser user) {
	wstring display_msg = Utils::FormattedString(L"%s<%s>", user.nickname.c_str(), user.user_id.c_str());
	m_ListBoxParticipants.InsertString(-1, display_msg.c_str());
}

void CSendBirdExampleDlg::DisplayGroupChannelMemberAtLastPosition(SBDUser user) {
	wstring display_msg = Utils::FormattedString(L"%s<%s>", user.nickname.c_str(), user.user_id.c_str());
	m_ListBoxGroupChannelMembers.InsertString(-1, display_msg.c_str());
}

void CSendBirdExampleDlg::DisplayBannedUserAtLastPosition(SBDUser user) {
	wstring display_msg = Utils::FormattedString(L"%s<%s>", user.nickname.c_str(), user.user_id.c_str());
	m_ListBoxBannedUsers.InsertString(-1, display_msg.c_str());
}

void CSendBirdExampleDlg::DisplayMutedUserAtLastPosition(SBDUser user) {
	wstring display_msg = Utils::FormattedString(L"%s<%s>", user.nickname.c_str(), user.user_id.c_str());
	m_ListBoxMutedUsers.InsertString(-1, display_msg.c_str());
}

void CSendBirdExampleDlg::DisplayBlockedUserAtLastPosition(SBDUser user) {
	wstring display_msg = Utils::FormattedString(L"%s<%s>", user.nickname.c_str(), user.user_id.c_str());
	m_ListBoxBlockedUsers.InsertString(-1, display_msg.c_str());
}

void CSendBirdExampleDlg::SetCurrentChannel(SBDBaseChannel *channel, SBDChannelType channel_type) {
	if (channel_type == SBDChannelTypeOpen) {
		m_CurrOpenChannel = (SBDOpenChannel *)channel;
		m_CurrGroupChannel = NULL;

		PostMessage(WM_ENTERED_OR_JOINED_CHANNEL, 0, (LPARAM)(new CurrentChannelParam(Utils::FormattedString(L"O/ %s<%s>", channel->name.c_str(), channel->channel_url.c_str()))));
		m_STextChannelName.SetWindowText(Utils::FormattedString(L"O/ %s<%s>", channel->name.c_str(), channel->channel_url.c_str()).c_str());
	}
	else {
		m_CurrGroupChannel = (SBDGroupChannel *)channel;
		m_CurrOpenChannel = NULL;

		PostMessage(WM_ENTERED_OR_JOINED_CHANNEL, 0, (LPARAM)(new CurrentChannelParam(Utils::FormattedString(L"G/ %s<%s>", channel->name.c_str(), channel->channel_url.c_str()))));
		m_STextChannelName.SetWindowText(Utils::FormattedString(L"G/ %s<%s>", channel->name.c_str(), channel->channel_url.c_str()).c_str());
	}
}

void CSendBirdExampleDlg::ExitCurrentChannel() {
	m_CurrOpenChannel = NULL;
	m_CurrGroupChannel = NULL;
	m_SelectedMessage = NULL;
	PostMessage(WM_ENTERED_OR_JOINED_CHANNEL, 0, (LPARAM)(new CurrentChannelParam(L"")));
}

void CSendBirdExampleDlg::OnEnChangeEditMessage()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (m_CurrGroupChannel != NULL) {
		CString message_text;
		m_EditChatMessage.GetWindowText(message_text);
		if (message_text.GetLength() > 0) {
			m_CurrGroupChannel->StartTyping();
		}
		else {
			m_CurrGroupChannel->EndTyping();
		}
	}
}

void CSendBirdExampleDlg::OnLbnDblclkListboxBlockedList()
{
	int cur_sel_pos = m_ListBoxBlockedUsers.GetCurSel();
	m_SelectedBlockedUser = m_BlockedUserList[cur_sel_pos];
	m_IsBlockedUserSelected = true;

	wstring status_msg = L"";;
	status_msg = Utils::FormattedString(L"Blocked user, <%s> is selected.", m_SelectedBlockedUser.nickname.c_str());

	DisplayStatus(status_msg);
}

LRESULT CSendBirdExampleDlg::UpdateUISendMessage(WPARAM wParam, LPARAM lParam) {
	MessageParam *param = (MessageParam *)lParam;

	if (param->message->message_type == SBDMessageTypeUser) {
		wostringstream created_at;
		SBDUserMessage *user_message = (SBDUserMessage *)param->message;
		created_at << user_message->created_at;
		wstring created_at_w(created_at.str());
		wstring display_msg = Utils::FormattedString(L"%s<%s>: %s", user_message->GetSender().nickname.c_str(), created_at_w.c_str(), user_message->message.c_str());

		m_Messages.push_back(param->message);
		DisplayMessageAtLastPosition(display_msg);
	}
	else if (param->message->message_type == SBDMessageTypeFile) {
		SBDFileMessage *file_message = (SBDFileMessage *)param->message;
		wostringstream created_at;
		created_at << file_message->created_at;
		wstring created_at_w(created_at.str());
		wstring display_msg = Utils::FormattedString(L"%s<%s>: %s", file_message->GetSender().nickname.c_str(), created_at_w.c_str(), file_message->url.c_str());

		m_Messages.insert(m_Messages.begin(), file_message);
		DisplayMessageAtLastPosition(display_msg);
	}
	else if (param->message->message_type == SBDMessageTypeAdmin) {
		SBDAdminMessage *admin_message = (SBDAdminMessage *)param->message;
		wstring display_msg = Utils::FormattedString(L"ADMIN => %s", admin_message->message.c_str());

		m_Messages.insert(m_Messages.begin(), admin_message);
		DisplayMessageAtLastPosition(display_msg);
	}

	delete param;

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUIReceiveMessage(WPARAM wParam, LPARAM lParam) {
	MessageParam *param = (MessageParam *)lParam;

	if (param->message->message_type == SBDMessageTypeUser) {
		wostringstream created_at;
		SBDUserMessage *user_message = (SBDUserMessage *)param->message;
		created_at << user_message->created_at;
		wstring created_at_w(created_at.str());
		wstring display_msg = Utils::FormattedString(L"%s<%s>: %s", user_message->GetSender().nickname.c_str(), created_at_w.c_str(), user_message->message.c_str());

		m_Messages.push_back(param->message);
		DisplayMessageAtLastPosition(display_msg);
	}
	else if (param->message->message_type == SBDMessageTypeFile) {
		SBDFileMessage *file_message = (SBDFileMessage *)param->message;
		wostringstream created_at;
		created_at << file_message->created_at;
		wstring created_at_w(created_at.str());
		wstring display_msg = Utils::FormattedString(L"%s<%s>: %s", file_message->GetSender().nickname.c_str(), created_at_w.c_str(), file_message->url.c_str());

		m_Messages.insert(m_Messages.begin(), file_message);
		DisplayMessageAtLastPosition(display_msg);
	}
	else if (param->message->message_type == SBDMessageTypeAdmin) {
		SBDAdminMessage *admin_message = (SBDAdminMessage *)param->message;
		wstring display_msg = Utils::FormattedString(L"ADMIN: %s", admin_message->message.c_str());

		m_Messages.insert(m_Messages.begin(), admin_message);
		DisplayMessageAtLastPosition(display_msg);
	}

	delete param;

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUIInsertOpenChannelTopPostion(WPARAM wParam, LPARAM lParam) {
	OpenChannelParam *param = (OpenChannelParam *)lParam;
	SBDOpenChannel *channel = param->open_channel;

	DisplayOpenChannelAtTopPosition(channel);

	delete param;

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUILoadPreviousMessages(WPARAM wParam, LPARAM lParam) {
	MessageListParam *param = (MessageListParam *)lParam;
	std::reverse(param->messages.begin(), param->messages.end());
	for (vector<SBDBaseMessage *>::iterator it = param->messages.begin(); it != param->messages.end(); ++it) {
		if ((*it)->message_type == SBDMessageTypeUser) {
			SBDUserMessage *user_message = (SBDUserMessage *)(*it);
			wostringstream created_at;
			created_at << user_message->created_at;
			wstring created_at_w(created_at.str());
			wstring display_msg = Utils::FormattedString(L"%s<%s>: %s", user_message->GetSender().nickname.c_str(), created_at_w.c_str(), user_message->message.c_str());
			m_Messages.insert(m_Messages.begin(), user_message);
			DisplayMessageAtTopPosition(display_msg);
		}
		else if ((*it)->message_type == SBDMessageTypeFile) {
			SBDFileMessage *file_message = (SBDFileMessage *)(*it);
			wostringstream created_at;
			created_at << file_message->created_at;
			wstring created_at_w(created_at.str());
			wstring display_msg = Utils::FormattedString(L"%s<%s>: %s", file_message->GetSender().nickname.c_str(), created_at_w.c_str(), file_message->url.c_str());
			m_Messages.insert(m_Messages.begin(), file_message);
			DisplayMessageAtTopPosition(display_msg);
		}
		else if ((*it)->message_type == SBDMessageTypeAdmin) {
			SBDAdminMessage *admin_message = (SBDAdminMessage *)(*it);
			wstring display_msg = Utils::FormattedString(L"ADMIN: %s", admin_message->message.c_str());
			m_Messages.insert(m_Messages.begin(), admin_message);
			DisplayMessageAtTopPosition(display_msg);
		}

		if ((*it)->created_at < m_MinMessageTimestamp) {
			m_MinMessageTimestamp = (*it)->created_at;
		}
	}

	delete param;

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUIClearAllControls(WPARAM wParam, LPARAM lParam) {
	DisplayStatus(L"Disconnected.");
	delete m_OpenChannelListQuery;
	m_OpenChannelListQuery = NULL;
	m_ParticipantListQuery = NULL;
	m_SelectedMessage = NULL;

	m_OpenChannelList.clear();
	m_GroupChannelList.clear();

	for (size_t i = 0; i < m_Messages.size(); i++) {
		delete m_Messages[i];
	}
	m_Messages.clear();
	m_ListBoxChatMsg.ResetContent();
	m_EditChatMessage.SetWindowText(L"");

	m_ListBoxOpenChannel.ResetContent();
	m_ListOpenChannelOps.ResetContent();
	m_ListGroupChannelInvitees.ResetContent();
	m_EditCreateOpenChannelName.SetWindowText(L"");
	m_EditOpUserID.SetWindowText(L"");

	m_ListBoxGroupChannels.ResetContent();
	m_ListGroupChannelInvitees.ResetContent();
	m_EditBoxNewInviteeId.SetWindowText(L"");
	m_EditNewGroupChannelName.SetWindowText(L"");

	m_STextChannelName.SetWindowText(L"");

	m_BannedUserList.clear();
	m_MutedUserList.clear();

	m_IsSenderSelected = false;
	m_IsParticipantSelected = false;
	m_IsMutedUserSelected = false;
	m_IsBannedUserSelected = false;
	m_IsMemberSelected = false;
	m_IsBlockedUserSelected = false;

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUIRefreshMyGroupChannelList(WPARAM wParam, LPARAM lParam) {
	GroupChannelListParam *param = (GroupChannelListParam *)lParam;

	for (vector<SBDGroupChannel *>::iterator it = param->group_channels.begin(); it != param->group_channels.end(); ++it) {
		DisplayGroupChannelAtLastPosition((*it));
		m_GroupChannelList.push_back((*it));
	}

	delete param;

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUIDisplayStatusMessage(WPARAM wParam, LPARAM lParam) {
	StatusParam *param = (StatusParam *)lParam;

	DisplayStatus(param->status_message);

	delete param;

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUIInsertGroupChannelTopPostion(WPARAM wParam, LPARAM lParam) {
	GroupChannelParam *param = (GroupChannelParam *)lParam;

	SBDGroupChannel *channel = param->group_channel;

	DisplayGroupChannelAtTopPosition(channel);

	m_GroupChannelList.insert(m_GroupChannelList.begin(), channel);

	SetCurrentChannel(channel, SBDChannelTypeGroup);

	delete param;

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUIEnteredOrJoinedChannel(WPARAM wParam, LPARAM lParam) {
	CurrentChannelParam *param = (CurrentChannelParam *)lParam;
	m_STextChannelName.SetWindowText(param->current_channel_info.c_str());
	for (size_t i = 0; i < m_Messages.size(); i++) {
		delete m_Messages[i];
	}
	m_Messages.clear();
	m_ListBoxChatMsg.ResetContent();
	m_MinMessageTimestamp = INT64_MAX;

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUILoadMoreParticipantList(WPARAM wParam, LPARAM lParam) {
	UserListParm *param = (UserListParm *)lParam;

	for (vector<SBDUser>::iterator it = param->users.begin(); it != param->users.end(); ++it) {
		DisplayParticipantAtLastPosition((*it));
	}

	delete param;

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUIRefreshGroupChannelMemberList(WPARAM wParam, LPARAM lParam) {
	m_ListBoxGroupChannelMembers.ResetContent();

	for (vector<SBDMember>::iterator it = m_CurrGroupChannel->members.begin(); it != m_CurrGroupChannel->members.end(); ++it) {
		DisplayGroupChannelMemberAtLastPosition((*it));
	}

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUIExitOpenChannel(WPARAM wParam, LPARAM lParam) {
	DisplayStatus(L"Exited from the open channel.");

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUIBanUser(WPARAM wParam, LPARAM lParam) {
	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUILoadMoreBanUserList(WPARAM wParam, LPARAM lParam) {
	UserListParm *param = (UserListParm *)lParam;

	for (vector<SBDUser>::iterator it = param->users.begin(); it != param->users.end(); ++it) {
		m_BannedUserList.push_back((*it));
		DisplayBannedUserAtLastPosition((*it));
	}

	delete param;

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUIUnbanUser(WPARAM wParam, LPARAM lParam) {
	for (size_t i = 0; i < m_BannedUserList.size(); i++) {
		if (!m_BannedUserList[i].user_id.compare(m_SelectedBannedUser.user_id)) {
			m_BannedUserList.erase(m_BannedUserList.begin() + i);
			m_ListBoxBannedUsers.DeleteString((UINT)i);
			break;
		}
	}

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUIMuteUser(WPARAM wParam, LPARAM lParam) {
	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUILoadMoreMuteUserList(WPARAM wParam, LPARAM lParam) {
	UserListParm *param = (UserListParm *)lParam;

	for (vector<SBDUser>::iterator it = param->users.begin(); it != param->users.end(); ++it) {
		m_MutedUserList.push_back((*it));
		DisplayMutedUserAtLastPosition((*it));
	}

	delete param;

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUIUnmuteUser(WPARAM wParam, LPARAM lParam) {
	for (size_t i = 0; i < m_MutedUserList.size(); i++) {
		if (!m_MutedUserList[i].user_id.compare(m_SelectedMutedUser.user_id)) {
			m_MutedUserList.erase(m_MutedUserList.begin() + i);
			m_ListBoxMutedUsers.DeleteString((UINT)i);
			break;
		}
	}

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUILoadMoreBlockUserList(WPARAM wParam, LPARAM lParam) {
	UserListParm *param = (UserListParm *)lParam;

	for (vector<SBDUser>::iterator it = param->users.begin(); it != param->users.end(); ++it) {
		m_BlockedUserList.push_back((*it));
		DisplayBlockedUserAtLastPosition((*it));
	}

	delete param;

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUIUnblockUser(WPARAM wParam, LPARAM lParam) {
	for (size_t i = 0; i < m_BlockedUserList.size(); i++) {
		if (!m_BlockedUserList[i].user_id.compare(m_SelectedBlockedUser.user_id)) {
			m_BlockedUserList.erase(m_BlockedUserList.begin() + i);
			m_ListBoxBlockedUsers.DeleteString((UINT)i);
			break;
		}
	}

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUIBlockUser(WPARAM wParam, LPARAM lParam) {
	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUILeaveHideGroupChannel(WPARAM wParam, LPARAM lParam) {
	for (size_t i = 0; i < m_GroupChannelList.size(); i++) {
		if (!m_GroupChannelList[i]->channel_url.compare(m_CurrGroupChannel->channel_url)) {
			m_GroupChannelList.erase(m_GroupChannelList.begin() + i);
			m_ListBoxGroupChannels.DeleteString((UINT)i);
			break;
		}
	}

	ExitCurrentChannel();

	return 0;
}

LRESULT CSendBirdExampleDlg::UpdateUITypingStatus(WPARAM wParam, LPARAM lParam) {
	GroupChannelParam *param = (GroupChannelParam *)lParam;

	vector<SBDMember> typing_members = param->group_channel->GetTypingMembers();
	wostringstream typing_members_wostr;

	typing_members_wostr << L"";

	for (size_t i = 0; i < typing_members.size(); i++) {
		SBDMember member = param->group_channel->GetTypingMembers()[i];
		typing_members_wostr << member.nickname << L" ";
	}

	wstring typing_members_str(typing_members_wostr.str());
	m_STextTypingMembers.SetWindowText(typing_members_str.c_str());

	delete param;

	return 0;
}

