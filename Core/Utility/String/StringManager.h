/*********************************************************************
 *  @file   StringManager.h
 *  @brief  A set of string utility API.
 * 
 *  @author Jayou
 *  @date   2020-11-24
 *********************************************************************/

#pragma once

#include <sstream>
#include <codecvt>
#include "Core/TypeDef.h"

class StringUtil
{
public:

	//---------------------------------------------------------------------------
	// Common utilities.
	//---------------------------------------------------------------------------

	/**
	*  Convert string to wstring.
	* 
	*  @param  InStringToConvert
	* 
	*  @return The converted string.
	**/
	static wstring StringToWString(const string& InStringToConvert);

	/**
	 *  Convert wstring to string.
	 * 
	 *  @param  InStringToConvert
	 * 
	 *  @return The converted string.
	 */
	static string WStringToString(const wstring& InStringToConvert);

	/**
	 *  Convert ansi_string to wstring.
	 * 
	 *  @param  InStringToConvert
	 * 
	 *  @return The converted string.
	 */
	static wstring AnsiToWString(const string& InStringToConvert);

	/**
	 *  Convert wstring to ansi_string.
	 * 
	 *  @param  InStringToConvert
	 * 
	 *  @return The converted string.
	 */
	static string WStringToAnsi(const wstring& InStringToConvert);

	static std::vector<string>    GetBetween        (const string& str, const string& boundary);

	static std::vector<string>    GetBetween        (const string& str, const string& bound1, const string& bound2);

	static std::vector<wstring>   WGetBetween       (const wstring& wstr, const wstring& boundary);

	static std::vector<wstring>   WGetBetween       (const wstring& wstr, const wstring& bound1, const wstring& bound2);

	static string                 GetFirstBetween   (const string& str, const string& boundary);

	static wstring                WGetFirstBetween  (const wstring& wstr, const wstring& boundary);

	static std::vector<string>    RemoveBetween     (string& str, const string& boundary);

	static std::vector<wstring>   WRemoveBetween    (wstring& wstr, const wstring& boundary);

	static std::vector<string>    ReplaceBetween    (string& str, const string& boundary, const string& str_replace);

	static std::vector<wstring>   WReplaceBetween   (wstring& wstr, const wstring& boundary, const wstring& wstr_replace);

	static void                        EraseAll          (string& str, const string& str_erase);

	static void                        WEraseAll         (wstring& wstr, const wstring& wstr_erase);

	static string                 ToLowerCase       (const string& str);

	static uint32                      StrHexToNumeric   (const string& InHexStr);

	static string                 UUIDToString      (const uint8* InUUID);

public:

	// Extended utilities.

	static string                 WStringToStringV2 (const wstring& wstr);

	static int32                       WCharToInt32      (wchar_t wch);

	static bool                        ExtractFilePath   (const string& InPath, string* OutName = nullptr, string* OutExt = nullptr, string* OutDir = nullptr);

	static bool                        WExtractFilePath  (const wstring& InPath, wstring* OutName = nullptr, wstring* OutExt = nullptr, wstring* OutDir = nullptr);

public:

	// Template utilities.

	template<typename T> static T                                StringToNumeric(const string& str);

	template<typename T> static T                                WStringToNumeric(const wstring& wstr);

	template<typename T> static std::vector<T>                   StringToArray(const string& str, const char& separator);

	template<typename T> static std::vector<T>                   WStringToArray(const wstring& wstr, const wchar_t& separator);

	// Use This Function Carefully!!! <sstream> Auto Recognize Variable Type May Cause Undesirable Result!!!
	template<typename T, typename... Targs> static string   Printf(const char* InFormat, T InValue, Targs... InArgs);
};

#include "StringManager.inl"
