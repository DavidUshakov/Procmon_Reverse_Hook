#pragma once

#include <vector>

class StringAllocationManager
{
public:
    friend __int64 __fastcall HookSAlloc(void* Src, size_t MaxCount);
    friend __int64 __fastcall HookWindowManager(HWND hWindow, UINT intCode, WPARAM wparam, LPARAM lparam);
    bool InitFilterBlacklist();
    bool IsStringInBlackList(std::wstring& _str);

private:
    int winMgrCode;
    std::vector<std::wstring> filterBlacklist;
};

