/*********************************************************************
 *  StringManager.h
 *  Copyright (C) 2020 Jayou. All Rights Reserved.
 * 
 *  A set of string utility API.
 *********************************************************************/

#pragma once

#include "Core/TypeDef.h"

using vec_string  = std::vector<string>;
using vec_wstring = std::vector<wstring>;

class StringUtil
{
public:

	//---------------------------------------------------------------------------
	// Common utilities.
	//---------------------------------------------------------------------------

	/**
	*  Convert a string to a wstring.
	* 
	*  @param  InString  the string to convert.
	* 
	*  @return the converted string.
	**/
	static wstring StringToWString(const string& InString);

	/**
	 *  Convert a wstring to a string, may produce wrong result.
	 * 
	 *  @param  InString  the string to convert.
	 * 
	 *  @return the converted string.
	 */
	static string WStringToString(const wstring& InString);

	/**
	 *  Convert an ansi_string to a wstring.
	 * 
	 *  @param  InString  the string to convert.
	 * 
	 *  @return the converted string.
	 */
	static wstring AnsiToWString(const string& InString);

	/**
	 *  Convert a wstring to an ansi_string, may produce wrong result.
	 * 
	 *  @param  InString  the string to convert.
	 * 
	 *  @return the converted string.
	 */
	static string WStringToAnsi(const wstring& InString);

	/**
	 *  Get all string separate by the specifier in a string.
	 * 
	 *  @param  InString     the string to process.
	 *  @param  InSpecifier  the specifier as boundary.
	 * 
	 *  @return an array of matching strings.
	 */
	static vec_string GetBetween(const string& InString, const string& InSpecifier);

	/**
	 *  Get all string separate by two specifiers in a string.
	 * 
	 *  @param  InString     the string to process.
	 *  @param  InSpecLeft   the specifier as left boundary.
	 *  @param  InSpecRight  the specifier as right boundary.
	 * 
	 *  @return an array of matching strings.
	 */
	static vec_string GetBetween(const string& InString, const string& InSpecLeft, const string& InSpecRight);

	/**
	 *  Get all wstring separate by the specifier in a string.
	 * 
	 *  @param  InString     the string to process.
	 *  @param  InSpecifier  the specifier as boundary.
	 * 
	 *  @return an array of matching wstrings.
	 */
	static vec_wstring WGetBetween(const wstring& InString, const wstring& InSpecifier);

	/**
	 *  Get all wstring separate by two specifiers in a wstring.
	 * 
	 *  @param  InString     the string to process.
	 *  @param  InSpecLeft   the specifier as left boundary.
	 *  @param  InSpecRight  the specifier as right boundary.
	 * 
	 *  @return an array of matching wstrings.
	 */
	static vec_wstring WGetBetween(const wstring& InString, const wstring& InSpecLeft, const wstring& InSpecRight);

	/**
	 *  Get the first string separate by the specifier in a string.
	 * 
	 *  @param  InString     the string to process.
	 *  @param  InSpecifier  the specifier as boundary.
	 * 
	 *  @return the first matching string.
	 */
	static string GetFirstBetween(const string& InString, const string& InSpecifier);

	/**
	 *  Get the first wstring separate by the specifier in a wstring.
	 * 
	 *  @param  InString     the string to process.
	 *  @param  InSpecifier  the specifier as boundary.
	 * 
	 *  @return the first matching wstring.
	 */
	static wstring WGetFirstBetween(const wstring& InString, const wstring& InSpecifier);

	/**
	 *  Remove all string separate by the specifier in a string.
	 * 
	 *  @param  InString     the string to process.
	 *  @param  InSpecifier  the specifier as boundary.
	 * 
	 *  @return an array of removed strings.
	 */
	static vec_string RemoveBetween(string& InString, const string& InSpecifier);

	/**
	 *  Remove all wstring separate by the specifier in a wstring.
	 * 
	 *  @param  InString     the string to process.
	 *  @param  InSpecifier  the specifier as boundary.
	 * 
	 *  @return an array of removed wstrings.
	 */
	static vec_wstring WRemoveBetween(wstring& InString, const wstring& InSpecifier);

	/**
	 *  Replace all string separate by the specifier in a string with a new string.
	 * 
	 *  @param  InString     the string to process.
	 *  @param  InSpecifier  the specifier as boundary.
	 *  @param  InReplace    the new string to replace.
	 * 
	 *  @return an array of removed strings.
	 */
	static vec_string ReplaceBetween(string& InString, const string& InSpecifier, const string& InReplace);

	/**
	 *  Replace all wstring separate by the specifier in a wstring with a new wstring.
	 *
	 *  @param  InString     the string to process.
	 *  @param  InSpecifier  the specifier as boundary.
	 *  @param  InReplace    the new string to replace.
	 *
	 *  @return an array of removed wstrings.
	 */
	static vec_wstring WReplaceBetween(wstring& InString, const wstring& InSpecifier, const wstring& InReplace);

	/**
	 *  Erase all of the specific string in a string.
	 * 
	 *  @param  InString  the string to process.
	 *  @param  InErase   the specific string to erase.
	 */
	static void EraseAll(string& InString, const string& InErase);

	/**
	 *  Erase all of the specific wstring in a wstring.
	 * 
	 *  @param  InString  the string to process.
	 *  @param  InErase   the specific string to erase.
	 */
	static void WEraseAll(wstring& InString, const wstring& InErase);

	/**
	 *  Convert all chars to lower case in a string.
	 * 
	 *  @param  InString  the string to process.
	 * 
	 *  @return a new lower case string.
	 */
	static string ToLowerCase(const string& InString);

	/**
	 *  Convert a hexadecimal format string to a decimal number.
	 * 
	 *  @param  InHexStr  the string to process.
	 * 
	 *  @return a decimal number.
	 */
	static uint32 StrHexToNumeric(const string& InHexStr);

	/**
	 *  Convert a 16 Byte UUID to a human readable string.
	 * 
	 *  @param  InUUID  the UUID to process.
	 * 
	 *  @return a humen readable string.
	 */
	static string UUIDToString(const uint8* InUUID);

public:

	//---------------------------------------------------------------------------
	// Extended utilities.
	//---------------------------------------------------------------------------

	/**
	 *  Convert wstring to string, may produce wrong result(second version).
	 * 
	 *  @param  InString  the string to process.
	 * 
	 *  @return the converted string.
	 */
	static string WStringToStringV2(const wstring& InString);

	/**
	 *  Convert number represent by wchar to decimal.
	 * 
	 *  @param  InChar  the wchar to convert.
	 * 
	 *  @return a valid decimal number(0-9), or -1 if wchar is not a convertible number.
	 */
	static int32 WCharToInt32(wchar_t InChar);

	/**
	 *  Resolve the detail of a file path from a string.
	 * 
	 *  @param  InPath   the file path to resolve.
	 *  @param  OutName  output the file name.
	 *  @param  OutExt   output the file extension.
	 *  @param  OutDir   output the file directory.
	 * 
	 *  @return true if resolve is ok, otherwise false.
	 */
	static bool ExtractFilePath(const string& InPath, string* OutName = nullptr, string* OutExt = nullptr, string* OutDir = nullptr);

	/**
	 *  Resolve the detail of a file path from a wstring.
	 * 
	 *  @param  InPath   the file path to resolve.
	 *  @param  OutName  output the file name.
	 *  @param  OutExt   output the file extension.
	 *  @param  OutDir   output the file directory.
	 * 
	 *  @return true if resolve is ok, otherwise false.
	 */
	static bool WExtractFilePath(const wstring& InPath, wstring* OutName = nullptr, wstring* OutExt = nullptr, wstring* OutDir = nullptr);

public:

	//---------------------------------------------------------------------------
	// Template utilities, all were based on sstream, use with caution.
	//---------------------------------------------------------------------------

	/**
	 *  Convert a string to a specific type of numeric.
	 * 
	 *  @param  InString  the string to process.
	 * 
	 *  @return a specific type of numeric.
	 */
	template<typename T>
	static T StringToNumeric(const string& InString);

	/**
	 *  Convert a wstring to a specific type of numeric.
	 *
	 *  @param  InString  the string to process.
	 *
	 *  @return a specific type of numeric.
	 */
	template<typename T> 
	static T WStringToNumeric(const wstring& InString);

	/**
	 *  Resolve an array of typed data from a string.
	 * 
	 *  @param  InString     the string to process.
	 *  @param  InSeparator  the separator to part data.
	 * 
	 *  @return an array of typed data.
	 */
	template<typename T> 
	static std::vector<T> StringToArray(const string& InString, const char& InSeparator);

	/**
	 *  Resolve an array of typed data from a wstring.
	 *
	 *  @param  InString     the string to process.
	 *  @param  InSeparator  the separator to part data.
	 *
	 *  @return an array of typed data.
	 */
	template<typename T> 
	static std::vector<T> WStringToArray(const wstring& InString, const wchar_t& InSeparator);

	/**
	 *  A convenient string patching function.
	 *  Example: --StringUtil::Printf("Hello %, I am %.", "C++", 250)-- Equal to --Hello C++, I am 250.--
	 * 
	 *  @param  InFormat   main string body.
	 *  @param  InValue    specific typed value.
	 *  @param  ...InArgs  variable parameters.
	 * 
	 *  @return a patched string.
	 */
	template<typename T, typename... Targs> 
	static string Printf(const char* InFormat, T InValue, Targs... InArgs);
};

#include "StringManager.inl"
