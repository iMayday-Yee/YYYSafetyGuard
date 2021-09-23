
// TestDlg.h: 头文件
//

#pragma once


// CTestDlg 对话框
class CTestDlg : public CDialogEx
{
// 构造
public:
	CTestDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTestmessagea();
	afx_msg void OnBnClickedTestmessagew();
	afx_msg void OnBnClickedTestcreatefile();
	afx_msg void OnBnClickedCopyfile();
	afx_msg void OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedTestcreatefile2();
	afx_msg void OnBnClickedTestdeletefile();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedTestreadfile();
	afx_msg void OnBnClickedTestwritefile();
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedTestheapcreate();
	afx_msg void OnBnClickedTestheapdestroy();
	afx_msg void OnBnClickedTestheapfree();
	afx_msg void OnBnClickedTestclosekey();
	afx_msg void OnBnClickedTestcreatekey();
	afx_msg void OnBnClickedTestopenkey();
	afx_msg void OnBnClickedWarning1();
	afx_msg void OnBnClickedWarning2();
	afx_msg void OnBnClickedWarning3();
	afx_msg void OnBnClickedWarning4();
	afx_msg void OnBnClickedWarning5();
};
