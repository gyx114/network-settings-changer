
// 切换网络Dlg.h: 头文件
//

#pragma once


// C切换网络Dlg 对话框
class C切换网络Dlg : public CDialogEx
{
// 构造
public:
	C切换网络Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CTabCtrl m_tab;
	CListCtrl m_list;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();
  afx_msg void OnBnClickedButton4();
  afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	void UpdateTabControls();
  void CopySelectedAdapterName();
};
