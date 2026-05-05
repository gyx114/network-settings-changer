
// 切换网络Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "切换网络.h"
#include "切换网络Dlg.h"
#include "afxdialogex.h"
#include <string>
#include <vector>
#include <Windows.h>
#include <iphlpapi.h>

#define IDM_LIST_COPY 20001

#pragma comment(lib, "iphlpapi.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C切换网络Dlg 对话框



C切换网络Dlg::C切换网络Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C切换网络Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(C切换网络Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &C切换网络Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &C切换网络Dlg::OnBnClickedButton2)
   ON_BN_CLICKED(IDC_BUTTON3, &C切换网络Dlg::OnBnClickedButton3)
   ON_BN_CLICKED(IDC_BUTTON4, &C切换网络Dlg::OnBnClickedButton4)
   ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &C切换网络Dlg::OnTcnSelchangeTab1)
   ON_NOTIFY(NM_RCLICK, IDC_LIST1, &C切换网络Dlg::OnNMRClickList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &C切换网络Dlg::OnNMDblclkList1)
END_MESSAGE_MAP()


// C切换网络Dlg 消息处理程序

BOOL C切换网络Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_tab.InsertItem(0, _T("切换网络"));
	m_tab.InsertItem(1, _T("网卡信息"));
	m_tab.SetCurSel(0);

   m_list.ModifyStyle(0, LVS_REPORT);
	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
   m_list.InsertColumn(0, _T("状态"), LVCFMT_LEFT, 70);
	m_list.InsertColumn(1, _T("网卡名称"), LVCFMT_LEFT, 180);

	UpdateTabControls();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C切换网络Dlg::OnPaint()
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
HCURSOR C切换网络Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void C切换网络Dlg::OnBnClickedButton1()
{
    // 切换到互联网配置（配置节名: Internet）
	CString iniPath;
	wchar_t buf[MAX_PATH];
	GetModuleFileName(NULL, buf, MAX_PATH);
	CString modulePath = buf;
	int pos = modulePath.ReverseFind('\\');
	if (pos != -1)
		iniPath = modulePath.Left(pos + 1) + _T("config.ini");
	else
		iniPath = _T("config.ini");

	// 如果配置不存在，写入默认配置以便调试
	DWORD attrib = GetFileAttributes(iniPath);
	if (attrib == INVALID_FILE_ATTRIBUTES)
	{
		// 写默认 Internet 和 Jinhong 配置
		WritePrivateProfileString(_T("Internet"), _T("InterfaceName"), _T("Ethernet"), iniPath);
		WritePrivateProfileString(_T("Internet"), _T("IP"), _T("192.168.0.100"), iniPath);
		WritePrivateProfileString(_T("Internet"), _T("Mask"), _T("255.255.255.0"), iniPath);
		WritePrivateProfileString(_T("Internet"), _T("Gateway"), _T("192.168.0.1"), iniPath);
		WritePrivateProfileString(_T("Internet"), _T("DNS1"), _T("8.8.8.8"), iniPath);
		WritePrivateProfileString(_T("Internet"), _T("DNS2"), _T("8.8.4.4"), iniPath);

		WritePrivateProfileString(_T("Jinhong"), _T("InterfaceName"), _T("Ethernet"), iniPath);
		WritePrivateProfileString(_T("Jinhong"), _T("IP"), _T("10.10.10.100"), iniPath);
		WritePrivateProfileString(_T("Jinhong"), _T("Mask"), _T("255.255.255.0"), iniPath);
		WritePrivateProfileString(_T("Jinhong"), _T("Gateway"), _T("10.10.10.1"), iniPath);
		WritePrivateProfileString(_T("Jinhong"), _T("DNS1"), _T("114.114.114.114"), iniPath);
		WritePrivateProfileString(_T("Jinhong"), _T("DNS2"), _T("1.1.1.1"), iniPath);
	}

	// 读取 Internet 节
	TCHAR bufVal[256];
	GetPrivateProfileString(_T("Internet"), _T("InterfaceName"), _T(""), bufVal, 256, iniPath);
	CString ifName = bufVal;
	GetPrivateProfileString(_T("Internet"), _T("IP"), _T(""), bufVal, 256, iniPath);
	CString ip = bufVal;
	GetPrivateProfileString(_T("Internet"), _T("Mask"), _T(""), bufVal, 256, iniPath);
	CString mask = bufVal;
	GetPrivateProfileString(_T("Internet"), _T("Gateway"), _T(""), bufVal, 256, iniPath);
	CString gateway = bufVal;
	GetPrivateProfileString(_T("Internet"), _T("DNS1"), _T(""), bufVal, 256, iniPath);
	CString dns1 = bufVal;
	GetPrivateProfileString(_T("Internet"), _T("DNS2"), _T(""), bufVal, 256, iniPath);
	CString dns2 = bufVal;

	if (ifName.IsEmpty() || ip.IsEmpty() || mask.IsEmpty())
	{
		AfxMessageBox(_T("配置不完整，请检查 config.ini 中的 Internet 节"));
		return;
	}

	// 构建 netsh 命令并执行
	CString cmd;
	// 设置 IP
	cmd.Format(_T("netsh interface ipv4 set address name=\"%s\" static %s %s %s 1"), ifName, ip, mask, gateway);
	{
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(si);
		BOOL ok = CreateProcess(NULL, cmd.GetBuffer(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		cmd.ReleaseBuffer();
		if (ok)
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}

	// 设置主 DNS
	if (!dns1.IsEmpty())
	{
		CString cmd2;
		cmd2.Format(_T("netsh interface ipv4 set dnsservers name=\"%s\" static %s primary"), ifName, dns1);
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(si);
		BOOL ok = CreateProcess(NULL, cmd2.GetBuffer(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		cmd2.ReleaseBuffer();
		if (ok)
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}

	// 添加次 DNS（如果有）
	if (!dns2.IsEmpty())
	{
		CString cmd3;
		cmd3.Format(_T("netsh interface ipv4 add dnsservers name=\"%s\" address=%s index=2"), ifName, dns2);
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(si);
		BOOL ok = CreateProcess(NULL, cmd3.GetBuffer(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		cmd3.ReleaseBuffer();
		if (ok)
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}

	AfxMessageBox(_T("已尝试应用 Internet 配置。注意：修改网络需要管理员权限，若失败请以管理员身份运行程序。"));
}

void C切换网络Dlg::OnBnClickedButton2()
{
    // 切换到金宏网配置（配置节名: Jinhong）
	CString iniPath;
	wchar_t buf[MAX_PATH];
	GetModuleFileName(NULL, buf, MAX_PATH);
	CString modulePath = buf;
	int pos = modulePath.ReverseFind('\\');
	if (pos != -1)
		iniPath = modulePath.Left(pos + 1) + _T("config.ini");
	else
		iniPath = _T("config.ini");

	// 读取 Jinhong 节
	TCHAR bufVal[256];
	GetPrivateProfileString(_T("Jinhong"), _T("InterfaceName"), _T(""), bufVal, 256, iniPath);
	CString ifName = bufVal;
	GetPrivateProfileString(_T("Jinhong"), _T("IP"), _T(""), bufVal, 256, iniPath);
	CString ip = bufVal;
	GetPrivateProfileString(_T("Jinhong"), _T("Mask"), _T(""), bufVal, 256, iniPath);
	CString mask = bufVal;
	GetPrivateProfileString(_T("Jinhong"), _T("Gateway"), _T(""), bufVal, 256, iniPath);
	CString gateway = bufVal;
	GetPrivateProfileString(_T("Jinhong"), _T("DNS1"), _T(""), bufVal, 256, iniPath);
	CString dns1 = bufVal;
	GetPrivateProfileString(_T("Jinhong"), _T("DNS2"), _T(""), bufVal, 256, iniPath);
	CString dns2 = bufVal;

	if (ifName.IsEmpty() || ip.IsEmpty() || mask.IsEmpty())
	{
		AfxMessageBox(_T("配置不完整，请检查 config.ini 中的 Jinhong 节"));
		return;
	}

	// 设置 IP
	CString cmd;
	cmd.Format(_T("netsh interface ipv4 set address name=\"%s\" static %s %s %s 1"), ifName, ip, mask, gateway);
	{
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(si);
		BOOL ok = CreateProcess(NULL, cmd.GetBuffer(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		cmd.ReleaseBuffer();
		if (ok)
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}

	// 设置主 DNS
	if (!dns1.IsEmpty())
	{
		CString cmd2;
		cmd2.Format(_T("netsh interface ipv4 set dnsservers name=\"%s\" static %s primary"), ifName, dns1);
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(si);
		BOOL ok = CreateProcess(NULL, cmd2.GetBuffer(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		cmd2.ReleaseBuffer();
		if (ok)
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}

	// 添加次 DNS（如果有）
	if (!dns2.IsEmpty())
	{
		CString cmd3;
		cmd3.Format(_T("netsh interface ipv4 add dnsservers name=\"%s\" address=%s index=2"), ifName, dns2);
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(si);
		BOOL ok = CreateProcess(NULL, cmd3.GetBuffer(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		cmd3.ReleaseBuffer();
		if (ok)
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}

	AfxMessageBox(_T("已尝试应用 Jinhong 配置。注意：修改网络需要管理员权限，若失败请以管理员身份运行程序。"));
}

void C切换网络Dlg::OnBnClickedButton3()
{
	// 打开配置文件（在 exe 同目录的 config.ini）
	wchar_t buf[MAX_PATH];
	GetModuleFileName(NULL, buf, MAX_PATH);
	CString modulePath = buf;
	int pos = modulePath.ReverseFind('\\');
	CString iniPath;
	if (pos != -1)
		iniPath = modulePath.Left(pos + 1) + _T("config.ini");
	else
		iniPath = _T("config.ini");

	// 如果不存在，提示并返回（InitInstance 已创建默认文件，但以防万一）
	DWORD attr = GetFileAttributes(iniPath);
	if (attr == INVALID_FILE_ATTRIBUTES)
	{
		AfxMessageBox(_T("配置文件不存在：") + iniPath);
		return;
	}

	// 使用默认关联程序打开
	HINSTANCE h = ShellExecute(NULL, _T("open"), iniPath, NULL, NULL, SW_SHOWNORMAL);
	if ((INT_PTR)h <= 32)
	{
		AfxMessageBox(_T("无法打开配置文件，请手动定位并打开 config.ini。"));
	}
}

void C切换网络Dlg::OnBnClickedButton4()
{
 m_list.DeleteAllItems();

	ULONG bufferLength = 0;
	DWORD flags = GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER;
	DWORD result = GetAdaptersAddresses(AF_UNSPEC, flags, nullptr, nullptr, &bufferLength);
	if (result != ERROR_BUFFER_OVERFLOW)
	{
		m_list.InsertItem(0, _T("无法获取网卡信息。"));
		return;
	}

	std::vector<BYTE> buffer(bufferLength);
	auto addresses = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(buffer.data());
	result = GetAdaptersAddresses(AF_UNSPEC, flags, nullptr, addresses, &bufferLength);
	if (result != NO_ERROR)
	{
		m_list.InsertItem(0, _T("无法获取网卡信息。"));
		return;
	}

  int index = 0;
	for (auto current = addresses; current != nullptr; current = current->Next)
	{
		if (!current->FriendlyName || current->FriendlyName[0] == L'\0')
		{
			continue;
		}

        CString status;
		switch (current->OperStatus)
		{
		case IfOperStatusUp:
			status = _T("已连接");
			break;
		case IfOperStatusDown:
			status = _T("未连接");
			break;
		case IfOperStatusDormant:
			status = _T("休眠");
			break;
		case IfOperStatusNotPresent:
			status = _T("不可用");
			break;
		case IfOperStatusLowerLayerDown:
			status = _T("链路断开");
			break;
		default:
			status = _T("未知");
			break;
		}

		m_list.InsertItem(index, status);
		m_list.SetItemText(index, 1, current->FriendlyName);
		++index;
	}

	if (index == 0)
	{
       m_list.InsertItem(0, _T("未找到网卡。"));
	}
}

void C切换网络Dlg::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	CPoint point;
	GetCursorPos(&point);
	CMenu menu;
	if (!menu.CreatePopupMenu())
	{
		return;
	}

	menu.AppendMenu(MF_STRING, IDM_LIST_COPY, _T("复制"));
	int command = menu.TrackPopupMenu(TPM_RETURNCMD | TPM_RIGHTBUTTON, point.x, point.y, this);
	if (command == IDM_LIST_COPY)
	{
		CopySelectedAdapterName();
	}
}

void C切换网络Dlg::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	CopySelectedAdapterName();
	*pResult = 0;
}

void C切换网络Dlg::CopySelectedAdapterName()
{
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (!pos)
	{
		return;
	}

	int item = m_list.GetNextSelectedItem(pos);
	CString name = m_list.GetItemText(item, 1);
	if (name.IsEmpty())
	{
		return;
	}

	if (!OpenClipboard())
	{
		return;
	}

	EmptyClipboard();
	SIZE_T bytes = (name.GetLength() + 1) * sizeof(wchar_t);
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, bytes);
	if (hMem)
	{
		void* data = GlobalLock(hMem);
		if (data)
		{
			memcpy(data, name.GetString(), bytes);
			GlobalUnlock(hMem);
			SetClipboardData(CF_UNICODETEXT, hMem);
		}
		else
		{
			GlobalFree(hMem);
		}
	}

	CloseClipboard();
}

void C切换网络Dlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateTabControls();
	*pResult = 0;
}

void C切换网络Dlg::UpdateTabControls()
{
	int sel = m_tab.GetCurSel();
	bool isTab1 = (sel == 0);
	bool isTab2 = (sel == 1);

	GetDlgItem(IDC_BUTTON1)->ShowWindow(isTab1 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BUTTON2)->ShowWindow(isTab1 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BUTTON3)->ShowWindow(isTab2 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BUTTON4)->ShowWindow(isTab2 ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_LIST1)->ShowWindow(isTab2 ? SW_SHOW : SW_HIDE);
}
