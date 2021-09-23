// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "framework.h"
#include "detours.h"
#include "stdio.h"
#include <string.h>
#include "stdarg.h"
#include "windows.h"
#include <iostream>
#include <fstream>
#include <mutex>

#pragma comment(lib,"detours.lib")
#define MAX_NUM_WRITEFILE 65535

HANDLE createheap[10000];
HANDLE freeheap[10000];

FILE* stream;

void detach();
void attach();

int num_messagea = 0;
int num_messagew = 0;

int num_createfile = 0;
int num_copyfile = 0;
int num_deletefile = 0;
int num_writefile = 0;
int num_readfile = 0;

int num_heapcreate = 0;
int num_heapdestroy = 0;
int num_heapfree = 0;

int num_regcreatekey = 0;
int num_regsetvalue = 0;
int num_regclosekey = 0;
int num_regopenkey = 0;
int num_regdeletevalue = 0;

SYSTEMTIME st;

SYSTEMTIME st_wf[MAX_NUM_WRITEFILE];

HANDLE       hFile_wf[MAX_NUM_WRITEFILE];
LPCVOID      lpBuffer_wf[MAX_NUM_WRITEFILE];
DWORD        nNumberOfBytesToWrite_wf[MAX_NUM_WRITEFILE];
LPDWORD      lpNumberOfBytesWritten_wf[MAX_NUM_WRITEFILE];
LPOVERLAPPED lpOverlapped_wf[MAX_NUM_WRITEFILE];


//勾取 MessageBoxA 和 MessageBoxW
static int(WINAPI* OldMessageBoxW)(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText,_In_opt_ LPCWSTR lpCaption, _In_ UINT uType) = MessageBoxW;
static int(WINAPI* OldMessageBoxA)(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) = MessageBoxA;

extern "C" __declspec(dllexport)int WINAPI NewMessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType)
{
    num_messagea++;

    detach();
    freopen_s(&stream, ".\\log\\out_messageboxa.txt", "a+", stdout);
    //freopen_s(&stream, "G:\\User\\Desktop\\out.txt", "a+", stdout);
    attach();

    printf("\n----------------------------------------------------\n\n");
    printf("MessageBoxA Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    printf("----------------------------------------------------\n\n\n");
    
    detach();
    fclose(stream);
    attach();

    return OldMessageBoxA(hWnd, lpText, lpCaption, uType);
}

extern "C" __declspec(dllexport)int WINAPI NewMessageBoxW(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType)
{
    num_messagew++;
    
    detach();
    freopen_s(&stream, ".\\log\\out_messageboxw.txt", "a+", stdout);
    //freopen_s(&stream, "G:\\User\\Desktop\\out.txt", "a+", stdout);
    attach();
    
    printf("\n----------------------------------------------------\n\n");
    printf("MessageBoxW Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    printf("----------------------------------------------------\n\n\n");

    detach();
    fclose(stream);
    attach();

    return OldMessageBoxW(hWnd, lpText, lpCaption, uType);
}


//勾取 CreateFile
static HANDLE(WINAPI* OldCreateFile)(
    LPCTSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile
    ) = CreateFile;
extern "C" __declspec(dllexport)HANDLE WINAPI NewCreateFile(
    LPCTSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile)
{
    num_createfile++;

    detach();
    freopen_s(&stream, ".\\log\\out_creatfile.txt", "a+", stdout);
    attach();

    printf("\n----------------------------------------------------\n\n");
    printf("CreateFile Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    int num = WideCharToMultiByte(CP_OEMCP, NULL, lpFileName, -1, NULL, 0, NULL, FALSE);
    char* pchar = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, lpFileName, -1, pchar, num, NULL, FALSE);
    std::cout << "lpFileName:\t\t" << pchar << std::endl;
    std::cout << "dwDesiredAccess:\t0x" << std::hex << dwDesiredAccess << std::endl;
    std::cout << "dwShareMode:\t\t0x" << std::hex << dwShareMode << std::endl;
    std::cout << "lpSecurityAttributes:\t0x" << std::hex << lpSecurityAttributes << std::endl;
    std::cout << "dwCreationDisposition:\t0x" << std::hex << dwCreationDisposition << std::endl;
    std::cout << "dwFlagsAndATTributes:\t0x" << std::hex << dwFlagsAndAttributes << std::endl;
    std::cout << "hTemplateFile:\t0x" << std::hex << hTemplateFile << std::endl;
    printf("----------------------------------------------------\n\n\n");
    
    detach();
    fclose(stream);
    attach();

    if (strstr(pchar, ".exe") != 0)
    {
        detach();
        freopen_s(&stream, ".\\log\\warning.txt", "a+", stdout);
        attach();

        printf("\n----------------------------------------------------\n");
        printf("Warning: 目标程序正在试图读写PE文件，已阻止！\n具体参数如下：\n");
        printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        std::cout << "lpFileName:\t\t" << pchar << std::endl;
        std::cout << "dwDesiredAccess:\t0x" << std::hex << dwDesiredAccess << std::endl;
        std::cout << "dwShareMode:\t\t0x" << std::hex << dwShareMode << std::endl;
        std::cout << "lpSecurityAttributes:\t0x" << std::hex << lpSecurityAttributes << std::endl;
        std::cout << "dwCreationDisposition:\t0x" << std::hex << dwCreationDisposition << std::endl;
        std::cout << "dwFlagsAndATTributes:\t0x" << std::hex << dwFlagsAndAttributes << std::endl;
        std::cout << "hTemplateFile:\t0x" << std::hex << hTemplateFile << std::endl;
        printf("----------------------------------------------------\n\n\n");

        detach();
        fclose(stream);
        attach();

        return NULL;
    }

    return OldCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}


//勾取 CopyFile
static BOOL(WINAPI* OldCopyFile)(
    LPCWSTR lpExistingFileName,
    LPCWSTR lpNewFileName,
    BOOL   bFailIfExists
    ) = CopyFileW;
extern "C" __declspec(dllexport)BOOL WINAPI NewCopyFile(
    LPCWSTR lpExistingFileName,
    LPCWSTR lpNewFileName,
    BOOL   bFailIfExists)
{
    num_copyfile++;

    detach();
    freopen_s(&stream, ".\\log\\out_copyfile.txt", "a+", stdout);
    attach();

    printf("\n----------------------------------------------------\n\n");
    printf("CopyFile Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    int num1 = WideCharToMultiByte(CP_OEMCP, NULL, lpExistingFileName, -1, NULL, 0, NULL, FALSE);
    char* pchar1 = new char[num1+1];
    WideCharToMultiByte(CP_OEMCP, NULL, lpExistingFileName, -1, pchar1, num1, NULL, FALSE);
    pchar1[num1] = 0;
    int num2 = WideCharToMultiByte(CP_OEMCP, NULL, lpNewFileName, -1, NULL, 0, NULL, FALSE);
    char* pchar2 = new char[num2+1];
    WideCharToMultiByte(CP_OEMCP, NULL, lpNewFileName, -1, pchar2, num2, NULL, FALSE);
    pchar2[num2] = 0;
    std::cout << "lpExistingFileName:\t" << pchar1 << std::endl;
    std::cout << "lpNewFileName:\t" << pchar2 << std::endl;
    std::cout << "bFailIfExists:\t" << bFailIfExists << std::endl;
    printf("----------------------------------------------------\n\n\n");

    detach();
    fclose(stream);
    attach();
    
    if (strstr(pchar1, "TestApp.exe") != 0)
    {
        detach();
        freopen_s(&stream, ".\\log\\warning.txt", "a+", stdout);
        attach();

        printf("\n----------------------------------------------------\n");
        printf("Warning: 目标程序正在试图复制自身，已阻止！\n具体参数如下：\n");
        printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        std::cout << "lpExistingFileName:\t" << pchar1 << std::endl;
        std::cout << "lpNewFileName:\t" << pchar2 << std::endl;
        std::cout << "bFailIfExists:\t" << bFailIfExists << std::endl;
        printf("----------------------------------------------------\n\n\n");

        detach();
        fclose(stream);
        attach();

        return NULL;
    }

    return OldCopyFile(lpExistingFileName, lpNewFileName, bFailIfExists);
}



//勾取 DeleteFile
static BOOL(WINAPI* OldDeleteFile)(LPCWSTR lpFileName) = DeleteFileW;
extern "C" __declspec(dllexport)BOOL WINAPI NewDeleteFile(LPCWSTR lpFileName)
{
    num_deletefile++;

    detach();
    //freopen_s(&stream, "G:\\User\\Desktop\\out.txt", "a+", stdout);
    freopen_s(&stream, ".\\log\\out_deletefile.txt", "a+", stdout);
    attach();

    printf("\n----------------------------------------------------\n\n");
    printf("DeleteFile Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    int num1 = WideCharToMultiByte(CP_OEMCP, NULL, lpFileName, -1, NULL, 0, NULL, FALSE);
    char* pchar1 = new char[num1];
    WideCharToMultiByte(CP_OEMCP, NULL, lpFileName, -1, pchar1, num1, NULL, FALSE);
    std::cout << "lpFileName:\t" << pchar1 << std::endl;
    printf("----------------------------------------------------\n\n\n");

    detach();
    fclose(stream);
    attach();

    return OldDeleteFile(lpFileName);
}


//勾取 WriteFile
static BOOL(WINAPI* OldWriteFile)(
    HANDLE       hFile,
    LPCVOID      lpBuffer,
    DWORD        nNumberOfBytesToWrite,
    LPDWORD      lpNumberOfBytesWritten,
    LPOVERLAPPED lpOverlapped
    ) = WriteFile;
extern "C" __declspec(dllexport)BOOL WINAPI NewWriteFile(
    HANDLE       hFile,
    LPCVOID      lpBuffer,
    DWORD        nNumberOfBytesToWrite,
    LPDWORD      lpNumberOfBytesWritten,
    LPOVERLAPPED lpOverlapped)
{
    BOOL temp = OldWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
    if (GetFileType(hFile) == FILE_TYPE_DISK)
    {
        num_writefile++;
        detach();
        GetLocalTime(&st_wf[num_writefile]);
        hFile_wf[num_writefile] = hFile;
        lpBuffer_wf[num_writefile] = lpBuffer;
        nNumberOfBytesToWrite_wf[num_writefile] = nNumberOfBytesToWrite;
        lpNumberOfBytesWritten_wf[num_writefile] = lpNumberOfBytesWritten;
        lpOverlapped_wf[num_writefile] = lpOverlapped;
        attach();
    }

    return temp;
}


//勾取 ReadFile
static BOOL(WINAPI* OldReadFile)(
    HANDLE       hFile,
    LPVOID       lpBuffer,
    DWORD        nNumberOfBytesToRead,
    LPDWORD      lpNumberOfBytesRead,
    LPOVERLAPPED lpOverlapped
    ) = ReadFile;
extern "C" __declspec(dllexport)BOOL WINAPI NewReadFile(
    HANDLE       hFile,
    LPVOID       lpBuffer,
    DWORD        nNumberOfBytesToRead,
    LPDWORD      lpNumberOfBytesRead,
    LPOVERLAPPED lpOverlapped)
{
    num_readfile++;

    detach();
    freopen_s(&stream, ".\\log\\out_readfile.txt", "a+", stdout);
    attach();

    printf("\n----------------------------------------------------\n\n");
    printf("ReadFile Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    std::cout << "hFile:\t\t" << hFile << std::endl;
    std::cout << "lpBuffer:\t\t" << lpBuffer << std::endl;
    std::cout << "nNumberOfBytesToRead:\t" << nNumberOfBytesToRead << std::endl;
    std::cout << "lpNumberOfBytesRead:\t" << lpNumberOfBytesRead << std::endl;
    std::cout << "lpOverlapped:\t\t" << lpOverlapped << std::endl;
    printf("----------------------------------------------------\n\n\n");

    detach();
    fclose(stream);
    attach();



    return OldReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}


//勾取 HeapCreate
static HANDLE(WINAPI* OldHeapCreate)(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize) = HeapCreate;
extern"C" __declspec(dllexport)HANDLE WINAPI NewHeapCreate(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize)
{
    num_heapcreate++;

    detach();
    freopen_s(&stream, ".\\log\\out_heapcreate.txt", "a+", stdout);
    attach();

    HANDLE hHeap = OldHeapCreate(fIOoptions, dwInitialSize, dwMaximumSize);
    printf("\n----------------------------------------------------\n\n");
    printf("HeapCreate Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    std::cout << "fIOoptions:\t\t" << fIOoptions << std::endl;
    std::cout << "dwInitialSize:\t" << dwInitialSize << std::endl;
    std::cout << "dwMaximumSize:\t" << dwMaximumSize << std::endl;
    std::cout << "hHeap:\t\t" << hHeap << std::endl;
    printf("----------------------------------------------------\n\n\n");

    createheap[num_heapcreate] = hHeap;

    detach();
    fclose(stream);
    attach();

    return hHeap;
}


//勾取 HeapDestroy
static BOOL(WINAPI* OldHeapDestroy)(HANDLE hHeap) = HeapDestroy;
extern "C" __declspec(dllexport)BOOL WINAPI NewHeapDestroy(HANDLE hHeap)
{
    num_heapdestroy++;

    detach();
    freopen_s(&stream, ".\\log\\out_heapdestroy.txt", "a+", stdout);
    attach();

    printf("\n----------------------------------------------------\n\n");
    printf("HeapDestroy Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    std::cout << "hHeap:\t" << hHeap << std::endl;
    printf("----------------------------------------------------\n\n\n");

    detach();
    fclose(stream);
    attach();

    return OldHeapDestroy(hHeap);
}


//勾取 HeapFree
static BOOL(WINAPI* OldHeapFree)(HANDLE hHeap, DWORD dwFlags, _Frees_ptr_opt_ LPVOID lpMem) = HeapFree;
extern "C" __declspec(dllexport) BOOL WINAPI NewHeapFree(HANDLE hHeap, DWORD dwFlags, _Frees_ptr_opt_ LPVOID lpMem) 
{
    num_heapfree++;

    detach();
    freopen_s(&stream, ".\\log\\out_heapfree.txt", "a+", stdout);
    attach();

    printf("\n----------------------------------------------------\n\n");
    printf("HeapFree Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    std::cout << "hHeap:\t" << hHeap << std::endl;
    std::cout << "dwFlags:\t" << dwFlags << std::endl;
    std::cout << "lpMem:\t" << lpMem << std::endl;
    printf("----------------------------------------------------\n\n\n");

    detach();
    fclose(stream);
    attach();

    int flag_heapfree = 0;

    for (int i = 1; i <= num_heapcreate; i++)
    {
        if (hHeap == createheap[i])
        {
            flag_heapfree = 1;
            break;
        }
    }

    if (flag_heapfree == 0)
    {
        detach();
        freopen_s(&stream, ".\\log\\warning.txt", "a+", stdout);
        attach();

        printf("\n----------------------------------------------------\n");
        printf("Warning: 目标程序释放了未声明的堆，已阻止！\n具体参数如下：\n");
        printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        std::cout << "hHeap:\t" << hHeap << std::endl;
        std::cout << "dwFlags:\t" << dwFlags << std::endl;
        std::cout << "lpMem:\t" << lpMem << std::endl;
        printf("----------------------------------------------------\n\n\n");

        detach();
        fclose(stream);
        attach();

        return NULL;
    }

    return OldHeapFree(hHeap, dwFlags, lpMem);
}


//勾取 RegCreateKey
static LSTATUS(WINAPI* OldRegCreateKeyEx)(
    HKEY                        hKey,
    LPCWSTR                     lpSubKey,
    DWORD                       Reserved,
    LPWSTR                      lpClass,
    DWORD                       dwOptions,
    REGSAM                      samDesired,
    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY                       phkResult,
    LPDWORD                     lpdwDisposition) = RegCreateKeyEx;
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegCreateKeyEx(
    HKEY                        hKey,
    LPCWSTR                     lpSubKey,
    DWORD                       Reserved,
    LPWSTR                      lpClass,
    DWORD                       dwOptions,
    REGSAM                      samDesired,
    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    PHKEY                       phkResult,
    LPDWORD                     lpdwDisposition)
{
    num_regcreatekey++;

    detach();
    freopen_s(&stream, ".\\log\\out_regcreatekey.txt", "a+", stdout);
    attach();

    int num1 = WideCharToMultiByte(CP_OEMCP, NULL, lpSubKey, -1, NULL, 0, NULL, FALSE);
    char* pchar1 = new char[num1];
    WideCharToMultiByte(CP_OEMCP, NULL, lpSubKey, -1, pchar1, num1, NULL, FALSE);
    printf("\n----------------------------------------------------\n\n");
    printf("RegCreateKey Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    std::cout << "hKey:\t\t" << hKey << std::endl;
    std::cout << "lpSubKey:\t\t" << pchar1 << std::endl;
    std::cout << "Reserved:\t\t" << Reserved << std::endl;
    std::cout << "lpClass:\t\t" << lpClass << std::endl;
    std::cout << "dwOptions:\t\t" << dwOptions << std::endl;
    std::cout << "samDesired:\t\t" << samDesired << std::endl;
    std::cout << "lpSecurityAttributes:\t" << lpSecurityAttributes << std::endl;
    std::cout << "phkResult:\t\t" << phkResult << std::endl;
    std::cout << "lpdwDisposition:\t" << lpdwDisposition << std::endl;
    printf("----------------------------------------------------\n\n\n");

    detach();
    fclose(stream);
    attach();

    return OldRegCreateKeyEx(hKey, lpSubKey, Reserved, lpClass, dwOptions, samDesired, lpSecurityAttributes, phkResult, lpdwDisposition);
}


//勾取 RegSetValueEx
static LSTATUS(WINAPI* OldRegSetValueEx)(
    HKEY       hKey,
    LPCWSTR    lpValueName,
    DWORD      Reserved,
    DWORD      dwType,
    const BYTE* lpData,
    DWORD      cbData
    ) = RegSetValueEx;
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegSetValueEx(
    HKEY       hKey,
    LPCWSTR    lpValueName,
    DWORD      Reserved,
    DWORD      dwType,
    const BYTE * lpData,
    DWORD      cbData)
{
    num_regsetvalue++;

    detach();
    freopen_s(&stream, ".\\log\\out_regsetvalue.txt", "a+", stdout);
    attach();

    int num1 = WideCharToMultiByte(CP_OEMCP, NULL, lpValueName, -1, NULL, 0, NULL, FALSE);
    char* pchar1 = new char[num1];
    WideCharToMultiByte(CP_OEMCP, NULL, lpValueName, -1, pchar1, num1, NULL, FALSE);
    int num2 = WideCharToMultiByte(CP_OEMCP, NULL, (LPCWCH)lpData, -1, NULL, 0, NULL, FALSE);
    char* pchar2 = new char[num2];
    WideCharToMultiByte(CP_OEMCP, NULL, (LPCWCH)lpData, -1, pchar2, num2, NULL, FALSE);
    printf("\n----------------------------------------------------\n\n");
    printf("RegSetValueEx Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    std::cout << "hKey:\t" << hKey << std::endl;
    std::cout << "lpValueName:\t" << pchar1 << std::endl;
    std::cout << "Reserved:\t" << Reserved << std::endl;
    std::cout << "dwType:\t" << dwType << std::endl;
    std::cout << "lpData:\t" << pchar2 << std::endl;
    std::cout << "cbData:\t" << cbData << std::endl;
    printf("----------------------------------------------------\n\n\n");

    detach();
    fclose(stream);
    attach();

    detach();
    freopen_s(&stream, ".\\log\\warning.txt", "a+", stdout);
    attach();

    printf("\n----------------------------------------------------\n");
    printf("Warning: 目标程序正在修改注册表，已放行！\n具体参数如下：\n");
    printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    std::cout << "hKey:\t" << hKey << std::endl;
    std::cout << "lpValueName:\t" << pchar1 << std::endl;
    std::cout << "Reserved:\t" << Reserved << std::endl;
    std::cout << "dwType:\t" << dwType << std::endl;
    std::cout << "lpData:\t" << pchar2 << std::endl;
    std::cout << "cbData:\t" << cbData << std::endl;
    printf("----------------------------------------------------\n\n\n");

    detach();
    fclose(stream);
    attach();

    return OldRegSetValueEx(hKey, lpValueName, Reserved, dwType, lpData, cbData);
}


//勾取 RegCloseKey
static LSTATUS(WINAPI* OldRegCloseKey)(HKEY hKey) = RegCloseKey;
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegCloseKey(HKEY hKey)
{
    num_regclosekey++;

    detach();
    freopen_s(&stream, ".\\log\\out_regclosekey.txt", "a+", stdout);
    attach();

    printf("\n----------------------------------------------------\n\n");
    printf("RegCloseKey Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    std::cout << "hKey:\t" << hKey << std::endl;
    printf("----------------------------------------------------\n\n\n");

    detach();
    fclose(stream);
    attach();

    return OldRegCloseKey(hKey);
}


//勾取 RegOpenKeyEx
static LSTATUS(WINAPI* OldRegOpenKeyEx)(
    HKEY    hKey,
    LPCWSTR lpSubKey,
    DWORD   ulOptions,
    REGSAM  samDesired,
    PHKEY   phkResult
    ) = RegOpenKeyEx;
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegOpenKeyEx(
    HKEY    hKey,
    LPCWSTR lpSubKey,
    DWORD   ulOptions,
    REGSAM  samDesired,
    PHKEY   phkResult)
{
    num_regopenkey++;

    detach();
    freopen_s(&stream, ".\\log\\out_regopenkey.txt", "a+", stdout);
    attach();

    int num1 = WideCharToMultiByte(CP_OEMCP, NULL, lpSubKey, -1, NULL, 0, NULL, FALSE);
    char* pchar1 = new char[num1];
    WideCharToMultiByte(CP_OEMCP, NULL, lpSubKey, -1, pchar1, num1, NULL, FALSE);
    printf("\n----------------------------------------------------\n\n");
    printf("RegOpenKeyEx Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    std::cout << "hKey:\t" << hKey << std::endl;
    std::cout << "lpSubKey:\t" << pchar1 << std::endl;
    std::cout << "ulOptions:\t" << ulOptions << std::endl;
    std::cout << "samDesired:\t" << samDesired << std::endl;
    std::cout << "phkResult:\t" << phkResult << std::endl;
    printf("----------------------------------------------------\n\n\n");

    detach();
    fclose(stream);
    attach();

    if (strstr(pchar1, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run")!=0)
    {
        detach();
        freopen_s(&stream, ".\\log\\warning.txt", "a+", stdout);
        attach();

        printf("\n----------------------------------------------------\n");
        printf("Warning: 目标程序正在修改开机启动项，已阻止！\n具体参数如下：\n");
        printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
        std::cout << "hKey:\t" << hKey << std::endl;
        std::cout << "lpSubKey:\t" << pchar1 << std::endl;
        std::cout << "ulOptions:\t" << ulOptions << std::endl;
        std::cout << "samDesired:\t" << samDesired << std::endl;
        std::cout << "phkResult:\t" << phkResult << std::endl;
        printf("----------------------------------------------------\n\n\n");

        detach();
        fclose(stream);
        attach();

        return NULL;
    }

    return OldRegOpenKeyEx(hKey, lpSubKey, ulOptions, samDesired, phkResult);
}


//勾取 RegDeleteValue
static LSTATUS(WINAPI* OldRegDeleteValue)(
    HKEY    hKey,
    LPCWSTR lpValueName
    ) = RegDeleteValue;
extern "C" __declspec(dllexport)LSTATUS WINAPI NewRegDeleteValue(
    HKEY    hKey,
    LPCWSTR lpValueName)
{
    num_regdeletevalue++;

    detach();
    freopen_s(&stream, ".\\log\\out_regdeletevalue.txt", "a+", stdout);
    attach();

    int num1 = WideCharToMultiByte(CP_OEMCP, NULL, lpValueName, -1, NULL, 0, NULL, FALSE);
    char* pchar1 = new char[num1];
    WideCharToMultiByte(CP_OEMCP, NULL, lpValueName, -1, pchar1, num1, NULL, FALSE);
    printf("\n----------------------------------------------------\n\n");
    printf("RegDeleteValue Hooked\n");
    GetLocalTime(&st);
    printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    std::cout << "hKey:\t" << hKey << std::endl;
    std::cout << "lpValueName:\t" << pchar1 << std::endl;
    printf("----------------------------------------------------\n\n\n");

    detach();
    fclose(stream);
    attach();

    return OldRegDeleteValue(hKey, lpValueName);
}


void detach()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourDetach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);
    DetourDetach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);

    DetourDetach(&(PVOID&)OldCreateFile, NewCreateFile);
    DetourDetach(&(PVOID&)OldCopyFile, NewCopyFile);
    DetourDetach(&(PVOID&)OldDeleteFile, NewDeleteFile);
    DetourDetach(&(PVOID&)OldReadFile, NewReadFile);
    DetourDetach(&(PVOID&)OldWriteFile, NewWriteFile);

    DetourDetach(&(PVOID&)OldHeapCreate, NewHeapCreate);
    DetourDetach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
    DetourDetach(&(PVOID&)OldHeapFree, NewHeapFree);

    DetourDetach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
    DetourDetach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
    DetourDetach(&(PVOID&)OldRegDeleteValue, NewRegDeleteValue);
    DetourDetach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);
    DetourDetach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);

    DetourTransactionCommit();
}

void attach()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourAttach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);
    DetourAttach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);

    DetourAttach(&(PVOID&)OldCreateFile, NewCreateFile);
    DetourAttach(&(PVOID&)OldCopyFile, NewCopyFile);
    DetourAttach(&(PVOID&)OldDeleteFile, NewDeleteFile);
    DetourAttach(&(PVOID&)OldReadFile, NewReadFile);
    DetourAttach(&(PVOID&)OldWriteFile, NewWriteFile);

    DetourAttach(&(PVOID&)OldHeapCreate, NewHeapCreate);
    DetourAttach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
    DetourAttach(&(PVOID&)OldHeapFree, NewHeapFree);

    DetourAttach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
    DetourAttach(&(PVOID&)OldRegCreateKeyEx, NewRegCreateKeyEx);
    DetourAttach(&(PVOID&)OldRegDeleteValue, NewRegDeleteValue);
    DetourAttach(&(PVOID&)OldRegOpenKeyEx, NewRegOpenKeyEx);
    DetourAttach(&(PVOID&)OldRegSetValueEx, NewRegSetValueEx);

    DetourTransactionCommit();
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    system("mkdir .\\log\\");

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        freopen_s(&stream, ".\\log\\out_copyfile.txt", "w+", stdout);
        freopen_s(&stream, ".\\log\\out_creatfile.txt", "w+", stdout);
        freopen_s(&stream, ".\\log\\out_deletefile.txt", "w+", stdout);
        freopen_s(&stream, ".\\log\\out_heapcreate.txt", "w+", stdout);
        freopen_s(&stream, ".\\log\\out_heapdestroy.txt", "w+", stdout);
        freopen_s(&stream, ".\\log\\out_heapfree.txt", "w+", stdout);
        freopen_s(&stream, ".\\log\\out_messageboxa.txt", "w+", stdout);
        freopen_s(&stream, ".\\log\\out_messageboxw.txt", "w+", stdout);
        freopen_s(&stream, ".\\log\\out_readfile.txt", "w+", stdout);
        freopen_s(&stream, ".\\log\\out_regclosekey.txt", "w+", stdout);
        freopen_s(&stream, ".\\log\\out_regcreatekey.txt", "w+", stdout);
        freopen_s(&stream, ".\\log\\out_regdeletevalue.txt", "w+", stdout);
        freopen_s(&stream, ".\\log\\out_regopenkey.txt", "w+", stdout);
        freopen_s(&stream, ".\\log\\out_regsetvalue.txt", "w+", stdout);
        freopen_s(&stream, ".\\log\\out_writefile.txt", "w+", stdout);
        freopen_s(&stream, ".\\log\\warning.txt", "w+", stdout);

        fclose(stream);
        DisableThreadLibraryCalls(hModule);
        attach();
    }
    break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        detach();

        int temp_wf = num_writefile;
        for (int i = 1; i <= num_writefile; i++)
        {
            freopen_s(&stream, ".\\log\\out_writefile.txt", "a+", stdout);
            printf("\n----------------------------------------------------\n\n");
            printf("WriteFile Hooked\n");
            printf("DLL日志输出: \t%d-%d-%d %02d: %02d: %02d: %03d\n\n", st_wf[i].wYear, st_wf[i].wMonth, st_wf[i].wDay, st_wf[i].wHour, st_wf[i].wMinute, st_wf[i].wSecond, st_wf[i].wMilliseconds);
            std::cout << "hFile:\t\t" << hFile_wf[i] << std::endl;
            std::cout << "lpBuffer:\t\t" << lpBuffer_wf[i] << std::endl;
            std::cout << "nNumberOfBytesToWrite:\t" << nNumberOfBytesToWrite_wf[i] << std::endl;
            std::cout << "lpNumberOfBytesWritten:\t" << lpNumberOfBytesWritten_wf[i] << std::endl;
            std::cout << "lpOverlapped:\t\t" << lpOverlapped_wf[i] << std::endl;
            printf("----------------------------------------------------\n\n\n");
            fclose(stream);
        }
        
        freopen_s(&stream, ".\\log\\num.txt", "w+", stdout);

        std::cout << "Number of MessageboxA:\t" << std::oct << num_messagea << std::endl;
        std::cout << "Number of MessageboxW:\t" << std::oct << num_messagew << std::endl;

        std::cout << "Number of CreateFile:\t" << std::oct << num_createfile << std::endl;
        std::cout << "Number of CopyFile:\t" << std::oct << num_copyfile << std::endl;
        std::cout << "Number of DeleteFile:\t" << std::oct << num_deletefile << std::endl;
        std::cout << "Number of ReadFile:\t " << std::oct << num_readfile << std::endl;
        std::cout << "Number of WriteFile:\t" << std::oct << num_writefile << std::endl;

        std::cout << "Number of HeapCreate:\t" << std::oct << num_heapcreate << std::endl;
        std::cout << "Number of HeapDestroy:\t" << std::oct << num_heapdestroy << std::endl;
        std::cout << "Number of HeapFree:\t" << std::oct << num_heapfree << std::endl;

        std::cout << "Number of RegCloseKey:\t" << std::oct << num_regclosekey << std::endl;
        std::cout << "Number of RegCreateKeyEx:\t" << std::oct << num_regcreatekey << std::endl;
        std::cout << "Number of RegDeleteValue:\t" << std::oct << num_regdeletevalue << std::endl;
        std::cout << "Number of RegOpenKeyEx:\t" << std::oct << num_regopenkey << std::endl;
        std::cout << "Number of RegSetValueEx:\t" << std::oct << num_regsetvalue << std::endl;

        fclose(stream);
        break;
    }

    
    return TRUE;
 }

