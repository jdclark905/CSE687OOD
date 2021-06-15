#pragma once

#include <string>
#include <vector>

#ifdef MATTLIBRARY_EXPORTS
#define MATTLIBRARY_API __declspec(dllexport)
#else
#define MATTLIBRARY_API __declspec(dllimport)
#endif

// Exported function to get names of test funcitons
MATTLIBRARY_API std::vector<std::string> getFunctionNames(void);
// Exported functions for test cases
MATTLIBRARY_API bool testPass(void);
MATTLIBRARY_API bool testFail(void);
MATTLIBRARY_API bool testFailException(void);

// Library functions
extern "C" MATTLIBRARY_API int divide(int a, int b);