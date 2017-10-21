
// SendBirdExampleDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <SendBird.h>


// CSendBirdExampleDlg dialog
class CSendBirdExampleDlg : public CDialogEx
{
// Construction
public:
	CSendBirdExampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENDBIRDEXAMPLE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnConnect();
private:
	CString m_Nickname;
	CString m_UserId;
public:
	vector<SBDBaseMessage *> m_Messages;
	SBDBaseMessage *m_SelectedMessage;

	vector<SBDOpenChannel *> m_OpenChannelList;
	SBDOpenChannel *m_CurrOpenChannel;

	vector<SBDGroupChannel *> m_GroupChannelList;
	SBDGroupChannel *m_CurrGroupChannel;

	vector<SBDUser> m_BannedUserList;
	vector<SBDUser> m_MutedUserList;
	vector<SBDUser> m_BlockedUserList;

	SBDUser m_SelectedSender;
	bool m_IsSenderSelected;
	SBDUser m_SelectedParticipant;
	bool m_IsParticipantSelected;
	SBDUser m_SelectedMutedUser;
	bool m_IsMutedUserSelected;
	SBDUser m_SelectedBannedUser;
	bool m_IsBannedUserSelected;
	SBDUser m_SelectedMember;
	bool m_IsMemberSelected;
	SBDUser m_SelectedBlockedUser;
	bool m_IsBlockedUserSelected;

	int64_t m_MinMessageTimestamp;

	CString m_ChatMessageText;
	SBDOpenChannelListQuery *m_OpenChannelListQuery;
	SBDGroupChannelListQuery *m_MyGroupChannelListQuery;
	SBDUserListQuery *m_ParticipantListQuery;
	SBDUserListQuery *m_GroupChannelMemberListQuery;
	SBDUserListQuery *m_BannedUserListQuery;
	SBDUserListQuery *m_MutedUserListQuery;
	SBDUserListQuery *m_BlockedUserListQuery;

	CEdit m_UserIdEdit;
	CEdit m_NicknameEdit;
	CEdit m_EditChatMessage;
	CEdit m_EditCreateOpenChannelName;
	CEdit m_EditOpUserID;
	CEdit m_EditBoxNewInviteeId;
	CEdit m_EditNewGroupChannelName;

	CListBox m_ListBoxChatMsg;
	CListBox m_ListBoxOpenChannel;
	CListBox m_StatusListBox;
	CListBox m_ListOpenChannelOps;
	CListBox m_ListGroupChannelInvitees;
	CListBox m_ListBoxGroupChannels;
	CListBox m_ListBoxParticipants;
	CListBox m_ListBoxGroupChannelMembers;
	CListBox m_ListBoxBannedUsers;
	CListBox m_ListBoxMutedUsers;
	CListBox m_ListBoxBlockedUsers;

	CStatic m_STextChannelName;
	CStatic m_STextTypingMembers;

	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnRefreshOpenchannel();
	afx_msg void OnBnClickedBtnLoadmoreOpenchannel();
	afx_msg void OnBnClickedBtnCreateNewOpenchannel();
	afx_msg void OnLbnDblclkListOpenchannel();
	afx_msg void OnBnClickedBtnAddOps();
	afx_msg void OnBnClickedBtnLoadprevmsg();
	afx_msg void OnBnClickedBtnDisconnect();
	afx_msg void OnBnClickedBtnRefreshGroupchannel();
	afx_msg void OnBnClickedBtnAddInvitees();
	afx_msg void OnBnClickedBtnCreateNewGroupchannel();
	afx_msg void OnLbnDblclkListboxGroupchannel();
	afx_msg void OnBnClickedBtnRefreshParticipants();
	afx_msg void OnBnClickedBtnLoadmoreParticipants();
	afx_msg void OnBnClickedBtnRefreshMembers();
	afx_msg void OnBnClickedBtnExitOpenchannel();
	afx_msg void OnLbnDblclkListChatMsg();
	afx_msg void OnBnClickedBtnBanUser();
	afx_msg void OnBnClickedBtnRefreshBanned();
	afx_msg void OnBnClickedBtnLoadmoreBanned();
	afx_msg void OnLbnDblclkListboxBannedUsers();
	afx_msg void OnBnClickedBtnUnbanUser();
	afx_msg void OnBnClickedBtnMuteUser();
	afx_msg void OnLbnDblclkListboxMutedUsers();
	afx_msg void OnBnClickedBtnRefreshMuted();
	afx_msg void OnBnClickedBtnLoadmoreMuted();
	afx_msg void OnBnClickedBtnUnmuteUser();
	afx_msg void OnBnClickedBtnReconnect();
	afx_msg void OnBnClickedBtnLoadMoreBlockedList();
	afx_msg void OnBnClickedBtnRefreshBlockedList();
	afx_msg void OnBnClickedBtnUnblockUser();
	afx_msg void OnBnClickedBtnBlockUser();
	afx_msg void OnBnClickedBtnLeaveGroupchannel();
	afx_msg void OnBnClickedBtnHideGroupchannel();
	afx_msg void OnEnChangeEditMessage();
	afx_msg void OnLbnDblclkListboxBlockedList();

	void DisplayStatus(wstring status);
	void DisplayMessageAtLastPosition(wstring message);
	void DisplayMessageAtTopPosition(wstring message);
	void DisplayGroupChannelAtLastPosition(SBDGroupChannel *channel);
	void DisplayGroupChannelAtTopPosition(SBDGroupChannel *channel);
	void DisplayOpenChannelAtTopPosition(SBDOpenChannel *channel);
	void DisplayParticipantAtLastPosition(SBDUser user);
	void DisplayGroupChannelMemberAtLastPosition(SBDUser user);
	void DisplayBannedUserAtLastPosition(SBDUser user);
	void DisplayMutedUserAtLastPosition(SBDUser user);
	void DisplayBlockedUserAtLastPosition(SBDUser user);

	void SetCurrentChannel(SBDBaseChannel *channel, SBDChannelType channel_type);
	void ExitCurrentChannel();

	LRESULT UpdateUISendMessage(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUIReceiveMessage(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUIInsertOpenChannelTopPostion(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUILoadPreviousMessages(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUIClearAllControls(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUIRefreshMyGroupChannelList(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUIDisplayStatusMessage(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUIInsertGroupChannelTopPostion(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUIEnteredOrJoinedChannel(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUILoadMoreParticipantList(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUIRefreshGroupChannelMemberList(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUIExitOpenChannel(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUIBanUser(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUILoadMoreBanUserList(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUIUnbanUser(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUIMuteUser(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUILoadMoreMuteUserList(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUIUnmuteUser(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUILoadMoreBlockUserList(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUIUnblockUser(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUIBlockUser(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUILeaveHideGroupChannel(WPARAM wParam, LPARAM lParam);
	LRESULT UpdateUITypingStatus(WPARAM wParam, LPARAM lParam);
};
