/*********************************************************************
 *  StringManager.cpp
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 *********************************************************************/

#include "StringManager.h"
#include "Core/Platform/Platform.h"
#include <clocale>
#include <iomanip>

wchar_t const* const WCharDigitTables[] =
{
	L"0123456789",
	L"\u0660\u0661\u0662\u0663\u0664\u0665\u0666\u0667\u0668\u0669",
};

#if PLATFORM_WINDOW

wstring StringUtil::StringToWString(const string& InString)
{
	int bufferlen = MultiByteToWideChar(CP_UTF8, 0, InString.c_str(), -1, NULL, 0);
	wchar_t* buffer = new wchar_t[bufferlen];
	MultiByteToWideChar(CP_UTF8, 0, InString.c_str(), -1, buffer, bufferlen);
	wstring wstr(buffer);
	delete[] buffer;
	return wstr;
}

string StringUtil::WStringToString(const wstring& InString)
{
	int bufferlen = WideCharToMultiByte(CP_UTF8, 0, InString.c_str(), -1, NULL, 0, NULL, NULL);
	char* buffer = new char[bufferlen];
	WideCharToMultiByte(CP_UTF8, 0, InString.c_str(), -1, buffer, bufferlen, NULL, NULL);
	string str(buffer);
	delete[] buffer;
	return str;
}

wstring StringUtil::AnsiToWString(const string& InString)
{
	int bufferlen = MultiByteToWideChar(CP_ACP, 0, InString.c_str(), -1, NULL, 0);
	wchar_t* buffer = new wchar_t[bufferlen];
	MultiByteToWideChar(CP_ACP, 0, InString.c_str(), -1, buffer, bufferlen);
	wstring wstr(buffer);
	delete[] buffer;
	return wstr;
}

string StringUtil::WStringToAnsi(const wstring& InString)
{
	int bufferlen = WideCharToMultiByte(CP_ACP, 0, InString.c_str(), -1, NULL, 0, NULL, NULL);
	char* buffer = new char[bufferlen];
	WideCharToMultiByte(CP_ACP, 0, InString.c_str(), -1, buffer, bufferlen, NULL, NULL);
	string str(buffer);
	delete[] buffer;
	return str;
}

#else

wstring StringUtil::StringToWString(const string& InString)
{
	return _wstr_null;
}

string StringUtil::WStringToString(const wstring& InString)
{
	return _str_null;
}

wstring StringUtil::AnsiToWString(const string& InString)
{
	return _wstr_null;
}

string StringUtil::WStringToAnsi(const wstring& InString)
{
	return _str_null;
}

#endif

vec_string StringUtil::GetBetween(const string& InString, const string& InSpecifier)
{
	vec_string temp;
	string::size_type found1 = 0, found2;

	while (true)
	{
		found1 = InString.find(InSpecifier, found1);
		if (found1 != string::npos)
		{
			found2 = InString.find(InSpecifier, found1 + InSpecifier.size());
			if (found2 != string::npos)
			{
				temp.push_back(InString.substr(found1 + InSpecifier.size(), found2 - found1 - InSpecifier.size()));
				found1 = found2 + 1;
			}
			else break;
		}
		else break;
	}

	return temp;
}

vec_string StringUtil::GetBetween(const string& InString, const string& InSpecLeft, const string& InSpecRight)
{
	vec_string temp;
	string::size_type found1 = 0, found2;

	while (true)
	{
		found1 = InString.find(InSpecLeft, found1);
		if (found1 != string::npos)
		{
			found2 = InString.find(InSpecRight, found1 + InSpecLeft.size());
			if (found2 != string::npos)
			{
				temp.push_back(InString.substr(found1 + InSpecLeft.size(), found2 - found1 - InSpecLeft.size()));
				found1 = found2 + 1;
			}
			else break;
		}
		else break;
	}

	return temp;
}

vec_wstring StringUtil::WGetBetween(const wstring& InString, const wstring& InSpecifier)
{
	vec_wstring temp;
	wstring::size_type found1 = 0, found2;

	while (true)
	{
		found1 = InString.find(InSpecifier, found1);
		if (found1 != wstring::npos)
		{
			found2 = InString.find(InSpecifier, found1 + InSpecifier.size());
			if (found2 != wstring::npos)
			{
				temp.push_back(InString.substr(found1 + InSpecifier.size(), found2 - found1 - InSpecifier.size()));
				found1 = found2 + 1;
			}
			else break;
		}
		else break;
	}

	return temp;
}

vec_wstring StringUtil::WGetBetween(const wstring& InString, const wstring& InSpecLeft, const wstring& InSpecRight)
{
	vec_wstring temp;
	wstring::size_type found1 = 0, found2;

	while (true)
	{
		found1 = InString.find(InSpecLeft, found1);
		if (found1 != wstring::npos)
		{
			found2 = InString.find(InSpecRight, found1 + InSpecLeft.size());
			if (found2 != wstring::npos)
			{
				temp.push_back(InString.substr(found1 + InSpecLeft.size(), found2 - found1 - InSpecLeft.size()));
				found1 = found2 + 1;
			}
			else break;
		}
		else break;
	}

	return temp;
}

string StringUtil::GetFirstBetween(const string& InString, const string& InSpecifier)
{
	string::size_type found1, found2;

	found1 = InString.find(InSpecifier, _offset_0);
	if (found1 != string::npos)
	{
		found2 = InString.find(InSpecifier, found1 + InSpecifier.size());
		if (found2 != string::npos)
		{
			return InString.substr(found1 + InSpecifier.size(), found2 - found1 - InSpecifier.size());
		}
	}

	return _str_null;
}

wstring StringUtil::WGetFirstBetween(const wstring& InString, const wstring& InSpecifier)
{
	wstring::size_type found1, found2;

	found1 = InString.find(InSpecifier, _offset_0);
	if (found1 != wstring::npos)
	{
		found2 = InString.find(InSpecifier, found1 + InSpecifier.size());
		if (found2 != wstring::npos)
		{
			return InString.substr(found1 + InSpecifier.size(), found2 - found1 - InSpecifier.size());
		}
	}

	return _wstr_null;
}

vec_string StringUtil::RemoveBetween(string& InString, const string& InSpecifier)
{
	vec_string temp;
	string::size_type found1 = 0, found2;

	while (true)
	{
		found1 = InString.find(InSpecifier);
		if (found1 != string::npos)
		{
			found2 = InString.find(InSpecifier, found1 + InSpecifier.size());
			if (found2 != string::npos)
			{
				temp.push_back(InString.substr(found1 + InSpecifier.size(), found2 - found1 - InSpecifier.size()));
				InString.erase(found1, found2 - found1 + InSpecifier.size());
			}
			else break;
		}
		else break;
	}

	return temp;
}

vec_wstring StringUtil::WRemoveBetween(wstring& InString, const wstring& InSpecifier)
{
	vec_wstring temp;
	wstring::size_type found1 = 0, found2;

	while (true)
	{
		found1 = InString.find(InSpecifier);
		if (found1 != wstring::npos)
		{
			found2 = InString.find(InSpecifier, found1 + InSpecifier.size());
			if (found2 != wstring::npos)
			{
				temp.push_back(InString.substr(found1 + InSpecifier.size(), found2 - found1 - InSpecifier.size()));
				InString.erase(found1, found2 - found1 + InSpecifier.size());
			}
			else break;
		}
		else break;
	}

	return temp;
}

vec_string StringUtil::ReplaceBetween(string& InString, const string& InSpecifier, const string& InReplace)
{
	vec_string temp;
	string::size_type found1 = 0, found2;

	while (true)
	{
		found1 = InString.find(InSpecifier, found1);
		if (found1 != string::npos)
		{
			found2 = InString.find(InSpecifier, found1 + InSpecifier.size());
			if (found2 != string::npos)
			{
				temp.push_back(InString.substr(found1 + InSpecifier.size(), found2 - found1 - InSpecifier.size()));
				InString.replace(found1, found2 - found1 + InSpecifier.size(), InReplace);
				found1 += InReplace.size();
			}
			else break;
		}
		else break;
	}

	return temp;
}

vec_wstring StringUtil::WReplaceBetween(wstring& InString, const wstring& InSpecifier, const wstring& InReplace)
{
	vec_wstring temp;
	wstring::size_type found1 = 0, found2;

	while (true)
	{
		found1 = InString.find(InSpecifier, found1);
		if (found1 != wstring::npos)
		{
			found2 = InString.find(InSpecifier, found1 + InSpecifier.size());
			if (found2 != wstring::npos)
			{
				temp.push_back(InString.substr(found1 + InSpecifier.size(), found2 - found1 - InSpecifier.size()));
				InString.replace(found1, found2 - found1 + InSpecifier.size(), InReplace);
				found1 += InReplace.size();
			}
			else break;
		}
		else break;
	}

	return temp;
}

void StringUtil::EraseAll(string& InString, const string& InErase)
{
	string::size_type found;
	found = InString.find(InErase);
	while (found != string::npos)
	{
		InString.erase(found, InErase.size());
		found = InString.find(InErase, found);
	}
}

void StringUtil::WEraseAll(wstring& InString, const wstring& InErase)
{
	wstring::size_type found;
	found = InString.find(InErase);
	while (found != wstring::npos)
	{
		InString.erase(found, InErase.size());
		found = InString.find(InErase, found);
	}
}

string StringUtil::ToLowerCase(const string& InString)
{
	string lower = InString;
	std::transform(lower.begin(), lower.end(), lower.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return lower;
}

uint32 StringUtil::StrHexToNumeric(const string& InHexStr)
{
	std::stringstream strs;
	strs << InHexStr;
	uint32 value;
	strs >> std::hex >> value;
	return value;
}

string StringUtil::UUIDToString(const uint8* InUUID)
{
	std::stringstream strs;
	for (uint32 j = 0; j < _uuid_size; ++j)
	{
		strs << std::setw(2) << (uint32)InUUID[j];
		if (j == 3 || j == 5 || j == 7 || j == 9)
		{
			strs << '-';
		}
	}
	return strs.str();
}

string StringUtil::WStringToStringV2(const wstring& InString)
{
	std::setlocale(LC_ALL, "");
	const std::locale locale("");
	typedef std::codecvt<wchar_t, char, std::mbstate_t> converter_type;
	const converter_type& converter = std::use_facet<converter_type>(locale);
	std::vector<char> to(InString.length() * converter.max_length());
	std::mbstate_t state;
	const wchar_t* from_next;
	char* to_next;
	const converter_type::result result = converter.out(state, InString.data(), InString.data() + InString.length(), from_next, &to[0], &to[0] + to.size(), to_next);
	if (result == converter_type::ok || result == converter_type::noconv)
		return string(&to[0], to_next);
	else return _str_null;
}

int32 StringUtil::WCharToInt32(wchar_t InChar)
{
	int32 result = -1;

	for (wchar_t const* const* p = std::begin(WCharDigitTables);
		p != std::end(WCharDigitTables) && result == -1;
		++p)
	{
		wchar_t const* q = std::find(*p, *p + 10, InChar);
		if (q != *p + 10)
		{
			result = int32(q - *p);
		}
	}
	return result;
}

bool StringUtil::ExtractFilePath(const string& InPath, string* OutName, string* OutExt, string* OutDir)
{
	string::size_type found1, found2;

	found1 = InPath.find_last_of("/\\");

	if (found1 != string::npos)
	{
		if (OutDir != nullptr) *OutDir = InPath.substr(0, found1);

		string file = InPath.substr(found1 + 1);

		found2 = file.find_last_of(".");

		if (found2 != string::npos)
		{
			if (OutName != nullptr) *OutName = file.substr(0, found2);
			if (OutExt != nullptr) *OutExt = file.substr(found2 + 1);

			return true;
		}
		return false;
	}
	return false;
}

bool StringUtil::WExtractFilePath(const wstring& InPath, wstring* OutName, wstring* OutExt, wstring* OutDir)
{
	wstring::size_type found1, found2;

	found1 = InPath.find_last_of(L"/\\");

	if (found1 != wstring::npos)
	{
		if (OutDir != nullptr) *OutDir = InPath.substr(0, found1);

		wstring file = InPath.substr(found1 + 1);

		found2 = file.find_last_of(L".");

		if (found2 != wstring::npos)
		{
			if (OutName != nullptr) *OutName = file.substr(0, found2);
			if (OutExt != nullptr) *OutExt = file.substr(found2 + 1);

			return true;
		}
		return false;
	}
	return false;
}
