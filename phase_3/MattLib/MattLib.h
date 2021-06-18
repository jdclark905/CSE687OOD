#pragma once

#include <string>
#include <vector>

#ifdef MATTLIBRARY_EXPORTS
#define MATTLIBRARY_API __declspec(dllexport)
#else
#define MATTLIBRARY_API __declspec(dllimport)
#endif
	MATTLIBRARY_API std::vector<std::string> getTestNames();

#ifdef __cplusplus
extern "C" {
#endif
	// Exported function to get names of test funcitons
	// Exported functions for test cases
	MATTLIBRARY_API bool testPass(void);
	MATTLIBRARY_API bool testFail(void);
	MATTLIBRARY_API bool testFailException(void);

	// Library functions
	MATTLIBRARY_API int divide(int a, int b);
#ifdef __cplusplus
}
#endif