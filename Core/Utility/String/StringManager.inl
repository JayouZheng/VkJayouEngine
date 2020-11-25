//
// StringManager.inl
//

#pragma once

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
T StringUtil::StringToNumeric(const string& str)
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
T StringUtil::WStringToNumeric(const wstring& wstr)
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
std::vector<T> StringUtil::StringToArray(const string& str, const char& separator)
{
	std::istringstream str_stream(str);
	std::vector<T> temp_array;
	string temp_str;
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
std::vector<T> StringUtil::WStringToArray(const wstring& wstr, const wchar_t& separator)
{
	std::wistringstream wstr_stream(wstr);
	std::vector<T> temp_array;
	wstring temp_wstr;
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
string StringUtil::Printf(const char* InFormat, T InValue, Targs ...InArgs)
{
	std::ostringstream OutSStream;
	StringManager::InternalPrintf(OutSStream, InFormat, InValue, InArgs...);
	return OutSStream.str();
}