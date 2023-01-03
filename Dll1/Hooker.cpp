#pragma once

#include "pch.h"
#include "Hooker.h"
#include "StringAllocationManager.h"
//Allow calls to the Deviare library--------------------------
#include <NktHookLib.h>

#pragma comment(lib, "NktHookLib64")

//-----------------------------------------------------------

#define WINDOW_MANAGER_OFFSET static_cast<long long> (0x516F0)
#define STRING_ALLOCATION_OFFSET static_cast<long long> (0xACEB0)

#define STATUS_SUCCESS (static_cast<NTSTATUS>(0x00000000L))

#define NEW_FILTER_ADD_CODE static_cast<int>(273)

// Hook manager. I use it for set and unset the hooks
static CNktHookLib cHookMgr;

// String Allocation manager. I use it for control the string allocation
StringAllocationManager gSAllocMgr;


// this part is related to StringAllocation function hook


// Getting the necessary address of the connect function.
typedef __int64(__fastcall* _salloc)(
    void* _src,
    size_t _maxCount
);

_salloc gTrueSAlloc;

static struct
{
    SIZE_T nHookId;
    _salloc trueSAlloc;
} sSAllocHook = { 0, NULL };



// This method is the hook for connect method
__int64 __fastcall HookSAlloc(
    void* _src,
    size_t _maxCount
) 
{
    try 
    {
        // if the string is empty skip the hook
        if (_src == NULL)
        {
            return gTrueSAlloc(_src, _maxCount);
        }

        wchar_t* pw = reinterpret_cast<wchar_t*>(_src);
        std::wstring src(pw);

        if (gSAllocMgr.winMgrCode == NEW_FILTER_ADD_CODE && gSAllocMgr.IsStringInBlackList(src))
        {
            // Erasing the filter value
            pw[0] = L'\0';
        }
    }
    catch (std::exception& ex)
    {
        OutputDebugStringW(L"Unexpected exception:");
        OutputDebugStringA(ex.what());
    }

    return gTrueSAlloc(_src, _maxCount);
}

// This method sets the Hook for StringAllocation function from Procmon64.exe
extern "C" HRESULT WINAPI HookFunctionSAlloc()
{
    cHookMgr.SetEnableDebugOutput(TRUE);
    

    // Set the hook for StingAlloc function
    HMODULE hProcmon = GetModuleHandle(L"Procmon64.exe");
    LPVOID SAlloc = reinterpret_cast<LPVOID>(reinterpret_cast<long long>(hProcmon) + STRING_ALLOCATION_OFFSET);
    gTrueSAlloc = reinterpret_cast<_salloc>(SAlloc);

    HRESULT dwOsErr = cHookMgr.Hook(&(sSAllocHook.nHookId), reinterpret_cast<LPVOID*>(&(sSAllocHook.trueSAlloc)),
        SAlloc, HookSAlloc, NKTHOOKLIB_DisallowReentrancy);
    
    return dwOsErr;
}

extern "C" HRESULT WINAPI UnhookFunctionSAlloc()
{
    const HRESULT dwOsErr = cHookMgr.Unhook(sSAllocHook.nHookId);
    return dwOsErr;
}




// This part is related to WindowManager hook

// Getting the necessary address of the window manager function.
typedef INT_PTR(__stdcall* _wmgr)(
    HWND hWindow,
    UINT intCode,
    WPARAM wparam,
    LPARAM lparam
);

_wmgr gTrueWMgr;

static struct
{
    SIZE_T nHookId;
    _wmgr trueWMgr;
} sWMgrHook = { 0, NULL };

__int64 __fastcall HookWindowManager(
    HWND hWindow,
    UINT intCode,
    WPARAM wparam,
    LPARAM lparam
)
{
    // Save the code to indicate the operation in HookSAlloc
    gSAllocMgr.winMgrCode = intCode;
    return gTrueWMgr(hWindow, intCode, wparam, lparam);
}

// This method sets the Hook for WindowManager function from Procmon64.exe
extern "C" HRESULT WINAPI HookFunctionWindowManager()
{
    cHookMgr.SetEnableDebugOutput(TRUE);

    gSAllocMgr.InitFilterBlacklist();
    
    HMODULE hm = GetModuleHandle(L"Procmon64.exe");
    long long winMgrAddress = reinterpret_cast<long long>(hm) + WINDOW_MANAGER_OFFSET;
    LPVOID WMgr = reinterpret_cast<LPVOID>(winMgrAddress);
    gTrueWMgr = reinterpret_cast<_wmgr>(WMgr);

    HRESULT dwOsErr = cHookMgr.Hook(&(sWMgrHook.nHookId), (LPVOID*)&(sWMgrHook.trueWMgr),
        WMgr, HookWindowManager, NKTHOOKLIB_DisallowReentrancy);

    return dwOsErr;
}

extern "C" HRESULT WINAPI UnhookFunctionWindowManager()
{
    const HRESULT dwOsErr = cHookMgr.Unhook(sWMgrHook.nHookId);
    return dwOsErr;
}