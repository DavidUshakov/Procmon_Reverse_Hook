#include "pch.h"
#include "StringAllocationManager.h"

#include <fstream>

bool StringAllocationManager::InitFilterBlacklist()
{
    try{
        std::wifstream fBlacklist("blacklist.txt");
        wchar_t tmp[100];
        std::vector<std::wstring> tmplist;
        while (!fBlacklist.eof())
        {
            fBlacklist.getline(tmp,100);
            tmplist.push_back(tmp);
        }
        filterBlacklist = tmplist;
        OutputDebugStringW(L"Dll1.dll:StringAllocationManager - InitFilterBlacklist: successfully read blacklist.txt");
        return true;
    }
    catch (std::exception& ex)
    {
        OutputDebugStringW(L"Dll1.dll:StringAllocationManager - InitFilterBlacklist: failed to read blacklist.txt");
        OutputDebugStringA(ex.what());
        return false;
    }
}

bool StringAllocationManager::IsStringInBlackList(std::wstring& _str)
{
    for (int i = 0; i < filterBlacklist.size(); ++i)
    {
        if (filterBlacklist[i] == _str)
        {
            return true;
        }
    }
    return false;
}

