//
// StringManager.h
// NOTE: std::numeric_limits<T>::max() Conflict with windows.h #undef max...

#pragma once

#include <sstream>
#include <codecvt>
#include "TypeDef.h"

class StringUtil
{
public:

	// Common utilities.

	static std::wstring                StringToWString   (const std::string& str);

	static std::string                 WStringToString   (const std::wstring& wstr);

	static std::wstring                AnsiToWString     (const std::string& str);

	static std::string                 WStringToAnsi     (const std::wstring& wstr);

	static std::vector<std::string>    GetBetween        (const std::string& str, const std::string& boundary);

	static std::vector<std::string>    GetBetween        (const std::string& str, const std::string& bound1, const std::string& bound2);

	static std::vector<std::wstring>   WGetBetween       (const std::wstring& wstr, const std::wstring& boundary);

	static std::vector<std::wstring>   WGetBetween       (const std::wstring& wstr, const std::wstring& bound1, const std::wstring& bound2);

	static std::string                 GetFirstBetween   (const std::string& str, const std::string& boundary);

	static std::wstring                WGetFirstBetween  (const std::wstring& wstr, const std::wstring& boundary);

	static std::vector<std::string>    RemoveBetween     (std::string& str, const std::string& boundary);

	static std::vector<std::wstring>   WRemoveBetween    (std::wstring& wstr, const std::wstring& boundary);

	static std::vector<std::string>    ReplaceBetween    (std::string& str, const std::string& boundary, const std::string& str_replace);

	static std::vector<std::wstring>   WReplaceBetween   (std::wstring& wstr, const std::wstring& boundary, const std::wstring& wstr_replace);

	static void                        EraseAll          (std::string& str, const std::string& str_erase);

	static void                        WEraseAll         (std::wstring& wstr, const std::wstring& wstr_erase);

public:

	// Extended utilities.

	static std::string                 WStringToStringV2 (const std::wstring& wstr);

	static int32                       WCharToInt32      (wchar_t wch);

	static bool                        ExtractFilePath   (const std::string& InPath, std::string* OutName = nullptr, std::string* OutExt = nullptr, std::string* OutDir = nullptr);

	static bool                        WExtractFilePath  (const std::wstring& InPath, std::wstring* OutName = nullptr, std::wstring* OutExt = nullptr, std::wstring* OutDir = nullptr);

public:

	// Template utilities.

	template<typename T> static T                                StringToNumeric(const std::string& str);

	template<typename T> static T                                WStringToNumeric(const std::wstring& wstr);

	template<typename T> static std::vector<T>                   StringToArray(const std::string& str, const char& separator);

	template<typename T> static std::vector<T>                   WStringToArray(const std::wstring& wstr, const wchar_t& separator);

	template<typename T, typename... Targs> static std::string   Printf(const char* InFormat, T InValue, Targs... InArgs);

private:

	// Base function.
	static inline void Printf(std::ostringstream& OutSStream, const char* InFormat) { OutSStream << InFormat; }

	// Recursive call.
	template<typename T, typename... Targs>
	static void Printf(std::ostringstream& OutSStream, const char* InFormat, T InValue, Targs... InArgs)
	{
		for (; *InFormat != '\0'; InFormat++)
		{
			if (*InFormat == '%')
			{
				OutSStream << InValue;
				StringUtil::Printf(OutSStream, InFormat + 1, InArgs...);
				return;
			}

			OutSStream << *InFormat;
		}
	}
};

#pragma region Template Impl

template<typename T>
T StringUtil::StringToNumeric(const std::string& str)
{
	std::istringstream str_stream(str);
	T temp;
	str_stream >> temp; // uint8/bool Not Supported.
	if (str_stream.fail())
	{
		temp = _numeric_max(T);
	}
	return temp;
}

template<typename T>
T StringUtil::WStringToNumeric(const std::wstring& wstr)
{
	std::wistringstream wstr_stream(wstr);
	T temp;
	wstr_stream >> temp; // uint8/bool Not Supported.
	if (wstr_stream.fail())
	{
		temp = _numeric_max(T);
	}
	return temp;
}

template<typename T>
std::vector<T> StringUtil::StringToArray(const std::string& str, const char& separator)
{
	std::istringstream str_stream(str);
	std::vector<T> temp_array;
	std::string temp_str;
	T temp;
	while (true)
	{
		std::getline(str_stream, temp_str, separator);
		if (str_stream.fail())
			break;
		temp = StringUtil::StringToNumeric<T>(temp_str);
		if (temp == _numeric_max(T))
			continue;
		temp_array.push_back(temp);
	}
	return temp_array;
}

template<typename T>
std::vector<T> StringUtil::WStringToArray(const std::wstring& wstr, const wchar_t& separator)
{
	std::wistringstream wstr_stream(wstr);
	std::vector<T> temp_array;
	std::wstring temp_wstr;
	T temp;
	while (true)
	{
		std::getline(wstr_stream, temp_wstr, separator);
		if (wstr_stream.fail())
			break;
		temp = StringUtil::WStringToNumeric<T>(temp_wstr);
		if (temp == _numeric_max(T))
			continue;
		temp_array.push_back(temp);
	}
	return temp_array;
}

template<typename T, typename ...Targs>
std::string StringUtil::Printf(const char* InFormat, T InValue, Targs ...InArgs)
{
	std::ostringstream OutSStream;
	StringUtil::Printf(OutSStream, InFormat, InValue, InArgs...);
	return OutSStream.str();
}

#pragma endregion
