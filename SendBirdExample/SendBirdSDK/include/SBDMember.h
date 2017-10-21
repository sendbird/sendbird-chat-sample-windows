//
//  SBDMember.h
//  SendBird
//
//  Created by Jed Gyeong on 8/21/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDMEMBER_H_
#define SENDBIRD_SBDMEMBER_H_

#include <iostream>

#include "SBDUser.h"
#include "SBDTypes.h"

/**
 *  The `SBDMember` class represents a member of a group channel. This class has a property to show the state for invitation.
 */
class SBDMember : public SBDUser {
public:
    /**
     *  The state for invitation. The values of the property are `invited` and `joined`. The `invited` means that the user doesn't accept the invitation yet and the `joined` means that the user accepted the invitation manually or automatically.
     */
    SBDMemberState state;
    
    /**
     *  If the member is blocked by the current logged-in user, then true.
     */
    bool is_blocked_by_me;
    
    /**
     *  If the member is blocking the current logged-in user, then true.
     */
    bool is_blocking_me;

	/**
	 *  Internal use only.
	 */
	SBDMember();

	/**
	 *  Internal use only.
	 */
	SBDMember(string dict);
};

#endif /* SENDBIRD_SBDMEMBER_H_ */
