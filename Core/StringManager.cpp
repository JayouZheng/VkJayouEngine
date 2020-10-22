//
// StringManager.cpp
//

#include "StringManager.h"
#include "Platform.h"
#include <clocale>
#include <iomanip>

wchar_t const* const WCharDigitTables[] =
{
	L"0123456789",
	L"\u0660\u0661\u0662\u0663\u0664\u0665\u0666\u0667\u0668\u0669",
};

#if PLATFORM_WINDOW

std::wstring StringUtil::StringToWString(const std::string& str)
{
	int bufferlen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* buffer = new wchar_t[bufferlen];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer, bufferlen);
	std::wstring wstr(buffer);
	delete[] buffer;
	return wstr;
}

std::string StringUtil::WStringToString(const std::wstring& wstr)
{
	int bufferlen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	char* buffer = new char[bufferlen];
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, buffer, bufferlen, NULL, NULL);
	std::string str(buffer);
	delete[] buffer;
	return str;
}

std::wstring StringUtil::AnsiToWString(const std::string& str)
{
	int bufferlen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* buffer = new wchar_t[bufferlen];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, bufferlen);
	std::wstring wstr(buffer);
	delete[] buffer;
	return wstr;
}

std::string StringUtil::WStringToAnsi(const std::wstring& wstr)
{
	int bufferlen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	char* buffer = new char[bufferlen];
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, buffer, bufferlen, NULL, NULL);
	std::string str(buffer);
	delete[] buffer;
	return str;
}

#else

std::wstring StringUtil::StringToWString(const std::string& str)
{
	return _wstr_null;
}

std::string StringUtil::WStringToString(const std::wstring& wstr)
{
	return _str_null;
}

std::wstring StringUtil::AnsiToWString(const std::string& str)
{
	return _wstr_null;
}

std::string StringUtil::WStringToAnsi(const std::wstring& wstr)
{
	return _str_null;
}

#endif

std::vector<std::string> StringUtil::GetBetween(const std::string& str, const std::string& boundary)
{
	std::vector<std::string> temp;
	std::string::size_type found1 = 0, found2;

	while (true)
	{
		found1 = str.find(boundary, found1);
		if (found1 != std::string::npos)
		{
			found2 = str.find(boundary, found1 + boundary.size());
			if (found2 != std::string::npos)
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

std::vector<std::string> StringUtil::GetBetween(const std::string& str, const std::string& bound1, const std::string& bound2)
{
	std::vector<std::string> temp;
	std::string::size_type found1 = 0, found2;

	while (true)
	{
		found1 = str.find(bound1, found1);
		if (found1 != std::string::npos)
		{
			found2 = str.find(bound2, found1 + bound1.size());
			if (found2 != std::string::npos)
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

std::vector<std::wstring> StringUtil::WGetBetween(const std::wstring& wstr, const std::wstring& boundary)
{
	std::vector<std::wstring> temp;
	std::wstring::size_type found1 = 0, found2;

	while (true)
	{
		found1 = wstr.find(boundary, found1);
		if (found1 != std::wstring::npos)
		{
			found2 = wstr.find(boundary, found1 + boundary.size());
			if (found2 != std::wstring::npos)
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

std::vector<std::wstring> StringUtil::WGetBetween(const std::wstring& wstr, const std::wstring& bound1, const std::wstring& bound2)
{
	std::vector<std::wstring> temp;
	std::wstring::size_type found1 = 0, found2;

	while (true)
	{
		found1 = wstr.find(bound1, found1);
		if (found1 != std::wstring::npos)
		{
			found2 = wstr.find(bound2, found1 + bound1.size());
			if (found2 != std::wstring::npos)
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

std::string StringUtil::GetFirstBetween(const std::string& str, const std::string& boundary)
{
	std::string::size_type found1, found2;

	found1 = str.find(boundary, _offset_0);
	if (found1 != std::string::npos)
	{
		found2 = str.find(boundary, found1 + boundary.size());
		if (found2 != std::string::npos)
		{
			return str.substr(found1 + boundary.size(), found2 - found1 - boundary.size());
		}
	}

	return _str_null;
}

std::wstring StringUtil::WGetFirstBetween(const std::wstring& wstr, const std::wstring& boundary)
{
	std::wstring::size_type found1, found2;

	found1 = wstr.find(boundary, _offset_0);
	if (found1 != std::wstring::npos)
	{
		found2 = wstr.find(boundary, found1 + boundary.size());
		if (found2 != std::wstring::npos)
		{
			return wstr.substr(found1 + boundary.size(), found2 - found1 - boundary.size());
		}
	}

	return _wstr_null;
}

std::vector<std::string> StringUtil::RemoveBetween(std::string& str, const std::string& boundary)
{
	std::vector<std::string> temp;
	std::string::size_type found1 = 0, found2;

	while (true)
	{
		found1 = str.find(boundary);
		if (found1 != std::string::npos)
		{
			found2 = str.find(boundary, found1 + boundary.size());
			if (found2 != std::string::npos)
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

std::vector<std::wstring> StringUtil::WRemoveBetween(std::wstring& wstr, const std::wstring& boundary)
{
	std::vector<std::wstring> temp;
	std::wstring::size_type found1 = 0, found2;

	while (true)
	{
		found1 = wstr.find(boundary);
		if (found1 != std::wstring::npos)
		{
			found2 = wstr.find(boundary, found1 + boundary.size());
			if (found2 != std::wstring::npos)
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

std::vector<std::string> StringUtil::ReplaceBetween(std::string& str, const std::string& boundary, const std::string& str_replace)
{
	std::vector<std::string> temp;
	std::string::size_type found1 = 0, found2;

	while (true)
	{
		found1 = str.find(boundary, found1);
		if (found1 != std::string::npos)
		{
			found2 = str.find(boundary, found1 + boundary.size());
			if (found2 != std::string::npos)
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

std::vector<std::wstring> StringUtil::WReplaceBetween(std::wstring& wstr, const std::wstring& boundary, const std::wstring& wstr_replace)
{
	std::vector<std::wstring> temp;
	std::wstring::size_type found1 = 0, found2;

	while (true)
	{
		found1 = wstr.find(boundary, found1);
		if (found1 != std::wstring::npos)
		{
			found2 = wstr.find(boundary, found1 + boundary.size());
			if (found2 != std::wstring::npos)
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

void StringUtil::EraseAll(std::string& str, const std::string& str_erase)
{
	std::string::size_type found;
	found = str.find(str_erase);
	while (found != std::string::npos)
	{
		str.erase(found, str_erase.size());
		found = str.find(str_erase, found);
	}
}

void StringUtil::WEraseAll(std::wstring& wstr, const std::wstring& wstr_erase)
{
	std::wstring::size_type found;
	found = wstr.find(wstr_erase);
	while (found != std::wstring::npos)
	{
		wstr.erase(found, wstr_erase.size());
		found = wstr.find(wstr_erase, found);
	}
}

std::string StringUtil::ToLowerCase(const std::string& str)
{
	std::string lower = str;
	std::transform(lower.begin(), lower.end(), lower.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return lower;
}

uint32 StringUtil::StrHexToNumeric(const std::string& InHexStr)
{
	std::stringstream strs;
	strs << InHexStr;
	uint32 value;
	strs >> std::hex >> value;
	return value;
}

std::string StringUtil::UUIDToString(const uint8* InUUID)
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

std::string StringUtil::WStringToStringV2(const std::wstring& wstr)
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
		return std::string(&to[0], to_next);
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

bool StringUtil::ExtractFilePath(const std::string& InPath, std::string* OutName, std::string* OutExt, std::string* OutDir)
{
	std::string::size_type found1, found2;

	found1 = InPath.find_last_of("/\\");

	if (found1 != std::string::npos)
	{
		if (OutDir != nullptr) *OutDir = InPath.substr(0, found1);

		std::string file = InPath.substr(found1 + 1);

		found2 = file.find_last_of(".");

		if (found2 != std::string::npos)
		{
			if (OutName != nullptr) *OutName = file.substr(0, found2);
			if (OutExt != nullptr) *OutExt = file.substr(found2 + 1);

			return true;
		}
		return false;
	}
	return false;
}

bool StringUtil::WExtractFilePath(const std::wstring& InPath, std::wstring* OutName, std::wstring* OutExt, std::wstring* OutDir)
{
	std::wstring::size_type found1, found2;

	found1 = InPath.find_last_of(L"/\\");

	if (found1 != std::wstring::npos)
	{
		if (OutDir != nullptr) *OutDir = InPath.substr(0, found1);

		std::wstring file = InPath.substr(found1 + 1);

		found2 = file.find_last_of(L".");

		if (found2 != std::wstring::npos)
		{
			if (OutName != nullptr) *OutName = file.substr(0, found2);
			if (OutExt != nullptr) *OutExt = file.substr(found2 + 1);

			return true;
		}
		return false;
	}
	return false;
}
