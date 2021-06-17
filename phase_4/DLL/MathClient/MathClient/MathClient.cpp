// MathClient.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <vector>
#include <string>
#include "windows.h"

typedef bool(CALLBACK* LPFNDLLFUNC1)();

int main()
{
    std::cout << "Loading dll...\n";

    HINSTANCE hDLL;     // Handle to DLL
    LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer
    LPFNDLLFUNC1 lpfnDllFunc2;    // Function pointer
    LPFNDLLFUNC1 lpfnDllFunc3;    // Function pointer
    
    std::string ERROR_;

    hDLL = LoadLibrary(TEXT("MathLibrary.dll"));

    std::cout << "Loading complete.\n";

    // storing function names
    std::vector<LPCSTR> functions_;
    functions_.push_back("mathFunc1");
    functions_.push_back("mathFunc2");
    functions_.push_back("mathFunc3");

    // store results
    std::vector<bool> results_;

    if (NULL != hDLL)
    {
        lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL, functions_[0]);
        lpfnDllFunc2 = (LPFNDLLFUNC1)GetProcAddress(hDLL, functions_[1]);
        lpfnDllFunc3 = (LPFNDLLFUNC1)GetProcAddress(hDLL, functions_[2]);

        if (NULL != lpfnDllFunc1)
        {
            // call the function and store results
            results_.push_back(lpfnDllFunc1());
            results_.push_back(lpfnDllFunc2());
            results_.push_back(lpfnDllFunc3());
        }
        else
        {
            // report the error
            std::cout << "Error: Function not loaded.\n";
            ERROR_ = "ERROR_DELAY_LOAD_FAILED";
        }
        
    }
    else
    {
        std::cout << "Error: Library not loaded.\n";
        ERROR_ = "ERROR_DELAY_LOAD_FAILED";
    }

    if (ERROR_ == "ERROR_DELAY_LOAD_FAILED")
    {
        std::cout << "ERROR: Please check the library or all defined library functions being called.\n\n";
    }
    else
    {
        for (auto result : results_)
        {
            std::cout << "Result = " << result << "\n";
        }

        if (NULL != hDLL)
        {
            FreeLibrary(hDLL);
        }

    }

}
