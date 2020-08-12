//
// main.cpp
//

#if 1

 #include "Core/TypeDef.h"
 #include "vk_app.h"
 
 int main()
 {
 	_cmd_print_line("Hello Vulkan!\n");
 
 	{
 		vk_app app;
 		app.Begin();
 	}
 
 	system("pause");
 }

#endif

#pragma region Fossilize Test

#if 0

#ifndef VK_LAYER_fossilize
#define VK_LAYER_fossilize "VK_LAYER_fossilize"
#endif

#ifdef ANDROID
#include <android/log.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Fossilize", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Fossilize", __VA_ARGS__)
#else
#include <stdio.h>
#define LOGI(...) fprintf(stderr, "Fossilize INFO: " __VA_ARGS__)
#define LOGE(...) fprintf(stderr, "Fossilize ERROR: " __VA_ARGS__)
#endif

#include "Fossilize/fossilize_application_filter.hpp"
#include "vulkan/vulkan.h"
#include <stdlib.h>

#pragma comment(lib, "fossilize.lib")

static bool write_string_to_file(const char *path, const char *str)
{
	FILE *file;
	fopen_s(&file, path, "w");
	if (!file)
		return false;

	fprintf(file, "%s\n", str);
	fclose(file);
	return true;
}

int main()
{
	const char *test_json =
		R"(
{
	"asset": "FossilizeApplicationInfoFilter",
	"version" : 1,
	"blacklistedApplicationNames" : [ "A",  "B", "C" ],
	"blacklistedEngineNames" : [ "D", "E", "F" ],
	"applicationFilters" : {
		"test1" : { "minimumApplicationVersion" : 10 },
		"test2" : { "minimumApplicationVersion" : 10, "minimumEngineVersion" : 1000 },
		"test3" : { "minimumApiVersion" : 50 }
	},
	"engineFilters" : {
		"test1" : { "minimumEngineVersion" : 10 },
		"test2" : { "minimumEngineVersion" : 10, "minimumApplicationVersion" : 1000 },
		"test3" : { "minimumApiVersion" : 50 }
	}
}
)";

	if (!write_string_to_file(".__test_appinfo.json", test_json))
		return EXIT_FAILURE;

	Fossilize::ApplicationInfoFilter filter;
	filter.parse_async(".__test_appinfo.json");

	if (!filter.check_success())
	{
		LOGE("Parsing did not complete successfully.\n");
		return EXIT_FAILURE;
	}

	VkApplicationInfo appinfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };

	if (!filter.test_application_info(nullptr))
		return EXIT_FAILURE;

	// Test blacklists
	appinfo.pApplicationName = "A";
	appinfo.pEngineName = "G";
	if (filter.test_application_info(&appinfo))
		return EXIT_FAILURE;

	appinfo.pApplicationName = "D";
	appinfo.pEngineName = "A";
	if (!filter.test_application_info(&appinfo))
		return EXIT_FAILURE;

	appinfo.pApplicationName = "H";
	appinfo.pEngineName = "E";
	if (filter.test_application_info(&appinfo))
		return EXIT_FAILURE;

	// Test application version filtering
	appinfo.pApplicationName = "test1";
	appinfo.pEngineName = nullptr;
	appinfo.applicationVersion = 9;
	if (filter.test_application_info(&appinfo))
		return EXIT_FAILURE;
	appinfo.applicationVersion = 10;
	if (!filter.test_application_info(&appinfo))
		return EXIT_FAILURE;

	// Engine version should be ignored for appinfo filters.
	appinfo.pApplicationName = "test2";
	if (!filter.test_application_info(&appinfo))
		return EXIT_FAILURE;

	appinfo.pApplicationName = "test3";
	appinfo.applicationVersion = 0;
	appinfo.apiVersion = 49;
	if (filter.test_application_info(&appinfo))
		return EXIT_FAILURE;

	appinfo.apiVersion = 50;
	if (!filter.test_application_info(&appinfo))
		return EXIT_FAILURE;

	// Test engine version filtering
	appinfo.pApplicationName = nullptr;
	appinfo.pEngineName = "test1";
	appinfo.engineVersion = 9;
	if (filter.test_application_info(&appinfo))
		return EXIT_FAILURE;
	appinfo.engineVersion = 10;
	if (!filter.test_application_info(&appinfo))
		return EXIT_FAILURE;

	// Engine version should be ignored for appinfo filters.
	appinfo.pEngineName = "test2";
	if (!filter.test_application_info(&appinfo))
		return EXIT_FAILURE;

	appinfo.pEngineName = "test3";
	appinfo.engineVersion = 0;
	appinfo.apiVersion = 49;
	if (filter.test_application_info(&appinfo))
		return EXIT_FAILURE;

	appinfo.apiVersion = 50;
	if (!filter.test_application_info(&appinfo))
		return EXIT_FAILURE;

	remove(".__test_appinfo.json");
}

#endif

#pragma endregion

#pragma region iniparser test

#if 0

#include "iniparser/ini.hpp"

#include <sstream>

void test1();
void test2();
void test3();
void test4();
void test5();

int main()
{
	test1();
	test2();
	test3();
	test4();
	test5();

	return 0;
}

void test1()
{
	std::stringstream ss;

	ss <<
		"a=1\n"
		"b=1\n\n"
		"[e]\n"
		"ea=1\n"
		"eb=1\n\n"
		"[c]\n"
		"ca=2\n"
		"cb=2\n\n"
		"[[d]]\n"
		"da=3\n"
		"db=3\n\n"
		"[A]\n"
		"Aa=4\n"
		"Ab=4\n";

	INI::Parser p(ss);
	std::stringstream out;
	p.dump(out);
	assert(out.str() == ss.str());

	assert(p.top()["a"] == "1");
	assert(p.top()("e")["ea"] == "1");
}

void test2()
{
	std::stringstream ss;

	ss <<
		"a=1\n"
		"a=1\n\n";

	try {
		INI::Parser p(ss);
	}
	catch (std::runtime_error& e) {
		assert(std::string(e.what()) == "duplicated key found on line #2");
	}
}

void test3()
{
	std::stringstream ss;

	ss <<
		"a=1\n"
		"b\n";

	try {
		INI::Parser p(ss);
	}
	catch (std::runtime_error& e) {
		assert(std::string(e.what()) == "no '=' found on line #2");
	}
}

void test4()
{
	std::stringstream ss;

	ss <<
		"a=1\n"
		"[b]\n"
		"[[[a]]]\n";

	try {
		INI::Parser p(ss);
	}
	catch (std::runtime_error& e) {
		assert(std::string(e.what()) == "section with wrong depth on line #3");
	}
}

void test5()
{
	std::stringstream ss;

	ss <<
		"a=1\n"
		"[b]\n"
		"[b]\n";

	try {
		INI::Parser p(ss);
	}
	catch (std::runtime_error& e) {
		assert(std::string(e.what()) == "duplicate section name on the same level on line #3");
	}
}



#endif

#pragma endregion
