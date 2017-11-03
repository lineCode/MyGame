
// freetypeDemoDlg.h : 头文件
//

#pragma once
#include "FCharacterEntry.h"
#include <map>
#include <string>
using namespace std;
// CfreetypeDemoDlg 对话框
class CfreetypeDemoDlg : public CDialogEx
{
// 构造
public:
	CfreetypeDemoDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CfreetypeDemoDlg();
// 对话框数据
	enum { IDD = IDD_FREETYPEDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	void DrawString(const wstring& strText, HDC hdc);
	void GetRenderData(FCharacterEntry& InFontData, const TCHAR Char);
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClose();
public:


	FT_Library library;

	FT_Face face;
	std::map <wchar_t, FCharacterEntry >MappedEntries;
};
