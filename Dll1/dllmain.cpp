#pragma once
// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Hooker.h"
#include <exception>


#include "windows.h"
#include "tchar.h"
#include "stdio.h"
#include "psapi.h"



extern "C" BOOL APIENTRY DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,
    LPVOID lpvReserved)
{
    try {
        switch (fdwReason)
        {
        case DLL_PROCESS_ATTACH:
        {
            system("pause");

            HRESULT hookErr = HookFunctionWindowManager();
            if (SUCCEEDED(hookErr))
            {
                OutputDebugStringW(L"Dll1.dll:DllMain - DLL_PROCESS_ATTACH: Successfully set hook CreateProcess");
            }
            else
            {
                OutputDebugStringW(L"Dll1.dll:DllMain - DLL_PROCESS_ATTACH: Error: Failed to set hook CreateProcess.");
            }

            hookErr = HookFunctionSAlloc();

            if (SUCCEEDED(hookErr))
            {
                OutputDebugStringW(L"Dll1.dll:DllMain - DLL_PROCESS_ATTACH: Successfully set hook String allocation");
            }
            else
            {
                OutputDebugStringW(L"Dll1.dll:DllMain - DLL_PROCESS_ATTACH: Error: Failed to set hook String allocation.");
            }
            
        }
        break;
        case DLL_PROCESS_DETACH:
            HRESULT unhookError = UnhookFunctionWindowManager();
            if (SUCCEEDED(unhookError))
            {
                OutputDebugStringW(L"DeviarePlugin.dll:DllMain - DLL_PROCESS_DETACH: Successfully unhook CreateProcess");
            }
            else
            {
                OutputDebugStringW(L"DeviarePlugin.dll:DllMain - DLL_PROCESS_DETACH: Error: Failed to unhook CreateProcess.");
            }

            unhookError = UnhookFunctionSAlloc();
            if (SUCCEEDED(unhookError))
            {
                OutputDebugStringW(L"DeviarePlugin.dll:DllMain - DLL_PROCESS_DETACH: Successfully unhook String allocation");
            }
            else
            {
                OutputDebugStringW(L"DeviarePlugin.dll:DllMain - DLL_PROCESS_DETACH: Error: Failed to unhook String allocation.");
            }
            break;
        }
    }
    catch (std::exception& ex)
    {
        OutputDebugStringW(L"Unexpected exception:");
        OutputDebugStringA(ex.what());
    }
    return TRUE;
}






