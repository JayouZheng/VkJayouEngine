//
// enum_all_key.cpp
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <vector>

#define _length 12

using uint8 = uint8_t;
using uint64 = uint64_t;

constexpr uint64 max_single_pass_count = 89478485u;
constexpr size_t max_single_file_size_in_byte = max_single_pass_count * _length; // 1 GB

constexpr uint64 all = 4000000000;
constexpr uint64 sub_all = 1000000000;

static uint64 user_expected_count = 0;

bool write_to_file(const char* InPath, const void* InData, size_t InSize)
{
	FILE* pFile;
	pFile = fopen(InPath, "ab");
	if (pFile == NULL)
		return false;

	fwrite(InData, sizeof(uint8), InSize, pFile);
	if (ferror(pFile))
		return false;

	fclose(pFile);
	return true;
}

void single_pass(size_t file_size_in_byte, uint64 start)
{
	uint8* data = new uint8[file_size_in_byte];

	if (data == nullptr)
		exit(1);

	uint8 second_num[4] = { 51, 53, 55, 56 };

	for (uint64 i = start / sub_all; i < 4; i++)
	{
		for (uint64 j = start % sub_all; j < sub_all; j++)
		{
			uint64 offset = i * sub_all + j - start;

			if (offset + 1 > user_expected_count % max_single_pass_count)
				break;

			// 48-57, 10=\n
			uint8 number[_length];
			number[0] = 49;
			number[1] = second_num[i];
			number[11] = 10;

			// number[2-10] 000 000 000 -> 999 999 999
			number[2] = uint8(j / 100000000 + 48);
			number[3] = uint8(j / 10000000 - (j / 100000000) * 10 + 48);
			number[4] = uint8(j / 1000000 - (j / 10000000) * 10 + 48);
			number[5] = uint8(j / 100000 - (j / 1000000) * 10 + 48);
			number[6] = uint8(j / 10000 - (j / 100000) * 10 + 48);
			number[7] = uint8(j / 1000 - (j / 10000) * 10 + 48);
			number[8] = uint8(j / 100 - (j / 1000) * 10 + 48);
			number[9] = uint8(j / 10 - (j / 100) * 10 + 48);
			number[10] = uint8(j / 1 - (j / 10) * 10 + 48);

			memcpy(data + offset * _length, number, _length);
		}
	}

	write_to_file("key.txt", data, file_size_in_byte);
	delete data;
}

int main()
{
    std::cout << "enum all key(1,3578,0-9,all=11)...\n";
	std::cout << "enter your range(-1 for all):";
	
	std::cin >> user_expected_count;

	if (user_expected_count == (uint64)-1)
		user_expected_count = all;

	size_t file_size_in_byte = size_t(user_expected_count * _length);

	uint64 start = 0;
	while (true)
	{
		if (file_size_in_byte < max_single_file_size_in_byte)
		{
			single_pass(file_size_in_byte, start);
			break;
		}
		
		single_pass(max_single_file_size_in_byte, start);

		file_size_in_byte -= max_single_file_size_in_byte;
		if (file_size_in_byte == 0) break;
		start += max_single_file_size_in_byte;
	}

	std::cout << "everything goes well!\n";
	system("pause");

	return 0;
}
