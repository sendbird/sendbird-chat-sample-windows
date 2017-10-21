//
//  SBDUtils.h
//  SendBird
//
//  Created by Jed Gyeong on 7/20/17.
//  Copyright © 2017 SendBird. All rights reserved.
//

#ifndef SENDBIRD_SBDUTILS_H_
#define SENDBIRD_SBDUTILS_H_

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <stdarg.h>  

using namespace std;

class SBDUUID {
public:
    static string UUID();
};

class SBDUtils {
public:
    static wstring UrlEncode(wstring org);
    static wstring FormattedString(wstring fmt_str, ...);
	static string FormattedString(string fmt_str, ...);
	static wstring ConvertToWString(const std::string& input);
	static wstring ConvertUTF8ToUnicode(string str_utf8);
	static string ConvertUnicodeToUTF8(wstring str_uni);
	static string ConvertUnicodeToMultiByte(wstring str_uni);
	static wstring ConvertMultiByteToUnicode(string str_multi);
};

#endif /* SENDBIRD_SBDUTILS_H_ */
