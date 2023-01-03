# Hook for Process Monitor x64
This hook blocks adding blacklist filter values, which the user can manage in  x64/Debug/blacklist.txt file.
# How to use the application?
Build it. Go to x64/Debug/program.exe and run. If the program writes "successufully attached to the proces" everything is works.
Othrewise, you have to download Deviare-InProc, build it for MVS2019, and copy the files to Deviare2 with replacing.
than open Dll1.sln and build it. 
# How to set the block for site?
You can just insert new blacklist strings to the end of file. It support wide strings. 
In the example x64/Debug/blacklist.txt it erases WinSCP.exe and explorer.exe.