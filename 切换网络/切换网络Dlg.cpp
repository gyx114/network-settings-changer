
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
}

BEGIN_MESSAGE_MAP(C切换网络Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &C切换网络Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &C切换网络Dlg::OnBnClickedButton2)
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
