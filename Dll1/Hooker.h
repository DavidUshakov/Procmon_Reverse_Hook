#pragma once

__int64 __fastcall HookSAlloc(
    void* _src,
    size_t _maxCount
);

__int64 __fastcall HookWindowManager(
    HWND hWindow,
    UINT intCode,
    WPARAM wparam,
    LPARAM lparam
);

//-----------------------------------------------------------
extern "C" HRESULT WINAPI HookFunctionSAlloc();
extern "C" HRESULT WINAPI UnhookFunctionSAlloc();
extern "C" HRESULT WINAPI HookFunctionWindowManager();
extern "C" HRESULT WINAPI UnhookFunctionWindowManager();
//-----------------------------------------------------------



