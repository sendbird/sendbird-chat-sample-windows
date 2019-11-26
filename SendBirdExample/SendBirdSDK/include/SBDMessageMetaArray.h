//
//  SBDMessageMetaArray.h
//  SendBird
//
//  Created by Miyoung Han on 10/25/19.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDMESSAGEMETAARRAY_H_
#define SENDBIRD_SBDMESSAGEMETAARRAY_H_

#include <iostream>
#include <vector>

using namespace std;

class SBDMessageMetaArray
{
private:
	wstring key;
	vector<wstring> value;

public:
	SBDMessageMetaArray() {};
	~SBDMessageMetaArray() {};

	SBDMessageMetaArray(wstring key);
	SBDMessageMetaArray(wstring key, const vector<wstring>& value);

	wstring GetKey() const;
	vector<wstring> GetValue() const;
	void SetKey(wstring key);
	void SetValue(vector<wstring> value);
	void AddValue(wstring value);
	void AddValue(vector<wstring> value);
	void RemoveValue(wstring value);
	void RemoveValue(vector<wstring> value);
};
#endif /* SENDBIRD_SBDMESSAGEMETAARRAY_H_ */
