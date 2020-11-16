//
// FileManager.cpp
//

#define _CRT_SECURE_NO_WARNINGS

#include "FileManager.h"
#include "Core/Utility/LogSystem.h"
#include "Core/Utility/String/StringManager.h"

bool FileUtil::Read(const std::string& InPath, std::vector<uint8>& OutData)
{
	LogSystem::Log(StringUtil::Printf("Begin read file: %", InPath), LogSystem::Category::IO);

#ifndef ENABLE_TRY_CATCH

	FILE* pFile;
	pFile = fopen(InPath.c_str(), "rb");
	if (pFile == NULL)
	{
		_log_error(StringUtil::Printf("Open file(%) for reading failed!", InPath), LogSystem::Category::IO);
		return false;
	}

	fseek(pFile, 0, SEEK_END);
	int64 size = ftell(pFile);
	rewind(pFile);

	if (ferror(pFile))
	{
		_log_error(StringUtil::Printf("Get size of file(%) failed!", InPath), LogSystem::Category::IO);
		return false;
	}

	OutData.resize(size);
	fread(OutData.data(), sizeof(uint8), OutData.size(), pFile);
	if (ferror(pFile))
	{
		_log_error(StringUtil::Printf("Reading file(%) failed!", InPath), LogSystem::Category::IO);
		return false;
	}

	fclose(pFile);

#else

	std::ifstream ifs(InPath, std::ios::binary | std::ios::ate);

	if (ifs.is_open())
	{
		try
		{
			// Get size of file.
			ifs.seekg(0, ifs.end);
			int32 size = (int32)ifs.tellg();
			ifs.seekg(0, ifs.beg);
			if (size == -1)
			{
				_log_error(StringUtil::Printf("Reading error at %", InPath), LogSystem::Category::IO);
				return false;
			}
			
			OutData.resize(size);

			ifs.read((char*)OutData.data(), size);
			ifs.close();
		}
		catch (const std::ios_base::failure& e)
		{
			_log_error(StringUtil::Printf("Exception thrown, what %, code %", e.what(), e.code()), LogSystem::Category::IO);
			return false;
		}
	}
	else
	{
		_log_error(StringUtil::Printf("Can't open file: %", InPath), LogSystem::Category::IO);
		return false;
	}

#endif

	LogSystem::Log(StringUtil::Printf("End read file: %", InPath), LogSystem::Category::IO);
	return true;
}

bool FileUtil::Write(const std::string& InPath, const std::vector<uint8>& InData)
{
	LogSystem::Log(StringUtil::Printf("Begin write file: %", InPath), LogSystem::Category::IO);

#ifndef ENABLE_TRY_CATCH

	FILE* pFile;
	pFile = fopen(InPath.c_str(), "wb");
	if (pFile == NULL)
	{
		_log_error(StringUtil::Printf("Open file(%) for writing failed!", InPath), LogSystem::Category::IO);
		return false;
	}

	fwrite(InData.data(), sizeof(uint8), InData.size(), pFile);
	if (ferror(pFile))
	{
		_log_error(StringUtil::Printf("writing to file(%) failed!", InPath), LogSystem::Category::IO);
		return false;
	}

	fclose(pFile);

#else

	std::ofstream ofs(InPath, std::ofstream::binary);

	if (ofs.is_open())
	{
		try
		{
			ofs.write((const char*)InData.data(), InData.size());
			ofs.close();
		}
		catch (const std::ios_base::failure& e)
		{
			_log_error(StringUtil::Printf("Exception thrown, what %, code %", e.what(), e.code()), LogSystem::Category::IO);
			return false;
		}
	}

#endif

	LogSystem::Log(StringUtil::Printf("End write file: %", InPath), LogSystem::Category::IO);
	return true;
}
