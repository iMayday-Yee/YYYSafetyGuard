#include <iostream>
#include <cstdio>
#include <windows.h>
#include <detours.h>


//#include <TCHAR.H>
//typedef void(*ptrGet)(char*, int);
//typedef void(*ptrNum)(int&);
//typedef void(*pHeapAllocNum)(int&);
//HMODULE hMod = LoadLibraryA("G:\\User\\Desktop\\ruanankeshe\\YYY\\InjectDll\\Debug\\InnjectDll.dll");
//ptrGet GetSeg = (ptrGet)GetProcAddress(hMod, "GetSeg");
//ptrNum GetNum = (ptrNum)GetProcAddress(hMod, "GetNum");
//pHeapAllocNum GetAllocNum = (pHeapAllocNum)GetProcAddress(hMod, "GetHeapAllocNum");


#pragma comment(lib,"detours.lib")

WCHAR* ctowc(char* str)
{
    WCHAR* wstr = new WCHAR[2 * strlen(str) + 2];
    if (!wstr)
        return NULL;
    MultiByteToWideChar(CP_ACP, 0, str, -1, wstr, 2 * (int)strlen(str) + 2);
    return wstr;
}

int main(int argc, char* argv[])
{
    
    printf("开始注入......\n");
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    si.cb = sizeof(STARTUPINFO);
    WCHAR DirPath[MAX_PATH + 1];

    wcscpy_s(DirPath, MAX_PATH, ctowc(argv[1]));//DLL文件位置
    //wcscpy_s(DirPath, MAX_PATH, L"G:\\User\\Desktop\\ruanankeshe\\YYY\\InjectDll\\Debug");//DLL文件位置

    char DLLPath[MAX_PATH + 1];
    strcpy_s(DLLPath, argv[2]);
    //char DLLPath[MAX_PATH + 1] = "G:\\User\\Desktop\\ruanankeshe\\YYY\\InjectDll\\Debug\\InnjectDll.dll";
    
    WCHAR EXE[MAX_PATH + 1] = { 0 };
    
    wcscpy_s(EXE, MAX_PATH, ctowc(argv[3]));
    //wcscpy_s(EXE, MAX_PATH, L"G:\\User\\Desktop\\ruanankeshe\\YYY\\InjectDll\\Debug\\TestApp.exe");

    if (DetourCreateProcessWithDllEx(EXE, NULL, NULL,NULL,TRUE, CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, DirPath, &si, &pi, DLLPath, NULL))
    {
        printf("注入成功！\n");
        ResumeThread(pi.hThread);
        WaitForSingleObject(pi.hProcess, INFINITE);
    }
    else
    {
        char error[100];
        sprintf_s(error, "%d", GetLastError());
    }
    return 0;

}