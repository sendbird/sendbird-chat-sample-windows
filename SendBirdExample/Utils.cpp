#include "stdafx.h"

#include "Utils.h"
#include <atlstr.h>
#include <memory>
//#include <vector>
//#include <numeric>
//#include <locale>
//#include <codecvt>
//#include <limits.h>
#include <cstring>
//#include <typeinfo>
//#include <stdint.h>
//#include <Windows.h>
//#include <WinInet.h>


string Utils::FormattedString(string fmt_str, ...) {
	int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
	std::unique_ptr<char[]> formatted;
	va_list ap;
	while (1) {
		formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
		strcpy(&formatted[0], fmt_str.c_str());
		va_start(ap, fmt_str);
		final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}

	return formatted.get();
}

wstring Utils::FormattedString(wstring fmt_str, ...) {
	int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
	std::unique_ptr<wchar_t[]> formatted;
	va_list ap;
	while (1) {
		formatted.reset(new wchar_t[n]); /* Wrap the plain char array into the unique_ptr */
		wcscpy(&formatted[0], fmt_str.c_str());
		va_start(ap, fmt_str);
		final_n = _vsnwprintf(&formatted[0], n, fmt_str.c_str(), ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}

	return formatted.get();
}

wstring Utils::ConvertUTF8ToUnicode(string str_utf8) {
	wstring str_unicode = CA2W(str_utf8.c_str(), CP_UTF8);

	return str_unicode;
}

string Utils::ConvertUnicodeToUTF8(wstring str_uni) {
	string str_utf8 = CW2A(str_uni.c_str(), CP_UTF8);

	return str_utf8;
}

string Utils::ConvertUnicodeToMultiByte(wstring str_uni) {
	string str_multi = CW2A(str_uni.c_str());

	return str_multi;
}

wstring Utils::ConvertMultiByteToUnicode(string str_multi) {
	wstring str_unicode = CA2W(str_multi.c_str());

	return str_unicode;
}