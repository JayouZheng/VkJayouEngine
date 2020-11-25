//
// StringManager.inl
//

#pragma once

#include <sstream>

namespace StringManager
{
	// Base function.
	inline void InternalPrintf(std::ostringstream& OutSStream, const char* InFormat) { OutSStream << InFormat; }

	// Recursive call.
	template<typename T, typename... Targs>
	void InternalPrintf(std::ostringstream& OutSStream, const char* InFormat, T InValue, Targs... InArgs)
	{
		for (; *InFormat != '\0'; InFormat++)
		{
			if (*InFormat == '%')
			{
				OutSStream << InValue;
				StringManager::InternalPrintf(OutSStream, InFormat + 1, InArgs...);
				return;
			}

			OutSStream << *InFormat;
		}
	}
}

template<typename T>
T StringUtil::StringToNumeric(const string& InString)
{
	std::istringstream str_stream(InString);
	T temp;
	str_stream >> temp; // uint8/bool Not Supported.
	if (str_stream.fail())
	{
		temp = _numeric_max(T);
	}
	return temp;
}

template<typename T>
T StringUtil::WStringToNumeric(const wstring& InString)
{
	std::wistringstream wstr_stream(InString);
	T temp;
	wstr_stream >> temp; // uint8/bool Not Supported.
	if (wstr_stream.fail())
	{
		temp = _numeric_max(T);
	}
	return temp;
}

template<typename T>
std::vector<T> StringUtil::StringToArray(const string& InString, const char& InSeparator)
{
	std::istringstream str_stream(InString);
	std::vector<T> temp_array;
	string temp_str;
	T temp;
	while (true)
	{
		std::getline(str_stream, temp_str, InSeparator);
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
std::vector<T> StringUtil::WStringToArray(const wstring& InString, const wchar_t& InSeparator)
{
	std::wistringstream wstr_stream(InString);
	std::vector<T> temp_array;
	wstring temp_wstr;
	T temp;
	while (true)
	{
		std::getline(wstr_stream, temp_wstr, InSeparator);
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
string StringUtil::Printf(const char* InFormat, T InValue, Targs ...InArgs)
{
	std::ostringstream OutSStream;
	StringManager::InternalPrintf(OutSStream, InFormat, InValue, InArgs...);
	return OutSStream.str();
}