
// MFCApplication1Dlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1Dlg 대화 상자



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_DROP_LIST, m_drop_list);
	DDX_Control(pDX, IDC_HIDE_PATH_CHECK, m_hide_path_check);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_HIDE_PATH_CHECK, &CMFCApplication1Dlg::OnBnClickedHidePathCheck)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 메시지 처리기

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	m_drop_list.SubclassDlgItem(IDC_DROP_LIST, this);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication1Dlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnDropFiles(HDROP hDropInfo)
{
	m_drop_list.ResetContent();

	int count = DragQueryFile(hDropInfo, 0xFFFFFFFF/*-1*/, NULL, 0);

	wchar_t temp_path[MAX_PATH];

	if (count > 0) {
		int len = DragQueryFile(hDropInfo, 0, temp_path, MAX_PATH);
		if (len > 3) {
			wchar_t *p = temp_path + len;
			while (*p != '\\') p--;
			*(p + 1) = 0;
			SetDlgItemText(IDC_PATH_EDIT, temp_path);
			m_drop_list.SetBasePath(temp_path, p - temp_path + 1);
		}
	}

	for (int i = 0; i < count; i++) {
		DragQueryFile(hDropInfo, i, temp_path, MAX_PATH);
		m_drop_list.InsertString(i, temp_path);
		/*
		if (GetFileAttributes(temp_path) & FILE_ATTRIBUTE_DIRECTORY) {
			// 디렉토리라면 1
			m_drop_list.SetItemData(i, 1);
		}
		else {
			// 파일이라면 0
			m_drop_list.SetItemData(i, 0);
		}
		*/
		// 소스간단화
		m_drop_list.SetItemData(i, (GetFileAttributes(temp_path) & FILE_ATTRIBUTE_DIRECTORY) >> 4);
	}

	MakeFileList();

	CDialogEx::OnDropFiles(hDropInfo);
}


void CMFCApplication1Dlg::MakeFileList()
{
	int is_continue, count, i;
	wchar_t temp_path[MAX_PATH];

	do {
		is_continue = 0;
		count = m_drop_list.GetCount();
		for (i = 0; i < count; i++) {
			if (m_drop_list.GetItemData(i) == 1) {
				// 디렉터리 발견
				m_drop_list.SetItemData(i, 2);
				m_drop_list.GetText(i, temp_path);
				GetFileInDirectory(temp_path, i + 1);
				is_continue = 1;
				break;
			}
		}
	} while (is_continue);
}

void CMFCApplication1Dlg::GetFileInDirectory(wchar_t * ap_path, int a_insert_index)
{
	CString path, file_path;
	WIN32_FIND_DATA file_data;

	path.Format(L"%s\\*.*", ap_path);
	HANDLE h_item_list = FindFirstFile(path, &file_data);
	if (INVALID_HANDLE_VALUE != h_item_list) {
		do {
			// ".", ".." 모두 출력됨
			// 배제하는 코드 추가
			if (memcmp(file_data.cFileName, L".", 4) && memcmp(file_data.cFileName, L"..", 6)) {	
				// . : 유니코드이고 NULL 문자를 포함하기에 4바이트
				file_path.Format(L"%s\\%s", ap_path, file_data.cFileName);
				m_drop_list.InsertString(a_insert_index, file_path);
				m_drop_list.SetItemData(a_insert_index, (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) >> 4);

			}
		} while (FindNextFile(h_item_list, &file_data));
	}
}

void CMFCApplication1Dlg::OnBnClickedHidePathCheck()
{
	m_drop_list.SetSimpleMode(m_hide_path_check.GetCheck());
}
