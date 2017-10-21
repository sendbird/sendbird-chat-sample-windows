//
//  SBDInternalTypes.h
//  SendBird
//
//  Created by Jed Gyeong on 6/28/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDINTERNALTYPES_H_
#define SENDBIRD_SBDINTERNALTYPES_H_

typedef enum {
    SBDChannelMetaCountersUpdateModeSet = 0,
    SBDChannelMetaCountersUpdateModeIncrease = 1,
    SBDChannelMetaCountersUpdateModeDecrease = 2,
} SBDChannelMetaCountersUpdateMode;

typedef enum {
    SBDChannelEventCategoryNone = 0,
    
    SBDChannelEventCategoryChannelEnter = 10102,
    SBDChannelEventCategoryChannelExit = 10103,
    
    SBDChannelEventCategoryChannelMute = 10201,
    SBDChannelEventCategoryChannelUnmute = 10200,
    
    SBDChannelEventCategoryChannelBan = 10601,
    SBDChannelEventCategoryChannelUnban = 10600,
    
    SBDChannelEventCategoryChannelFrozen = 10701,
    SBDChannelEventCategoryChannelUnfrozen = 10700,
    
    SBDChannelEventCategoryTypingStart = 10900,
    SBDChannelEventCategoryTypingEnd = 10901,
    
    SBDChannelEventCategoryChannelJoin = 10000,
    SBDChannelEventCategoryChannelLeave = 10001,

    SBDChannelEventCategoryChannelInvite = 10020,
//    SBDChannelEventCategoryChannelAcceptInvite = 10021,
    SBDChannelEventCategoryChannelDeclineInvite = 10022,
    
    SBDChannelEventCategoryChannelPropChanged = 11000,
    SBDChannelEventCategoryChannelDeleted = 12000,
    
    SBDChannelEventCategoryMetaDataChanged = 11100,
    SBDChannelEventCategoryMetaCounterChanged = 11200,
} SBDChannelEventCategory;

typedef enum {
	SBDUserEventCategoryNone = 0,
    SBDUserEventCategoryUserUnblock = 20000,
    SBDUserEventCategoryUserBlock = 20001,
} SBDUserEventCategory;

typedef enum {
    SBDUserListQueryTypeAllUser = 1,
    SBDUserListQueryTypeBlockedUsers = 2,
    SBDUserListQueryTypeOpenChannelParticipants = 3,
    SBDUserListQueryTypeOpenChannelMutedUsers = 4,
    SBDUserListQueryTypeOpenChannelBannedUsers = 5,
    SBDUserListQueryTypeFilteredUsers = 6,
} SBDUserListQueryType;

typedef enum {
    SBDLogLevelNone = 0,
    SBDLogLevelError = 1,
    SBDLogLevelWarning = 2,
    SBDLogLevelInfo = 3,
    SBDLogLevelDebug = 4
} SBDLogLevel;

#endif /* SENDBIRD_SBDINTERNALTYPES_H_ */
