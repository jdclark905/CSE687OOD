#pragma once

#include "..\TestHarness\Test.h"

#ifdef MATTLIBRARY_EXPORTS
#define MATTLIBRARY_API __declspec(dllexport)
#else
#define MATTLIBRARY_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	// Exported function to get names of test funcitons
	MATTLIBRARY_API Test* getTests();

	// Fibonacci sequence
	MATTLIBRARY_API int fibonacci(int n);

	// Calculate expected fibonacci sequence value using golden ratio
	MATTLIBRARY_API double calculateFibonacci(int n);
#ifdef __cplusplus
}
#endif