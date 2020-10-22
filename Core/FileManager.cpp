//
// FileManager.cpp
//

#include "FileManager.h"
#include "LogSystem.h"
#include "StringManager.h"

bool FileUtil::Read(const std::string& InPath, std::vector<uint8>& OutData)
{
	LogSystem::Log(StringUtil::Printf("Begin read file: %", InPath), LogSystem::Category::IO);

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

	LogSystem::Log(StringUtil::Printf("End read file: %", InPath), LogSystem::Category::IO);
	return true;
}

bool FileUtil::Write(const std::string& InPath, const std::vector<uint8>& InData)
{
	LogSystem::Log(StringUtil::Printf("Begin write file: %", InPath), LogSystem::Category::IO);

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

	LogSystem::Log(StringUtil::Printf("End write file: %", InPath), LogSystem::Category::IO);
	return true;
}
