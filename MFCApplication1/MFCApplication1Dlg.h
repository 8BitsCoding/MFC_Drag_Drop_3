
// MFCApplication1Dlg.h: 헤더 파일
//

#pragma once

#include "TW_ListBox.h"

class TW_FileListBox : public TW_ListBox
{
private:
	char m_simple_mode = 0;
	wchar_t m_base_path[MAX_PATH];
	int m_path_len;

public:
	void SetBasePath(const wchar_t * ap_path, int a_length) 
	{
		m_path_len = a_length;
		memcpy(m_base_path, ap_path, a_length + 1);
	}

	void SetSimpleMode(char a_is_simple)
	{
		if (m_simple_mode != a_is_simple) {
			m_simple_mode = a_is_simple;
			Invalidate();
		}
	}

	virtual void DrawUserItem(CDC *ap_dc, RECT *ap_rect, int a_index, void * ap_data, unsigned char a_select_flag, unsigned char a_focus_flag)
	{
		CString str;
		GetText(a_index, str);

		if (GetItemData(a_index)) ap_dc->SetTextColor(RGB(255, 255, 128));
		else ap_dc->SetTextColor(RGB(128, 200, 200));

		if (m_simple_mode) {
			ap_dc->TextOut(ap_rect->left + 5, ap_rect->top + 3, (const wchar_t*)str + m_path_len, str.GetLength() - m_path_len);
		} else ap_dc->TextOut(ap_rect->left + 5, ap_rect->top + 3, str);
	}
};

// CMFCApplication1Dlg 대화 상자
class CMFCApplication1Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//CListBox m_drop_list;
	TW_FileListBox m_drop_list;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	void MakeFileList();
	void GetFileInDirectory(wchar_t * ap_path, int a_insert_index);
	CButton m_hide_path_check;
	afx_msg void OnBnClickedHidePathCheck();
};
