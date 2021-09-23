
// TestDlg.cpp: 实现文件
//

#include "pch.h"
#include <Windows.h>
#include "framework.h"
#include "Test.h"
#include "TestDlg.h"
#include "afxdialogex.h"

#define PAGE_SIZE	4096

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestDlg 对话框



CTestDlg::CTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(TESTMESSAGEA, &CTestDlg::OnBnClickedTestmessagea)
	ON_BN_CLICKED(TESTMESSAGEW, &CTestDlg::OnBnClickedTestmessagew)
	ON_BN_CLICKED(TESTCREATEFILE, &CTestDlg::OnBnClickedTestcreatefile)
	ON_BN_CLICKED(COPYFILE, &CTestDlg::OnBnClickedCopyfile)
	ON_BN_CLICKED(TESTDELETEFILE, &CTestDlg::OnBnClickedTestdeletefile)
	ON_BN_CLICKED(TESTREADFILE, &CTestDlg::OnBnClickedTestreadfile)
	ON_BN_CLICKED(TESTWRITEFILE, &CTestDlg::OnBnClickedTestwritefile)
	ON_BN_CLICKED(IDEXIT, &CTestDlg::OnBnClickedExit)
	ON_BN_CLICKED(TESTHEAPDESTROY, &CTestDlg::OnBnClickedTestheapdestroy)
	ON_BN_CLICKED(TESTHEAPFREE, &CTestDlg::OnBnClickedTestheapfree)
	ON_BN_CLICKED(TESTCREATEKEY, &CTestDlg::OnBnClickedTestcreatekey)
	ON_BN_CLICKED(TESTOPENKEY, &CTestDlg::OnBnClickedTestopenkey)
	ON_BN_CLICKED(WARNING1, &CTestDlg::OnBnClickedWarning1)
	ON_BN_CLICKED(WARNING2, &CTestDlg::OnBnClickedWarning2)
	ON_BN_CLICKED(WARNING3, &CTestDlg::OnBnClickedWarning3)
	ON_BN_CLICKED(WARNING4, &CTestDlg::OnBnClickedWarning4)
	ON_BN_CLICKED(WARNING5, &CTestDlg::OnBnClickedWarning5)
END_MESSAGE_MAP()


// CTestDlg 消息处理程序

BOOL CTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestDlg::OnBnClickedTestmessagea()
{
	MessageBoxA(0, "测试MessageBoxA", "Test", 0);
}


void CTestDlg::OnBnClickedTestmessagew()
{
	::MessageBoxW(0, L"测试MessageBoxW", L"Test", 0);
}


void CTestDlg::OnBnClickedTestcreatefile()
{
	CreateFileA("G:\\User\\Desktop\\Create.txt", GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_NEW, NULL, NULL);
}



void CTestDlg::OnBnClickedCopyfile()
{
	CopyFileW(L"G:\\User\\Desktop\\Create.txt", L"G:\\User\\Desktop\\Copy.txt", 0);
}



void CTestDlg::OnBnClickedTestdeletefile()
{
	DeleteFile(L"G:\\User\\Desktop\\Copy.txt");
}



void CTestDlg::OnBnClickedTestreadfile()
{
	CHAR* pBuffer;
	int fileSize = 0;
	bool flag;
	HANDLE hOpenFile = (HANDLE)CreateFile(L"G:\\User\\Desktop\\Read.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, NULL, NULL);
	fileSize = GetFileSize(hOpenFile, NULL);
	pBuffer = (char*)malloc((fileSize + 1) * sizeof(char));
	flag = ReadFile(hOpenFile, pBuffer, fileSize, NULL, NULL);
	pBuffer[fileSize] = 0;
	free(pBuffer);
	CloseHandle(hOpenFile);
}



void CTestDlg::OnBnClickedTestwritefile()
{
	CHAR* pBuffer;
	int fileSize = 0;
	char writeString[100];
	bool flag;
	HANDLE hOpenFile = (HANDLE)CreateFile(L"G:\\User\\Desktop\\Write.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	flag = WriteFile(hOpenFile, "Hello World!", strlen(writeString), NULL, NULL);
	FlushFileBuffers(hOpenFile);
	CloseHandle(hOpenFile);
}



void CTestDlg::OnBnClickedTestheapdestroy()
{
	HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, PAGE_SIZE * 10, PAGE_SIZE * 100);
	int* pArr = (int*)HeapAlloc(hHeap, 0, sizeof(int) * 30);
	for (int i = 0; i < 30; ++i)
		pArr[i] = i;
	HeapDestroy(hHeap);
}



void CTestDlg::OnBnClickedTestheapfree()
{
	HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, PAGE_SIZE * 10, PAGE_SIZE * 100);
	int* pArr = (int*)HeapAlloc(hHeap, 0, sizeof(int) * 30);
	for (int i = 0; i < 30; ++i)
		pArr[i] = i;
	HeapFree(hHeap, 0, pArr);
}



void CTestDlg::OnBnClickedTestcreatekey()
{
	HKEY hKey = NULL;
	TCHAR Data[254];
	memset(Data, 0, sizeof(Data));
	wcsncpy_s(Data, TEXT("Yu Yongyue"), 254);
	size_t lRet = RegCreateKeyEx(HKEY_CURRENT_USER, (LPWSTR)L"TestKey", 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &hKey, NULL);
	size_t iLen = wcslen(Data);
	// 设置键值
	lRet = RegSetValueEx(hKey, L"YYYSetTest", 0, REG_SZ, (CONST BYTE*)Data, sizeof(TCHAR) * iLen);
	RegCloseKey(hKey);
}



void CTestDlg::OnBnClickedTestopenkey()
{
	HKEY hKey = NULL;
	size_t lRet = RegOpenKeyEx(HKEY_CURRENT_USER, (LPWSTR)L"TestKey", 0, KEY_ALL_ACCESS, &hKey);
	lRet = RegDeleteValue(hKey, L"YYYSetTest");
	RegCloseKey(hKey);
}



void CTestDlg::OnBnClickedWarning1()
{
	CopyFileW(L"TestApp.exe", L"G:\\User\\Desktop\\TestApp.exe", 0);
}


void CTestDlg::OnBnClickedWarning2()
{
	HKEY hKey = NULL;
	TCHAR Data[254];
	memset(Data, 0, sizeof(Data));
	wcsncpy_s(Data, TEXT("YuYongyue2"), 254);
	size_t lRet = RegCreateKeyEx(HKEY_CURRENT_USER, (LPWSTR)L"TestKey2", 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &hKey, NULL);
	size_t iLen = wcslen(Data);
	lRet = RegSetValueEx(hKey, L"YYYSetTest2", 0, REG_SZ, (CONST BYTE*)Data, sizeof(TCHAR) * iLen);
	RegCloseKey(hKey);
}


void CTestDlg::OnBnClickedWarning3()
{
	HKEY hKey = NULL;
	size_t lRet = RegOpenKeyEx(HKEY_CURRENT_USER, (LPWSTR)L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey);
	RegCloseKey(hKey);
}


void CTestDlg::OnBnClickedWarning4()
{
	CreateFileA("G:\\User\\Desktop\\Warning.exe", GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_NEW, NULL, NULL);
}


void CTestDlg::OnBnClickedWarning5()
{
	int a = 9999;
	HeapFree((HANDLE)0x6666, 0, &a);
}


void CTestDlg::OnBnClickedExit()
{
	exit(0);
}