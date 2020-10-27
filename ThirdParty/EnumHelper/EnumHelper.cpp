//
// EnumHelper.cpp
// Jayou, 2020-10-27

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

void EraseAll(std::string& str, const std::string& str_erase)
{
    std::string::size_type found;
    found = str.find(str_erase);
    while (found != std::string::npos)
    {
        str.erase(found, str_erase.size());
        found = str.find(str_erase, found);
    }
}

using uBool32 = uint32_t;

struct Config
{
    uint32_t     interval_center;
    uint32_t     interval_right;
    uBool32      custom_key;
    std::string  vk_prefix;
};

int main()
{
    std::cout << "EnumHelper v1.0\n";

    Config config = {};
    std::cout << "config interval: ";
    std::cin >> config.interval_center;
    std::cin >> config.custom_key;
    if (!config.custom_key) std::cin >> config.vk_prefix;
    //std::cin >> config.interval_right;
    std::cin.get();

    std::cout << "input \"end\" to end enum.\n";
    std::cout << "input enum:\n";

    std::vector<std::string> outStrs;
    std::string inStr;
    std::string::size_type found;
    while (true)
    {      
        std::getline(std::cin, inStr);

        if (inStr == "end") break;
        
        EraseAll(inStr, " ");
        if (!config.custom_key) EraseAll(inStr, config.vk_prefix);

        found = inStr.find('=', 0);
        if (found != std::string::npos)
            outStrs.push_back(inStr.substr(0, found));
        else
        {
            found = inStr.find(',', 0);
            if (found != std::string::npos)
                outStrs.push_back(inStr.substr(0, found));
            else
                outStrs.push_back(inStr);
        }
    }

    uint32_t maxLength = 0;
    for (auto& str : outStrs)
        maxLength = std::max<uint32_t>(maxLength, (uint32_t)str.length());

    if (config.custom_key)
        for (auto& str : outStrs)
            std::cout << "{ \"\", " << std::setw(maxLength + (uint32_t)config.vk_prefix.length()) << std::left << config.vk_prefix + str << " }," << std::endl;
    else
        for (auto& str : outStrs)
            std::cout << "{ \"" << std::setw(maxLength + config.interval_center + 2) << std::left << str + "\"," << std::setw(maxLength + (uint32_t)config.vk_prefix.length()) << config.vk_prefix + str << " }," << std::endl;
}
