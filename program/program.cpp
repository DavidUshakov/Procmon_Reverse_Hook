#include <iostream>
#include <string>
#include <Windows.h>


//Allow calls to the Deviare library-------------------------
#include <NktHookLib.h>
#pragma comment(lib, "NktHookLib64")
//-----------------------------------------------------------

int __CRTDECL wmain(__in int argc, __in wchar_t* argv[])
{
    if (argc != 2)
    {
        std::wcout << L"Error: invalid input parameters." << std::endl;
        system("pause");
        return 1;
    }

    const std::wstring dllToInjectNameW = L"Dll1.dll";

    // Create process with out custom DLL that hooks APIs
    STARTUPINFOW sSiW{};
    sSiW.cb = static_cast<DWORD>(sizeof(sSiW));
    PROCESS_INFORMATION sPi{};
    // The example of the path:
    // "C:\\Program Files\\Procmon64.exe"
    std::wstring firefoxPath  = argv[1];
    const DWORD dwOsErr = NktHookLibHelpers::CreateProcessWithDllW(firefoxPath.c_str(), NULL, NULL, NULL, FALSE,
        0, NULL, NULL, &sSiW, &sPi, dllToInjectNameW.c_str(), NULL, NULL); 
    if (dwOsErr != ERROR_SUCCESS)
    {
        std::wcout << L"Error: " << dwOsErr << L" - Cannot launch process and inject dll." << std::endl;
        CloseHandle(sPi.hThread);
        CloseHandle(sPi.hProcess);
        system("pause");
        return 1;
    }

    std::wcout << L"InjectDLL: Process #" << sPi.dwProcessId << L" successfully launched with dll injected!" << std::endl;

    CloseHandle(sPi.hThread);
    CloseHandle(sPi.hProcess);
    
    return 0;
}
