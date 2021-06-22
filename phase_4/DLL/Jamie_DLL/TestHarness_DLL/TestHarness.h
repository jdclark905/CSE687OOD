#pragma once
#include <vector>
#include <string>
#ifdef TESTHARNESS_EXPORTS
#define TESTHARNESS_API __declspec(dllexport)
#else
#define TESTHARNESS_API __declspec(dllimport)
#endif

TESTHARNESS_API std::vector<std::string> getTestNames();
extern "C" TESTHARNESS_API bool beTrue(int x);
extern "C" TESTHARNESS_API bool beFalse(int x);
extern "C" TESTHARNESS_API bool beEqual(int x);

extern "C" TESTHARNESS_API bool beTrueTest();
extern "C" TESTHARNESS_API bool beFalseTest();
extern "C" TESTHARNESS_API bool beEqualTest();