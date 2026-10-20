
// ListControlDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ListControl.h"
#include "ListControlDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation

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



// CListControlDlg dialog

CListControlDlg::CListControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LISTCONTROL_DIALOG, pParent), m_nSelRow(-1), m_nSelCol(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

// DoDataExchange: 화면에 있는 저 컨트롤(ID)를 코드 상의 이 변수(m_listCtrl)로 제어하겠다는 대상 지정
void CListControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_editValue);
}

// MESSAGE_MAP: 사용자가 화면의 저 버튼을 눌렀을 때 이 함수(OnBnClickedBtnOpen)를 실행해라는 동작 지정
BEGIN_MESSAGE_MAP(CListControlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CListControlDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CListControlDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CListControlDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_EDIT, &CListControlDlg::OnBnClickedBtnEdit)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CListControlDlg::OnNMClickList1)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST1, &CListControlDlg::OnLvnGetdispinfoList1)
	ON_BN_CLICKED(IDC_BTN_FONT, &CListControlDlg::OnBnClickedBtnFont)
END_MESSAGE_MAP()


// CListControlDlg message handlers

BOOL CListControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_listCtrl.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);						// 리스트 컨트롤의 보기 모드를 report로 설정 (없어도 되지만 확실하게 하기 위해)
	m_listCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);	// 셀 사이 격자선 표시, 특정 셀 클릭 시 해당 행 전체 하이라이트됨

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CListControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CListControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CListControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/*
void SeparationString(std::vector<CString>& array, CString Text, CString Tokens)
{
	if (Text.IsEmpty())
	{
		return;
	}

	int Start = 0;
	int Len = Text.GetLength();

	for (int i = 0; i < Len; i++)
	{
		if (Tokens.Find(Text[i]) != -1)
		{
			array.push_back(Text.Mid(Start, i - Start));
			Start = i + 1;
		}
	}

	if (Start <= Len)
	{
		array.push_back(Text.Mid(Start, Len - Start));
	}
}
*/

// array는 Btnopen 함수 내 arr의 참조, text는 자를 글자, tokens는 구분 글자
void vectorString(std::vector<CString>& array, CString text, CString tokens)
{
	// 자를 글자가 비어 있으면 바로 끝냄
	if (text.IsEmpty())
	{
		return;
	}
	// 아니면 벡터를 비운 상태에서 시작
	array.clear();

	// 지금 자를 단어가 시작하는 위치
	int Start{};
	// 전체 글자 수
	int Len{ text.GetLength() };

	for (int i{}; i < Len; i++)
	{	
		// 지금 글자가 쉼표라면
		if (tokens.Find(text[i]) != -1)
		{
			// 
			array.push_back(text.Mid(Start, i - Start));
			Start = i + 1;
		}
	}

	if (Start <= Len)
	{
		array.push_back(text.Mid(Start, Len - Start));
	}
}


void CListControlDlg::OnBnClickedBtnOpen()
{
	// TODO: Add your control notification handler code here
	// 파일 열고 csv 확장자 파일만 표시되도록 설정
	CFileDialog dlg(TRUE, _T("csv"), NULL, OFN_FILEMUSTEXIST,
		_T("CSV Files (*.csv)|*.csv|All Files (*.*)|*.*||"));

	// 파일 고르고 ok 버튼 누르면
	if (dlg.DoModal() == IDOK)
	{
		// 파일 경로를 변수 strFilePath에 담음
		CString strFilePath = dlg.GetPathName();

		// 새 파일 열기 전 표에 남아 있는 이전 데이터, 제목 싹 날림
		OnBnClickedBtnClose();

		// 글자 안 깨지도록 설정
		setlocale(LC_ALL, "korean");

		// 파일 입출력 담당할 CStdioFile 객체 file 생성
		CStdioFile file;

		// strFilePath이 읽기 모드로 열렸을 때
		if (file.Open(strFilePath, CFile::modeRead | CFile::typeText))
		{
			// 파일에서 읽어온 단 한 줄의 텍스트 문자열 전체
			CString strLine;
			// 지금 읽는 줄이 첫 번째 줄인지 확인하는 bool 타입 변수
			bool bTopicLine = true;
			// 리스트 컨트롤(표)에 데이터를 넣을 행(Row, 가로줄) 번호 변수 (0부터 시작)
			int nRow = 0;

			// 파일 첫 번째 줄부터 마지막 줄까지 한 줄씩 글자 읽어와 strLine에 저장
			// 엑셀 자체에서 줄바꿈이 발생하면 \r\n이 추가됨 따라서 첫 번째 행의 내용이 끝나면 엑셀에 명시된 \r\n을 통해 줄바꿈이 자동으로 일어남
			while (file.ReadString(strLine))
			{
				if (strLine.IsEmpty())
				{
					continue;
				}

				bool bQuote = false;
				
				for (int i = 0; i < strLine.GetLength(); i++)
				{
					if (strLine[i] == _T('"'))
					{
						bQuote = !bQuote;
					}
					else if (strLine[i] == _T(',') && bQuote)
					{
						strLine.SetAt(i, _T(' '));
					}
				}

				strLine.Remove(_T('"'));

				std::vector<CString> arr;

				// SeparationString(arr, strLine, _T(","));
				vectorString(arr, strLine, _T(","));

				if (bTopicLine)
				{
					for (int i = 0; i < (int)arr.size(); i++)
					{
						m_listCtrl.InsertColumn(i, arr[i], LVCFMT_LEFT, 100);
					}

					m_vecHeader = arr;
					bTopicLine = false;
				}
				else
				{
					/*
					int nItemIndex = m_listCtrl.InsertItem(nRow, arr[0]);

					for (int i = 1; i < (int)arr.size(); i++)
					{
						m_listCtrl.SetItemText(nItemIndex, i, arr[i]);
					}

					nRow++;
					*/

					arr.resize(m_vecHeader.size());
					m_vecData.push_back(arr);
				}



				//// 가져온 줄을 쉼표 단위로 잘라내어 strToken에 담음
				//int nPos = 0;
				//int nCol = 0;

				//// strToken 변수 선언, 맨 처음 쉼표 전까지의 단어로 초기화 (첫 번째 줄 첫 번째 단어)
				//CString strToken = strLine.Tokenize(_T(","), nPos);

				//// bTopicLine이 참일 때
				//if (bTopicLine)
				//{
				//	// strToken 문자열이 비어있지 않는 동안 계속 반복
				//	while (!strToken.IsEmpty())
				//	{
				//		// 리스트 컨트롤(표)에 제목 칼럼(가로줄 세로칸)을 추가 (nCol 위치에, 좌측 정렬, 너비 100)
				//		// 칼럼 만들고 나서 nCol 변수값을 1 증가시킴
				//		m_listCtrl.InsertColumn(nCol++, strToken, LVCFMT_LEFT, 100);
				//		// 한 줄 전체(strLine)에서 다음 쉼표 전까지 단어 자르고 nPos 위치 갱신하여 strToken에 덮어씀
				//		strToken = strLine.Tokenize(_T(","), nPos);
				//	}

				//	// 첫 번째 줄(제목열) 넣는 작업 완료했으므로 false로 변경, if(bTopicLine) 탈출 else로 진입
				//	bTopicLine = false;
				//}
				//else
				//{
				//	// 지금 꺼낸 strToken이 비어 있지 않다면
				//	if (!strToken.IsEmpty())
				//	{
				//		// nItemIndex는 행 번호, 제목열에 대한 작업이 완료됐기에 그 아랫 줄부터 
				//		int nItemIndex = m_listCtrl.InsertItem(nRow, strToken);	
				//		// 쉼표 전까지의 단어 strToken에 저장
				//		strToken = strLine.Tokenize(_T(","), nPos);

				//		// 
				//		while (!strToken.IsEmpty())
				//		{
				//			nCol++;
				//			m_listCtrl.SetItemText(nItemIndex, nCol, strToken);
				//			strToken = strLine.Tokenize(_T(","), nPos);
				//		}
				//		nRow++;
				//	}
				//}
			}

			file.Close();
			m_listCtrl.SetItemCount((int)m_vecData.size());
		}
	}
}


void CListControlDlg::OnBnClickedBtnClose()
{
	// TODO: Add your control notification handler code here
	m_vecHeader.clear();
	m_vecData.clear();
	m_listCtrl.DeleteAllItems();
	
	CHeaderCtrl* pHeader = m_listCtrl.GetHeaderCtrl();

	if (pHeader != NULL)
	{
		int nColCount = pHeader->GetItemCount();

		for (int i = nColCount - 1; i >= 0; i--)
		{
			m_listCtrl.DeleteColumn(i);
		}
	}

	m_nSelRow = -1;
	m_editValue.SetWindowText(_T(""));
}

void CListControlDlg::OnBnClickedBtnSave()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(FALSE, _T("csv"), _T("MemberList.CSV"), OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
		_T("CSV Files (*.csv),|*.csv|All Files (*.*)|*.*||"));

	if (dlg.DoModal() == IDOK)
	{
		CString strFilePath = dlg.GetPathName();

		setlocale(LC_ALL, "korean");

		CStdioFile file;

		if (file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
		{
			CString strLine{};

			for (size_t c{}; c < m_vecHeader.size(); c++)
			{
				strLine += m_vecHeader[c];

				if (c + 1 < m_vecHeader.size())
				{
					strLine += _T(",");
				}
			}

			file.WriteString(strLine + _T("\n"));

			for (size_t r{}; r < m_vecData.size(); r++)
			{
				strLine.Empty();

				for (size_t c{}; c < m_vecData[r].size(); c++)
				{
					strLine += m_vecData[r][c];

					if (c + 1 < m_vecData[r].size())
					{
						strLine += _T(",");
					}
				}
			}

			file.WriteString(strLine + _T("\n"));
			file.Close();
			AfxMessageBox(_T("성공적으로 저장되었습니다."));


			/*
			CHeaderCtrl* pHeader = m_listCtrl.GetHeaderCtrl();

			int nColCount = (pHeader != nullptr) ? pHeader->GetItemCount() : 0;
			int nRowCount = m_listCtrl.GetItemCount();

			CString strHeaderLine =  _T("");
			LVCOLUMN lvc;
			lvc.mask = LVCF_TEXT;
			TCHAR szBuffer[256];
			lvc.pszText = szBuffer;
			lvc.cchTextMax = 256;

			for (int col = 0; col < nColCount; ++col)
			{
				if (m_listCtrl.GetColumn(col, &lvc))
				{
					strHeaderLine += lvc.pszText;
					if (col < nColCount - 1)
					{
						strHeaderLine += _T(",");
					}
				}
			}

			strHeaderLine += _T("\n");
			file.WriteString(strHeaderLine);

			for (int row = 0; row < nRowCount; ++row)
			{
				CString strRowLine = _T("");

				for (int col = 0; col < nColCount; ++col)
				{
					strRowLine += m_listCtrl.GetItemText(row, col);
					if (col < nColCount - 1)
					{
						strRowLine += _T(",");
					}
				}

				strRowLine += _T("\n");
				file.WriteString(strRowLine);
			}
			*/
		}
	}
}

void CListControlDlg::OnBnClickedBtnEdit()
{
	if (m_nSelRow < 0)
	{
		AfxMessageBox(_T("수정할 칸을 클릭해 주세요."));
		return;
	}

	CString strNew;
	m_editValue.GetWindowText(strNew);
	// m_listCtrl.SetItemText(m_nSelRow, m_nSelCol, strNew);
	
	m_vecData[m_nSelRow][m_nSelCol] = strNew;
	m_listCtrl.Invalidate();
}

void CListControlDlg::OnNMClickList1(NMHDR* pMNHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNM = reinterpret_cast<LPNMITEMACTIVATE>(pMNHDR);

	LVHITTESTINFO hit = { 0 };
	hit.pt = pNM->ptAction;
	m_listCtrl.SubItemHitTest(&hit);

	if (hit.iItem >= 0)
	{
		m_nSelRow = hit.iItem;
		m_nSelCol = hit.iSubItem;
		// m_editValue.SetWindowText(m_listCtrl.GetItemText(hit.iItem, hit.iSubItem));
		
		if (hit.iSubItem >= 0 && hit.iSubItem < (int)m_vecData[hit.iItem].size())
		{
			m_editValue.SetWindowText(m_vecData[hit.iItem][hit.iSubItem]);
		}
	}

	*pResult = 0;
}
void CListControlDlg::OnBnClickedBtnFont()
{
	// TODO: Add your control notification handler code here
	CFontDialog dlg;
	
	if (dlg.DoModal() == IDOK)
	{
		int nSize = dlg.GetSize();
		if (nSize < 60)
		{
			nSize = 90;
		}

		m_font.DeleteObject();
		m_font.CreatePointFont(nSize, dlg.GetFaceName());

		m_listCtrl.SetFont(&m_font);
		m_editValue.SetFont(&m_font);
	}
}

void CListControlDlg::OnLvnGetdispinfoList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVDISPINFO* pDisp = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	if (pDisp->item.mask& LVIF_TEXT)
	{
		int r = pDisp->item.iItem;
		int c = pDisp->item.iSubItem;

		if (r >= 0 && r < (int)m_vecData.size() && c >= 0 && c < (int)m_vecData[r].size())
		{
			_tcsncpy_s(pDisp->item.pszText, pDisp->item.cchTextMax, (LPCTSTR)m_vecData[r][c], _TRUNCATE);
		}
	}
	*pResult = 0;
}