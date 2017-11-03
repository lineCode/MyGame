
// freetypeDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "FCharacterEntry.h"
#include <map>
#include <string>
using namespace std;
// CfreetypeDemoDlg �Ի���
class CfreetypeDemoDlg : public CDialogEx
{
// ����
public:
	CfreetypeDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CfreetypeDemoDlg();
// �Ի�������
	enum { IDD = IDD_FREETYPEDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	void DrawString(const wstring& strText, HDC hdc);
	void GetRenderData(FCharacterEntry& InFontData, const TCHAR Char);
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
