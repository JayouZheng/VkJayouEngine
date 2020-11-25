//
// StringManager.cpp
//

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

wstring StringUtil::StringToWString(const string& InStringToConvert)
{
	int bufferlen = MultiByteToWideChar(CP_UTF8, 0, InStringToConvert.c_str(), -1, NULL, 0);
	wchar_t* buffer = new wchar_t[bufferlen];
	MultiByteToWideChar(CP_UTF8, 0, InStringToConvert.c_str(), -1, buffer, bufferlen);
	wstring wstr(buffer);
	delete[] buffer;
	return wstr;
}

string StringUtil::WStringToString(const wstring& InStringToConvert)
{
	int bufferlen = WideCharToMultiByte(CP_UTF8, 0, InStringToConvert.c_str(), -1, NULL, 0, NULL, NULL);
	char* buffer = new char[bufferlen];
	WideCharToMultiByte(CP_UTF8, 0, InStringToConvert.c_str(), -1, buffer, bufferlen, NULL, NULL);
	string str(buffer);
	delete[] buffer;
	return str;
}

wstring StringUtil::AnsiToWString(const string& InStringToConvert)
{
	int bufferlen = MultiByteToWideChar(CP_ACP, 0, InStringToConvert.c_str(), -1, NULL, 0);
	wchar_t* buffer = new wchar_t[bufferlen];
	MultiByteToWideChar(CP_ACP, 0, InStringToConvert.c_str(), -1, buffer, bufferlen);
	wstring wstr(buffer);
	delete[] buffer;
	return wstr;
}

string StringUtil::WStringToAnsi(const wstring& InStringToConvert)
{
	int bufferlen = WideCharToMultiByte(CP_ACP, 0, InStringToConvert.c_str(), -1, NULL, 0, NULL, NULL);
	char* buffer = new char[bufferlen];
	WideCharToMultiByte(CP_ACP, 0, InStringToConvert.c_str(), -1, buffer, bufferlen, NULL, NULL);
	string str(buffer);
	delete[] buffer;
	return str;
}

#else

wstring StringUtil::StringToWString(const string& str)
{
	return _wstr_null;
}

string StringUtil::WStringToString(const wstring& wstr)
{
	return _str_null;
}

wstring StringUtil::AnsiToWString(const string& str)
{
	return _wstr_null;
}

string StringUtil::WStringToAnsi(const wstring& wstr)
{
	return _str_null;
}

#endif

std::vector<string> StringUtil::GetBetween(const string& str, const string& boundary)
{
	std::vector<string> temp;
	string::size_type found1 = 0, found2;

	while (true)
	{
		found1 = str.find(boundary, found1);
		if (found1 != string::npos)
		{
			found2 = str.find(boundary, found1 + boundary.size());
			if (found2 != string::npos)
			{
				temp.push_back(str.substr(found1 + boundary.size(), found2 - found1 - boundary.size()));
				found1 = found2 + 1;
			}
			else break;
		}
		else break;
	}

	return temp;
}

std::vector<string> StringUtil::GetBetween(const string& str, const string& bound1, const string& bound2)
{
	std::vector<string> temp;
	string::size_type found1 = 0, found2;

	while (true)
	{
		found1 = str.find(bound1, found1);
		if (found1 != string::npos)
		{
			found2 = str.find(bound2, found1 + bound1.size());
			if (found2 != string::npos)
			{
				temp.push_back(str.substr(found1 + bound1.size(), found2 - found1 - bound1.size()));
				found1 = found2 + 1;
			}
			else break;
		}
		else break;
	}

	return temp;
}

std::vector<wstring> StringUtil::WGetBetween(const wstring& wstr, const wstring& boundary)
{
	std::vector<wstring> temp;
	wstring::size_type found1 = 0, found2;

	while (true)
	{
		found1 = wstr.find(boundary, found1);
		if (found1 != wstring::npos)
		{
			found2 = wstr.find(boundary, found1 + boundary.size());
			if (found2 != wstring::npos)
			{
				temp.push_back(wstr.substr(found1 + boundary.size(), found2 - found1 - boundary.size()));
				found1 = found2 + 1;
			}
			else break;
		}
		else break;
	}

	return temp;
}

std::vector<wstring> StringUtil::WGetBetween(const wstring& wstr, const wstring& bound1, const wstring& bound2)
{
	std::vector<wstring> temp;
	wstring::size_type found1 = 0, found2;

	while (true)
	{
		found1 = wstr.find(bound1, found1);
		if (found1 != wstring::npos)
		{
			found2 = wstr.find(bound2, found1 + bound1.size());
			if (found2 != wstring::npos)
			{
				temp.push_back(wstr.substr(found1 + bound1.size(), found2 - found1 - bound1.size()));
				found1 = found2 + 1;
			}
			else break;
		}
		else break;
	}

	return temp;
}

string StringUtil::GetFirstBetween(const string& str, const string& boundary)
{
	string::size_type found1, found2;

	found1 = str.find(boundary, _offset_0);
	if (found1 != string::npos)
	{
		found2 = str.find(boundary, found1 + boundary.size());
		if (found2 != string::npos)
		{
			return str.substr(found1 + boundary.size(), found2 - found1 - boundary.size());
		}
	}

	return _str_null;
}

wstring StringUtil::WGetFirstBetween(const wstring& wstr, const wstring& boundary)
{
	wstring::size_type found1, found2;

	found1 = wstr.find(boundary, _offset_0);
	if (found1 != wstring::npos)
	{
		found2 = wstr.find(boundary, found1 + boundary.size());
		if (found2 != wstring::npos)
		{
			return wstr.substr(found1 + boundary.size(), found2 - found1 - boundary.size());
		}
	}

	return _wstr_null;
}

std::vector<string> StringUtil::RemoveBetween(string& str, const string& boundary)
{
	std::vector<string> temp;
	string::size_type found1 = 0, found2;

	while (true)
	{
		found1 = str.find(boundary);
		if (found1 != string::npos)
		{
			found2 = str.find(boundary, found1 + boundary.size());
			if (found2 != string::npos)
			{
				temp.push_back(str.substr(found1 + boundary.size(), found2 - found1 - boundary.size()));
				str.erase(found1, found2 - found1 + boundary.size());
			}
			else break;
		}
		else break;
	}

	return temp;
}

std::vector<wstring> StringUtil::WRemoveBetween(wstring& wstr, const wstring& boundary)
{
	std::vector<wstring> temp;
	wstring::size_type found1 = 0, found2;

	while (true)
	{
		found1 = wstr.find(boundary);
		if (found1 != wstring::npos)
		{
			found2 = wstr.find(boundary, found1 + boundary.size());
			if (found2 != wstring::npos)
			{
				temp.push_back(wstr.substr(found1 + boundary.size(), found2 - found1 - boundary.size()));
				wstr.erase(found1, found2 - found1 + boundary.size());
			}
			else break;
		}
		else break;
	}

	return temp;
}

std::vector<string> StringUtil::ReplaceBetween(string& str, const string& boundary, const string& str_replace)
{
	std::vector<string> temp;
	string::size_type found1 = 0, found2;

	while (true)
	{
		found1 = str.find(boundary, found1);
		if (found1 != string::npos)
		{
			found2 = str.find(boundary, found1 + boundary.size());
			if (found2 != string::npos)
			{
				temp.push_back(str.substr(found1 + boundary.size(), found2 - found1 - boundary.size()));
				str.replace(found1, found2 - found1 + boundary.size(), str_replace);
				found1 += str_replace.size();
			}
			else break;
		}
		else break;
	}

	return temp;
}

std::vector<wstring> StringUtil::WReplaceBetween(wstring& wstr, const wstring& boundary, const wstring& wstr_replace)
{
	std::vector<wstring> temp;
	wstring::size_type found1 = 0, found2;

	while (true)
	{
		found1 = wstr.find(boundary, found1);
		if (found1 != wstring::npos)
		{
			found2 = wstr.find(boundary, found1 + boundary.size());
			if (found2 != wstring::npos)
			{
				temp.push_back(wstr.substr(found1 + boundary.size(), found2 - found1 - boundary.size()));
				wstr.replace(found1, found2 - found1 + boundary.size(), wstr_replace);
				found1 += wstr_replace.size();
			}
			else break;
		}
		else break;
	}

	return temp;
}

void StringUtil::EraseAll(string& str, const string& str_erase)
{
	string::size_type found;
	found = str.find(str_erase);
	while (found != string::npos)
	{
		str.erase(found, str_erase.size());
		found = str.find(str_erase, found);
	}
}

void StringUtil::WEraseAll(wstring& wstr, const wstring& wstr_erase)
{
	wstring::size_type found;
	found = wstr.find(wstr_erase);
	while (found != wstring::npos)
	{
		wstr.erase(found, wstr_erase.size());
		found = wstr.find(wstr_erase, found);
	}
}

string StringUtil::ToLowerCase(const string& str)
{
	string lower = str;
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

string StringUtil::WStringToStringV2(const wstring& wstr)
{
	std::setlocale(LC_ALL, "");
	const std::locale locale("");
	typedef std::codecvt<wchar_t, char, std::mbstate_t> converter_type;
	const converter_type& converter = std::use_facet<converter_type>(locale);
	std::vector<char> to(wstr.length() * converter.max_length());
	std::mbstate_t state;
	const wchar_t* from_next;
	char* to_next;
	const converter_type::result result = converter.out(state, wstr.data(), wstr.data() + wstr.length(), from_next, &to[0], &to[0] + to.size(), to_next);
	if (result == converter_type::ok || result == converter_type::noconv)
		return string(&to[0], to_next);
	else return _str_null;
}

int32 StringUtil::WCharToInt32(wchar_t wch)
{
	int32 result = -1;

	for (wchar_t const* const* p = std::begin(WCharDigitTables);
		p != std::end(WCharDigitTables) && result == -1;
		++p)
	{
		wchar_t const* q = std::find(*p, *p + 10, wch);
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
