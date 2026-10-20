// ListControlDlg.h : header file
//

#pragma once

#include <clocale>
#include <vector>


// CListControlDlg dialog
class CListControlDlg : public CDialogEx
{
// Construction
public:
	CListControlDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LISTCONTROL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_listCtrl{};										// CSV 파일 데이터를 그리드 형태로 보여줄 리스트 컨트롤 변수
	CEdit m_editValue{};										// 특정 셀을 수정할 때 쓰는 에디트 컨트롤 변수
	CFont m_font{};												// 폰트 수정할 때 쓰는 객체

	std::vector<CString> m_vecHeader{};							// 컬럼 제목 보관함
	std::vector<std::vector<CString>> m_vecData{};				// 데이터 전체 보관함

	int m_nSelRow;												// 현재 선택된 행 번호 
	int m_nSelCol;												// 현재 선택된 열 번호

	afx_msg void OnBnClickedBtnOpen();							// Open 버튼 컨트롤 함수
	afx_msg void OnBnClickedBtnClose();							// Close 버튼 컨트롤 함수
	afx_msg void OnBnClickedBtnSave();							// Save 버튼 컨트롤 함수
	afx_msg void OnBnClickedBtnEdit();							// Edit 버튼 컨트롤 함수
	afx_msg void OnBnClickedBtnFont();							// Font 버튼 컨트롤 함수
	afx_msg void OnNMClickList1(NMHDR* pNMHDR, LRESULT* pResult);	// 리스트 클릭했을 때
	afx_msg void OnLvnGetdispinfoList1(NMHDR* pNMHDR, LRESULT* pResult);	// 리스트가 이 칸에 쓸 글자 알려달라고 물을 때
};