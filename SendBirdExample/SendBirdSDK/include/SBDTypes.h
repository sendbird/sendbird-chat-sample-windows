//
//  SBDTypes.h
//  SendBird
//
//  Created by Jed Gyeong on 6/15/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDTYPES_H_
#define SENDBIRD_SBDTYPES_H_

#define SBD_CHANNEL_TYPE_OPEN L"open"
#define SBD_CHANNEL_TYPE_GROUP L"group"

/**
 *  The order type for `SBDGroupChannelListQuery`.
 */
typedef enum {
    SBDGroupChannelListOrderChronological = 0,
    SBDGroupChannelListOrderLatestLastMessage = 1,
} SBDGroupChannelListOrder;

/**
 *  Error types.
 */
typedef enum {
    SBDErrorNone = 0,
    
    // RESTful API Errors
    SBDErrorInvalidParameterValueString = 400100,
    SBDErrorInvalidParameterValueNumber = 400101,
    SBDErrorInvalidParameterValueList = 400102,
    SBDErrorInvalidParameterValueJson = 400103,
    SBDErrorInvalidParameterValueBoolean = 400104,
    SBDErrorInvalidParameterValueRequired = 400105,
    SBDErrorInvalidParameterValuePositive = 400106,
    SBDErrorInvalidParameterValueNegative = 400107,
    SBDErrorNonAuthorized = 400108,
    SBDErrorTokenExpired = 400109,
    SBDErrorInvalidChannelUrl = 400110,
    SBDErrorInvalidParameterValue = 400111,
    SBDErrorUnusableCharacterIncluded = 400151,
    SBDErrorNotFoundInDatabase = 400201,
    SBDErrorDuplicatedData = 400202,
	SBDErrorAccessTokenNotValid = 400302,
    SBDErrorInvalidApiToken = 400401,
    SBDErrorParameterMissing = 400402,
    SBDErrorInvalidJsonBody = 400403,
    
    // RESTful API Errors for SDK
    SBDErrorAppIdNotValid = 400404,
    //SBDErrorAccessTokenEmpty = 400500,
    //SBDErrorAccessTokenNotValid = 400501,
    SBDErrorUserNotExist = 400502,
    SBDErrorUserDeactivated = 400503,
    SBDErrorUserCreationFailed = 400504,
    
    SBDErrorInternalServerError = 500901,
    
    // SDK Internal Errors
    SBDErrorInvalidInitialization = 800100,
    SBDErrorConnectionRequired = 800101,
    SBDErrorInvalidParameter = 800110,
    SBDErrorNetworkError = 800120,
    SBDErrorNetworkRoutingError = 800121,
    SBDErrorMalformedData = 800130,
    SBDErrorMalformedErrorData = 800140,
    SBDErrorWrongChannelType = 800150,
    SBDErrorMarkAsReadRateLimitExceeded = 800160,
    SBDErrorQueryInProgress = 800170,
    SBDErrorAckTimeout = 800180,
    SBDErrorLoginTimeout = 800190,
    SBDErrorWebSocketConnectionClosed = 800200,
    SBDErrorWebSocketConnectionFailed = 800210,
    SBDErrorRequestFailed = 800220,
    SBDErrorFileUploadCancelFailed = 800230,
    SBDErrorFileUploadCancelled = 800240,
} SBDErrorCode;

/**
 *  Connection state
 */
typedef enum {
    /**
     *  Connecting
     */
    SBDWebSocketConnecting = 0,
    /**
     *  Open
     */
    SBDWebSocketOpen = 1,
    /**
     *  Closing
     */
    SBDWebSocketClosing = 2,
    /**
     *  Closed
     */
    SBDWebSocketClosed = 3,
} SBDWebSocketConnectionState;

/**
 *  User connection statuses for `SBDUser`.
 */
typedef enum {
    SBDUserConnectionStatusNotAvailable = 0,
    SBDUserConnectionStatusOnline = 1,
    SBDUserConnectionStatusOffline = 2,
} SBDUserConnectionStatus;

/**
 *  Channel types.
 */
typedef enum {
    /**
     *  Open channel.
     */
    SBDChannelTypeOpen = 0,
    /**
     *  Group channel.
     */
    SBDChannelTypeGroup = 1,
} SBDChannelType;

/**
 *  Push token registration statuses
 */
typedef enum {
    /**
     *  Registration succeeded.
     */
    SBDPushTokenRegistrationStatusSuccess = 0,
    /**
     *  Registration is pending.
     */
    SBDPushTokenRegistrationStatusPending = 1,
    /**
     *  Registartion is failed.
     */
    SBDPushTokenRegistrationStatusError = 2,
} SBDPushTokenRegistrationStatus;

/**
 *  The query type for `SBDGroupChannelListQuery`.
 */
typedef enum {
    SBDGroupChannelListQueryTypeAnd = 0,
    SBDGroupChannelListQueryTypeOr = 1,
} SBDGroupChannelListQueryType;


/**
 *  Message type for filtering
 *
 *  - SBDMessageTypeFilterAll: All.
 *  - SBDMessageTypeFilterUser: User message.
 *  - SBDMessageTypeFilterFile: File message.
 *  - SBDMessageTypeFilterAdmin: Admin message.
 */
typedef enum {
    SBDMessageTypeFilterAll = 0,
    SBDMessageTypeFilterUser = 1,
    SBDMessageTypeFilterFile = 2,
    SBDMessageTypeFilterAdmin = 3,
} SBDMessageTypeFilter;

/**
 *  Member state filter for group channel list query and group channel count
 *  - SBDMemberStateFilterAll: All.
 *  - SBDMemberStateFilterJoinedOnly: Joined state only.
 *  - SBDMemberStateFilterInvitedOnly: Invited state only.
 */
typedef enum {
    SBDMemberStateFilterAll = 0,
    SBDMemberStateFilterJoinedOnly = 1,
    SBDMemberStateFilterInvitedOnly = 2,
} SBDMemberStateFilter;

/**
 *  Member state in group channel.
 *  - SBDMemberStateJoined: Joined member in a group channel.
 *  - SBDMemberStateInvited: Invited member in a group channel.
 */
typedef enum {
    SBDMemberStateJoined = 0,
    SBDMemberStateInvited = 1,
} SBDMemberState;

/**
 *  Message type.
 */
typedef enum {
    SBDMessageTypeUser = 0,
    SBDMessageTypeFile = 1,
    SBDMessageTypeAdmin = 2,
} SBDMessageType;

/**
*	Mention type.
*/
typedef enum {
	SBDMentionTypeChannel = 0,
	SBDMentionTypeUsers = 1,
} SBDMentionType;
#endif /* SENDBIRD_SBDTYPES_H_ */
