#pragma once

#include <iostream>

using namespace std;

class Utils {
public:
	static string FormattedString(string fmt_str, ...);
	static wstring FormattedString(wstring fmt_str, ...);

	static wstring ConvertUTF8ToUnicode(string str_utf8);
	static string ConvertUnicodeToUTF8(wstring str_uni);
	static string ConvertUnicodeToMultiByte(wstring str_uni);
	static wstring ConvertMultiByteToUnicode(string str_multi);
};