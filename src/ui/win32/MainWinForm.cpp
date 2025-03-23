
#include <windows.h>
#include "MainWinForm.h"
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 200
HINSTANCE hInst;
HWND buttonImport, buttonExport, buttonAdd, buttonEdit, buttonDelete, buttonNetworkTime;
HWND hListView;
WCHAR interval[MAX_LOADSTRING];
WCHAR counter[MAX_LOADSTRING];
WCHAR error[MAX_LOADSTRING];
WCHAR errorNoSecret[MAX_LOADSTRING];
WCHAR errorErrorEncodeBase32[MAX_LOADSTRING];
WCHAR errorTimeEqualsZero[MAX_LOADSTRING];
WCHAR viewConfirm[MAX_LOADSTRING];
WCHAR viewTip[MAX_LOADSTRING];
WCHAR confirm[MAX_LOADSTRING];
WCHAR lastSync[MAX_LOADSTRING];
WCHAR lastSyncFailed[MAX_LOADSTRING];

wstring s2ws(const string& s);
string ws2s(std::wstring s);
GlobalConfiguration* globalConfig = GlobalConfiguration::getInstance();
vector<OTPInfo> otpInfos = globalConfig->getOTPConfig();
vector<string> sntpServers = globalConfig->getSNTPServers();
INT_PTR CALLBACK OTP_Client(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT isOTPDIALOGEDIT = -2;
std::map<std::string, int64_t> CurrentKeys;
std::map<std::string, vector<BYTE>> EncryptedDataMap;
bool alwaysUseNetTime = false;

INT_PTR CALLBACK HotpClientViewerProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK timeServerManager(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
SNTPClient sntpClient;
void ReadDataFromFile() {
	vector<BYTE> data = ReadDataFromFile("data.dat");
	if (data.size() == 0) return;
	string dataStr = DecryptData(data);
	globalConfig->setConfig(dataStr);
	otpInfos = globalConfig->getOTPConfig();
	sntpServers = globalConfig->getSNTPServers();
	alwaysUseNetTime = globalConfig->getUseNetworkTime();
	for (int i = 0; i < otpInfos.size(); i++)
	{

		//otpInfo.secret = encodeBase64FromBYTE(EncryptData(otpInfo.secret));
		GUID guid;
		HRESULT r1 = CoCreateGuid(&guid);
		TCHAR r[48];
		r1 = StringFromGUID2(guid, r, 48);
		string guidStr = ws2s(r);

		EncryptedDataMap[guidStr] = decodeBase64ToBYTE(otpInfos[i].secret);
		CurrentKeys[guidStr] = otpInfos[i].addition_param;
		otpInfos[i].secret = guidStr;
	}
}

void saveDataToFile()
{
	vector<OTPInfo> p = otpInfos;
	for (int i = 0; i < p.size(); i++)
	{
		p[i].secret = encodeBase64FromBYTE(EncryptedDataMap[p[i].secret]);
	}
	globalConfig->setOTPConfig(p);
	globalConfig->setSNTP_servers(sntpServers);
	globalConfig->setUseNetworkTime(alwaysUseNetTime);

	vector<BYTE> writeDATA = EncryptData(globalConfig->getConfig());

	BOOL writeResult = WriteBytesToFile("data.dat", writeDATA);
}




void setLangTextFromi18n(HWND hWnd) {
	i18nClient* i18n = i18nClient::getInstence();
	SetWindowText(hWnd, i18n->get("windowTitle").c_str());
	SetWindowText(buttonAdd, i18n->get("add").c_str());
	SetWindowText(buttonEdit, i18n->get("edit").c_str());
	SetWindowText(buttonDelete, i18n->get("delete").c_str());
	SetWindowText(buttonNetworkTime, i18n->get("networkTime").c_str());



	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_WIDTH;
	lvc.cx = 200;
	std::shared_ptr<wchar_t[]> name(new wchar_t[32]);
	std::shared_ptr<wchar_t[]> pwd(new wchar_t[32]);
	std::shared_ptr<wchar_t[]> expire(new wchar_t[32]);

	wcscpy_s(name.get(), 32, i18n->get("name").c_str());
	wcscpy_s(pwd.get(), 32, i18n->get("oncePassword").c_str());
	wcscpy_s(expire.get(), 32, i18n->get("remainTime").c_str());



	lvc.pszText = name.get();
	ListView_SetColumn(hListView, 0, &lvc);
	lvc.pszText = pwd.get();
	ListView_SetColumn(hListView, 1, &lvc);
	lvc.pszText = expire.get();
	ListView_SetColumn(hListView, 2, &lvc);

}


int64_t getCurrentMillSecond(bool usingNetTime = false) {
	if (!usingNetTime) {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
	else {
		int64_t resultTimeStamp = sntpClient.getResultTimestamp() + std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - sntpClient.getLastUpdate();
		if (resultTimeStamp < 0)
		{
			OutputDebugString(L"获取时钟失败");
		}
		return resultTimeStamp >= 0? resultTimeStamp : std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
	return 0;
}



wstring padZero(wstring str, int length) {
	if (str.size() >= length)return str;
	str = wstring(L"00000000").substr(0, length - str.size()) + str;
	return str;
}

void setLangFromi18n()
{
	LANGID lang = GetUserDefaultUILanguage();
	i18nLangSupported langSupported;
	switch (lang) {
	case 0x0804: // zh-CN
		langSupported = ZH_CN;
		break;
	default:
		langSupported = EN_US;
		break;
	}
	i18nClient::getInstence()->setLang(langSupported);
}

int APIENTRY mainWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	hInst = hInstance;
	i18nClient* i18n = i18nClient::getInstence();


	LoadString(hInstance, IDS_HOTPADDITION, counter, MAX_LOADSTRING);
	LoadString(hInstance, IDS_TOTPADDITION, interval, MAX_LOADSTRING);
	LoadString(hInstance, IDS_ERROR, error, MAX_LOADSTRING);
	LoadString(hInstance, IDS_ERROR_NOSECRET, errorNoSecret, MAX_LOADSTRING);
	LoadString(hInstance, IDS_ERROR_ERRORENCODE_BASE32, errorErrorEncodeBase32, MAX_LOADSTRING);
	LoadString(hInstance, IDS_ERROR_TIMEEQUALS_ZERO, errorTimeEqualsZero, MAX_LOADSTRING);
	LoadString(hInstance, IDS_CONFIRM_HOTPVIEW, viewConfirm, MAX_LOADSTRING);
	LoadString(hInstance, IDS_HOTP_INFO, viewTip, MAX_LOADSTRING);
	LoadString(hInstance, IDS_CONFIRM, confirm, MAX_LOADSTRING);
	LoadString(hInstance, IDS_LASTSYNC, lastSync, MAX_LOADSTRING);
	LoadString(hInstance, IDS_LASTSYNCFAILED, lastSyncFailed, MAX_LOADSTRING);

	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_PROGRESS_CLASS;
	InitCommonControlsEx(&icex);

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICONMAIN));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"MainWinForm";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICONMAIN));

	RegisterClassEx(&wcex);
	setLangFromi18n();
	ReadDataFromFile();
	if (IsWindowsVistaOrGreater())
	{
		SetProcessDPIAware();
	}
	HWND hWnd = CreateWindowEx(WS_EX_COMPOSITED, L"MainWinForm", L"OTP客户端", WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZE ^ WS_MAXIMIZEBOX ^ WS_SIZEBOX
		| WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		CW_USEDEFAULT, 0, 650, 450, NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);



	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	return (int)msg.wParam;
}

DWORD WINAPI SNTPClientUpdateThreadProc(LPVOID lpParameter)
{
	while (true)
	{

		if (getCurrentMillSecond() - sntpClient.getLastUpdate() > 600000)
		{

			if (sntpServers.size() > 0)
				if (sntpClient.updateSNTPTimeStamp(sntpServers[0].c_str(), 123, false) < 0 && sntpClient.updateSNTPTimeStamp(sntpServers[0].c_str(), 123, true) < 0)
				{
					if (sntpServers.size() > 1)
					{
						if (sntpClient.updateSNTPTimeStamp(sntpServers[1].c_str(), 123, false) < 0 && sntpClient.updateSNTPTimeStamp(sntpServers[1].c_str(), 123, true) < 0)
						{
						}
					}
				}
		}
		Sleep(60000);
	}
}

DWORD WINAPI ThreadProc(LPVOID lpParameter) {
	HWND hListView = (HWND)lpParameter;

	while (true) {
		Sleep(100);
		PostMessage(hListView, WM_USER + 1, NULL, NULL);

		for (auto key_value : CurrentKeys)
		{

			string i = key_value.first;
			OTPInfo otpInfo;
			int j = 0;
			for (j = 0; j < otpInfos.size(); j++)
			{
				if (otpInfos[j].secret == i)
				{
					otpInfo = otpInfos[j];
					break;
				}
			}
			if (j >= otpInfos.size()) continue;

			if (otpInfo.type == 0 && getCurrentMillSecond(alwaysUseNetTime) / 1000 / otpInfo.addition_param != key_value.second)
			{
				PostMessage(hListView, WM_USER + 2, NULL, NULL);
				break;
			};
		}
	}
	return 0;
}
struct ListViewItemData {
	wstring friendlyName;
	wstring password;
};
void addItem(HWND hListView, OTPInfo otpInfo) {


	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_PARAM;
	lvi.iItem = ListView_GetItemCount(hListView);
	lvi.iSubItem = 0;
	std::unique_ptr<wchar_t[]> name(new wchar_t[32]);
	wcscpy_s(name.get(), 32, s2ws(otpInfo.friendly_name).c_str());
	lvi.pszText = name.get();
	//lvi.lParam = otpInfo;

	wstring wPassword = L"";
	if (otpInfo.type == TOTP)
	{
		string secret = DecryptData(EncryptedDataMap[otpInfo.secret]);
		OTP otp(HOTP);
		otp.setAlgorithm(otpInfo.algorithm);
		int64_t times = getCurrentMillSecond(alwaysUseNetTime) / otpInfo.addition_param / 1000;
		string password = otp.generateOTP(secret, 1, otpInfo.digits, times);
		//lvi.iItem = ListView_GetItemCount(hListView);
		//lvi.iSubItem = 1;
		wPassword = padZero(s2ws(password), otpInfo.digits);

		//wPassword = padZero(s2ws(password), otpInfo.digits);

	}
	else
	{
		wPassword = viewTip;
	}
	ListViewItemData* pItemData = new ListViewItemData{ s2ws(otpInfo.friendly_name), wPassword };
	lvi.lParam = reinterpret_cast<LPARAM>(pItemData);
	ListView_InsertItem(hListView, &lvi);
	ListView_SetItemText(hListView, lvi.iItem, 1, (LPWSTR)wPassword.c_str());

}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:

		AddControls(hWnd);
		setLangTextFromi18n(hWnd);
		CreateThread(NULL, 0, ThreadProc, hWnd, 0, NULL);
		CreateThread(NULL, 0, SNTPClientUpdateThreadProc, hWnd, 0, NULL);
		break;
	case WM_DESTROY:
		saveDataToFile();
		PostQuitMessage(0);
		break;
	case WM_NOTIFY: {
		LPNMLISTVIEW  pnm = (LPNMLISTVIEW)lParam;
		if (pnm->hdr.code == NM_CUSTOMDRAW)
		{
			LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)lParam;
			switch (lplvcd->nmcd.dwDrawStage)
			{
			case CDDS_PREPAINT:
				//OutputDebugString(L"PrePaint\n");
				return CDRF_NOTIFYITEMDRAW; // 请求每项绘制前的通知
			case CDDS_ITEMPREPAINT:
				//OutputDebugString(L"ItemPrePaint\n");
			{
				int a = ListView_GetItemCount(hListView);
				int b = a;
			}
			return CDRF_NOTIFYSUBITEMDRAW; // 请求子项绘制通知
			case  CDDS_ITEMPREPAINT | CDDS_SUBITEM:
				/*OutputDebugString(L"SubItem\n")*/;
				if (lplvcd->iSubItem == 2) // 假设这是进度条列
				{
					int nItem = static_cast<int>(lplvcd->nmcd.dwItemSpec);
					// 获取或计算当前项目的进度值
					if (nItem < 0 || nItem >= otpInfos.size() || otpInfos[nItem].type == HOTP)
						return CDRF_DODEFAULT;

					int64_t interval = otpInfos[nItem].addition_param;
					int64_t now = getCurrentMillSecond(alwaysUseNetTime);
					int64_t usedTime = (now % (interval * 1000));
					int64_t remainTime = interval * 1000 - usedTime;
					int fProgress = usedTime / interval / 10; // 假设进度为50%

					RECT rc;
					RECT progressBarRc = {};

					ListView_GetSubItemRect(hListView, nItem, lplvcd->iSubItem, LVIR_BOUNDS, &rc);
					if (rc.right - rc.left < 58) {
						return CDRF_DODEFAULT;
					}
					progressBarRc.left = rc.left + 4;
					progressBarRc.right = rc.right - 50;
					progressBarRc.top = rc.top + 4;
					progressBarRc.bottom = rc.bottom - 4;



					FillRect(lplvcd->nmcd.hdc, &progressBarRc, GetSysColorBrush(COLOR_BTNFACE));


					// 计算并绘制进度条
					progressBarRc.right = progressBarRc.left + static_cast<LONG>(fProgress * (progressBarRc.right - progressBarRc.left) / 100);

					int g = fProgress > 50 ? 255 - fProgress * 255 / 50 : 255;
					int r = fProgress < 50 ? fProgress * 255 / 50 : 255;
					HBRUSH hbrush = CreateSolidBrush(RGB(r, g, 0));

					FillRect(lplvcd->nmcd.hdc, &progressBarRc, hbrush);
					DeleteObject(hbrush);


					// 在进度条上绘制文本
					wchar_t szText[64];
					progressBarRc.left = rc.right - 50;
					progressBarRc.right = rc.right;
					swprintf_s(szText, L"%.1f", static_cast<float>((double)remainTime / 1000.0));

					DrawText(lplvcd->nmcd.hdc, szText, -1, &progressBarRc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					return CDRF_SKIPDEFAULT;
				}


				return CDRF_DODEFAULT;
			default:
				break;
			}
			return CDRF_DODEFAULT;

		}
		else if (pnm->hdr.code == NM_CLICK)
		{
			LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lParam;
			int i = lpnmitem->iItem;
			isOTPDIALOGEDIT = i;
			if (i >= 0) {
				EnableWindow(buttonEdit, TRUE);
				EnableWindow(buttonDelete, TRUE);
			}
			else {
				EnableWindow(buttonEdit, FALSE);
				EnableWindow(buttonDelete, FALSE);
			}
		}
		else if (pnm->hdr.code == NM_DBLCLK)
		{
			LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lParam;
			int i = lpnmitem->iItem;
			if (i >= 0) {
				OTPInfo o = otpInfos[i];
				if (o.type == HOTP)
				{
					//DialogBox(hInst, MAKEINTRESOURCE(IDD_HOTP), hWnd, HotpClientViewerProc);
					int hresult = MessageBox(hWnd, viewConfirm, confirm, MB_ICONQUESTION | MB_OKCANCEL);
					if (hresult == IDOK) {
						DialogBox(hInst, MAKEINTRESOURCE(IDD_HOTP), hWnd, HotpClientViewerProc);
					}
					//MessageBox(hWnd, L"计次验证，无法查看密码", L"提示", MB_ICONINFORMATION);

				}

			}
		}


	}
				  break;
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		switch (wmId) {
		case IDC_BUTTON_IMPORT:
			break;
		case IDC_BUTTON_EXPORT:
			break;
		case IDC_BUTTON_ADD:
			isOTPDIALOGEDIT = -1;
			DialogBox(hInst, MAKEINTRESOURCE(IDD_OTP), hWnd, OTP_Client);
			break;
		case IDC_BUTTON_EDIT:
			isOTPDIALOGEDIT = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_OTP), hWnd, OTP_Client);
			break;
		case IDC_BUTTON_DELETE:
		{
			if (isOTPDIALOGEDIT < 0) return FALSE;
			int MessageResult = MessageBox(hWnd, L"确定删除吗？", L"删除", MB_ICONQUESTION | MB_YESNO);
			if (MessageResult == IDYES)
			{
				int i = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
				ListView_DeleteItem(hListView, i);
				string guid = otpInfos[i].secret;
				otpInfos.erase(otpInfos.begin() + i);
				CurrentKeys.erase(guid);
				EncryptedDataMap.erase(guid);
				if (otpInfos.size() == 0) {
					EnableWindow(buttonEdit, FALSE);
					EnableWindow(buttonDelete, FALSE);
				}
			}
		}
		break;
		case IDC_BUTTON_NETWORK_TIME:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_SNTP), hWnd, timeServerManager);
			break;
		default:
			break;
		}
	}
				   break;
	case WM_DPICHANGED:
	{
		RECT* const prcNewWindow = (RECT*)lParam;
		SetWindowPos(hWnd,
			NULL,
			prcNewWindow->left,
			prcNewWindow->top,
			prcNewWindow->right - prcNewWindow->left,
			prcNewWindow->bottom - prcNewWindow->top,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
	break;
	case WM_SIZE:
		if (wParam == SIZE_RESTORED)
		{
			RedrawWindow(hListView, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
			RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
		}
		return TRUE;
	case WM_SETTINGCHANGE: {
		if (wParam == SPI_SETNONCLIENTMETRICS ||
			(lParam != NULL && std::wstring((LPCWSTR)lParam) == L"intl")) {
			setLangFromi18n();
			setLangTextFromi18n(hWnd);
		}
		break;
	}
	case WM_CTLCOLORSTATIC:
	{
		HDC hdc = (HDC)wParam;
		SetTextColor(hdc, RGB(0, 0, 0));
		SetBkMode(hdc, TRANSPARENT);
		return (INT_PTR)GetStockObject(NULL_BRUSH);
	}
	case WM_USER + 1:
		if (!IsIconic(hWnd))
			RedrawWindow(hListView, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
		//InvalidateRect(hListView, NULL, TRUE);
		//UpdateListViewProgress();
		break;
	case WM_USER + 2: {
		for (int i = 0; i < otpInfos.size(); i++)
		{
			OTPInfo o = otpInfos[i];
			wstring wPassword = L"";

			if (o.type == 0)
			{
				OTP otp(HOTP);
				otp.setAlgorithm(o.algorithm);
				int64_t times = getCurrentMillSecond(alwaysUseNetTime) / o.addition_param / 1000;
				string guid = o.secret;
				string password = otp.generateOTP(DecryptData(EncryptedDataMap[guid]), 1, o.digits, times);
				wPassword = padZero(s2ws(password), o.digits);

				CurrentKeys[guid] = times;

				//set new generated password to listview
				LVITEM lvi;
				lvi.iItem = i;
				lvi.iSubItem = 1;
				std::shared_ptr<WCHAR[]> wPassword_to_show(new WCHAR[32]);
				wcscpy_s(wPassword_to_show.get(), 32, wPassword.c_str());
				lvi.pszText = wPassword_to_show.get();
				ListView_SetItemText(hListView, i, 1, wPassword_to_show.get());

			}
			else
			{
				ListView_SetItemText(hListView, i, 1, viewTip);
			}

		}
		return TRUE;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);;
}

void AddControls(HWND hWnd) {
	// 创建ListView
	DWORD dwStyle = //WS_TABSTOP |
		WS_CHILD | LVS_SINGLESEL |
		WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
		LVS_REPORT;
	hListView = CreateWindow(WC_LISTVIEW, L"",
		dwStyle,
		10, 10, 600, 300,
		hWnd, NULL, hInst, NULL);
	ListView_SetExtendedListViewStyle(hListView, LVS_EX_DOUBLEBUFFER);

	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_WIDTH;
	lvc.cx = 200;
	lvc.pszText = L"名称";
	ListView_InsertColumn(hListView, 0, &lvc);
	lvc.pszText = L"数字密钥";
	ListView_InsertColumn(hListView, 1, &lvc);
	lvc.pszText = L"过期时间";
	ListView_InsertColumn(hListView, 2, &lvc);

	// 添加按钮
	buttonAdd = CreateWindow(L"BUTTON", L"新增", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		190, 320, 80, 30, hWnd, (HMENU)IDC_BUTTON_ADD, hInst, NULL);
	buttonEdit = CreateWindow(L"BUTTON", L"修改", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		280, 320, 80, 30, hWnd, (HMENU)IDC_BUTTON_EDIT, hInst, NULL);


	buttonDelete = CreateWindow(L"BUTTON", L"删除", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		370, 320, 80, 30, hWnd, (HMENU)IDC_BUTTON_DELETE, hInst, NULL);
	buttonNetworkTime = CreateWindow(L"BUTTON", L"是否使用网络时间", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		460, 320, 150, 30, hWnd, (HMENU)IDC_BUTTON_NETWORK_TIME, hInst, NULL);

	HWND CopyrightStatic = CreateWindowEx(WS_EX_TRANSPARENT, L"STATIC", (i18nClient::getInstence()->get("copyright")).c_str(), WS_CHILD | WS_VISIBLE | SS_LEFT,
		30, 360, 450, 25, hWnd, NULL, hInst, NULL);

	EnableWindow(buttonEdit, FALSE);
	EnableWindow(buttonDelete, FALSE);

	for (OTPInfo o : otpInfos)
	{
		addItem(hListView, o);
	}

	// 设置默认字体为微软雅黑
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = -MulDiv(9, GetDeviceCaps(GetDC(hWnd), LOGPIXELSY), 72);
	wcscpy_s(lf.lfFaceName, L"Microsoft YaHei");
	HFONT font = CreateFontIndirect(&lf);
	SendMessage(hWnd, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
	SendMessage(hListView, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));

	SendMessage(buttonAdd, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
	SendMessage(buttonDelete, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
	SendMessage(buttonEdit, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
	SendMessage(buttonNetworkTime, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
	SendMessage(CopyrightStatic, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));

	ListView_SetExtendedListViewStyle(hListView,
		LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	//// 解决双缓冲问题
	//SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_COMPOSITED);
}





//OTP Client IDD_OTP logics:

wstring s2ws(const string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	wstring r(buf);
	delete[] buf;
	return r;
}

void AlignButtons(HWND hwnd, HWND hRefButton, HWND hButtonToAlign1, HWND hButtonToAlign2) {
	RECT rectRefButton;
	RECT rectButtonToAlign1;
	RECT rectButtonToAlign2;


	if (GetWindowRect(hRefButton, &rectRefButton)) {

		ScreenToClient(hwnd, (LPPOINT)&rectRefButton.left);
		ScreenToClient(hwnd, (LPPOINT)&rectRefButton.right);


		if (GetWindowRect(hButtonToAlign1, &rectButtonToAlign1)) {

			ScreenToClient(hwnd, (LPPOINT)&rectButtonToAlign1.left);
			ScreenToClient(hwnd, (LPPOINT)&rectButtonToAlign1.right);


			SetWindowPos(hButtonToAlign1, NULL,
				rectButtonToAlign1.left, rectRefButton.top, 0, 0,
				SWP_NOZORDER | SWP_NOSIZE);
		}


		if (GetWindowRect(hButtonToAlign2, &rectButtonToAlign2)) {

			ScreenToClient(hwnd, (LPPOINT)&rectButtonToAlign2.left);
			ScreenToClient(hwnd, (LPPOINT)&rectButtonToAlign2.right);


			SetWindowPos(hButtonToAlign2, NULL,
				rectButtonToAlign2.left, rectRefButton.top, 0, 0,
				SWP_NOZORDER | SWP_NOSIZE);
		}
	}
}


void MoveButtonToBottomOfDialog(HWND hDlg, HWND hButton) {
	// 获取对话框的客户区矩形
	RECT dlgRect;
	GetClientRect(hDlg, &dlgRect);

	// 获取按钮的矩形
	RECT buttonRect;
	GetWindowRect(hButton, &buttonRect);
	POINT pt = { buttonRect.left, buttonRect.top };
	POINT pt2 = { buttonRect.right, buttonRect.bottom };
	ScreenToClient(hDlg, &pt);
	ScreenToClient(hDlg, &pt2);



	int buttonHeight = buttonRect.bottom - buttonRect.top;


	int newY = dlgRect.bottom - buttonHeight;


	int margin = 10;
	newY -= margin;


	SetWindowPos(hButton, NULL,
		pt.x, // 保持原来的 X 坐标
		newY,
		0, 0,
		SWP_NOZORDER | SWP_NOSIZE);
}

/**
* wide string to string
*/
string ws2s(std::wstring s)
{
	string result;
	int slength = (int)s.length() + 2;
	int len;
	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	char* buf = new char[len];
	WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0);
	result = buf;
	delete[] buf;
	return result;

}


/**
* OTP Client Dialog Procedure
* This Function is for OTP Client Dialog Event Handling
*/
INT_PTR CALLBACK OTP_Client(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static BOOL adv = false;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		adv = false;
		HWND hName = GetDlgItem(hDlg, IDC_OTPNAME);
		HWND hSecret = GetDlgItem(hDlg, IDC_OTPSECRET);
		HWND hAlgorithm = GetDlgItem(hDlg, IDC_ALGORITHM);
		HWND hIsHOTP = GetDlgItem(hDlg, IDC_CHECKISHOTP);
		HWND hDigitLength = GetDlgItem(hDlg, IDC_DIGITLENGTH);
		HWND hAdditionEdit = GetDlgItem(hDlg, IDC_ADDITIONEDIT);
		HWND hAdVancedButton = GetDlgItem(hDlg, IDC_ADVANCEDBUTTON);
		SendMessage(hAlgorithm, CB_ADDSTRING, 0, (LPARAM)L"SHA1");
		SendMessage(hDigitLength, TBM_SETRANGE, NULL, 0x00080004);
		wchar_t buffer[50];
		GetWindowText(hAdVancedButton, buffer, 50);
		wstring str = buffer;
		str[str.size() - 2] = L'>';
		str[str.size() - 1] = L'>';
		SetWindowTextW(hAdVancedButton, str.c_str());
		SendMessage(hAdditionEdit, EM_LIMITTEXT, (WPARAM)3, 0);
		HWND hDialogOK = GetDlgItem(hDlg, IDOK);
		HWND hDialogCancel = GetDlgItem(hDlg, IDCANCEL);
		AlignButtons(hDlg, hAdVancedButton, hDialogOK, hDialogCancel);
		if (isOTPDIALOGEDIT >= 0)
		{

			SetWindowText(hName, s2ws(otpInfos[isOTPDIALOGEDIT].friendly_name).c_str());

			SetWindowText(hSecret, s2ws(DecryptData(EncryptedDataMap[otpInfos[isOTPDIALOGEDIT].secret])).c_str());
			SetWindowText(hAdditionEdit, to_wstring(otpInfos[isOTPDIALOGEDIT].addition_param).c_str());
			SendMessage(hDigitLength, TBM_SETPOS, TRUE, otpInfos[isOTPDIALOGEDIT].digits);

			SendMessage(hAlgorithm, CB_SETCURSEL, 0, 0);
			SendMessage(hIsHOTP, BM_SETCHECK, otpInfos[isOTPDIALOGEDIT].type == HOTP ? BST_CHECKED : BST_UNCHECKED, 0);

			int res = 0x00040008;


			if (!adv && isOTPDIALOGEDIT <= 0 && HIWORD(wParam) == BN_CLICKED)
			{
				HWND hAdditionEdit = GetDlgItem(hDlg, IDC_ADDITIONEDIT);
				if (IsDlgButtonChecked(hDlg, IDC_CHECKISHOTP) == BST_CHECKED)
				{
					SetWindowText(hAdditionEdit, L"0");
				}
				else
				{
					SetWindowText(hAdditionEdit, L"30");
				}
			}
			HWND hStatic = GetDlgItem(hDlg, IDC_HOTPTOTPADDSTATIC);
			if (IsDlgButtonChecked(hDlg, IDC_CHECKISHOTP) == BST_CHECKED)
			{
				SetWindowText(hStatic, counter);
			}
			else
			{
				SetWindowText(hStatic, interval);
			}

			RECT rect = { 0,0,MulDiv(450, GetDpiForWindow(hDlg), 96) ,MulDiv(160, GetDpiForWindow(hDlg), 96) };
			/*MulDiv(311, GetDpiForWindow(hDlg), 96);*/
			AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU, FALSE);

			SetWindowPos(hDlg, HWND_TOPMOST, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE);
		}
		else
		{
			RECT rect = { 0,0,MulDiv(450, GetDpiForWindow(hDlg), 96) ,MulDiv(160, GetDpiForWindow(hDlg), 96) };
			AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU, FALSE);
			SetWindowPos(hDlg, HWND_TOPMOST, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE);
			//HWND hAlgorithm = GetDlgItem(hDlg, IDC_ALGORITHM);
			//GetWindowRect(hAdVancedButton, &rect);

			RECT OKRect, CancelRect;
			ShowWindow(hAlgorithm, FALSE);
			ShowWindow(hDigitLength, FALSE);
			ShowWindow(hAdditionEdit, FALSE);
			SendMessage(hDigitLength, TBM_SETPOS, TRUE, 6);
			SendMessageW(hAlgorithm, CB_SETCURSEL, 0, 0);
			SetWindowText(hAdditionEdit, L"30");

			SendMessage(hAlgorithm, CB_SETCURSEL, 0, 0);
		}

	}
	return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		switch LOWORD(wParam)
		{
		case IDC_ADVANCEDBUTTON:
		{
			HWND hAlgorithm = GetDlgItem(hDlg, IDC_ALGORITHM);
			HWND hDigitLength = GetDlgItem(hDlg, IDC_DIGITLENGTH);
			HWND hAdditionEdit = GetDlgItem(hDlg, IDC_ADDITIONEDIT);
			HWND hAdvancedButton = GetDlgItem(hDlg, IDC_ADVANCEDBUTTON);

			wchar_t buffer[50];
			GetWindowText(hAdvancedButton, buffer, 50);
			wstring str = buffer;
			HWND hDialogOK = GetDlgItem(hDlg, IDOK);
			HWND hDialogCancel = GetDlgItem(hDlg, IDCANCEL);

			if (str[str.size() - 2] == L'>')
			{
				str[str.size() - 2] = L'<';
				str[str.size() - 1] = L'<';
				RECT rect = { 0,0,MulDiv(450, GetDpiForWindow(hDlg), 96) ,MulDiv(250, GetDpiForWindow(hDlg), 96) };
				AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU, TRUE);
				SetWindowPos(hDlg, HWND_TOPMOST, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE);

				//MoveButtonToBottomOfDialog(hDlg, hAdvancedButton);
				MoveButtonToBottomOfDialog(hDlg, hDialogOK);
				MoveButtonToBottomOfDialog(hDlg, hDialogCancel);

				ShowWindow(hAlgorithm, TRUE);
				ShowWindow(hDigitLength, TRUE);
				adv = true;
				ShowWindow(hAdditionEdit, TRUE);
			}
			else
			{
				str[str.size() - 2] = L'>';
				str[str.size() - 1] = L'>';
				RECT rect = { 0,0,MulDiv(450, GetDpiForWindow(hDlg), 96) ,MulDiv(140, GetDpiForWindow(hDlg), 96) };
				AdjustWindowRect(&rect, WS_CAPTION | WS_SYSMENU, TRUE);

				AlignButtons(hDlg, hAdvancedButton, hDialogOK, hDialogCancel);
				SetWindowPos(hDlg, HWND_TOPMOST, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE);
				ShowWindow(hAlgorithm, FALSE);
				ShowWindow(hDigitLength, FALSE);
				ShowWindow(hAdditionEdit, FALSE);
			}
			SetWindowTextW(hAdvancedButton, str.c_str());
		}

		break;
		case IDC_CHECKISHOTP:
		{
			if (!adv && isOTPDIALOGEDIT <= 0 && HIWORD(wParam) == BN_CLICKED)
			{
				HWND hAdditionEdit = GetDlgItem(hDlg, IDC_ADDITIONEDIT);
				if (IsDlgButtonChecked(hDlg, IDC_CHECKISHOTP) == BST_CHECKED)
				{
					SetWindowText(hAdditionEdit, L"0");
				}
				else
				{
					SetWindowText(hAdditionEdit, L"30");
				}
			}
			HWND hStatic = GetDlgItem(hDlg, IDC_HOTPTOTPADDSTATIC);
			if (IsDlgButtonChecked(hDlg, IDC_CHECKISHOTP) == BST_CHECKED)
			{
				SetWindowText(hStatic, counter);
			}
			else
			{
				SetWindowText(hStatic, interval);
			}
			break;
		}
		//if()
		case IDOK:
		{
			HWND hName = GetDlgItem(hDlg, IDC_OTPNAME);
			HWND hSecret = GetDlgItem(hDlg, IDC_OTPSECRET);
			HWND hAlgorithm = GetDlgItem(hDlg, IDC_ALGORITHM);
			HWND hIsHOTP = GetDlgItem(hDlg, IDC_CHECKISHOTP);
			HWND hDigitLength = GetDlgItem(hDlg, IDC_DIGITLENGTH);
			HWND hAdditionEdit = GetDlgItem(hDlg, IDC_ADDITIONEDIT);


			OTPInfo otpinfo;
			int trackBarValue = SendMessage(hDigitLength, TBM_GETPOS, 0, 0);

			// 获取 Edit 控件的当前值
			TCHAR edit1Text[1024];
			GetWindowText(hName, edit1Text, sizeof(edit1Text) / sizeof(TCHAR));

			TCHAR edit2Text[1024];
			GetWindowText(hSecret, edit2Text, sizeof(edit2Text) / sizeof(TCHAR));

			TCHAR edit3Text[1024];
			GetWindowText(hAdditionEdit, edit3Text, sizeof(edit3Text) / sizeof(TCHAR));

			// 获取 CheckBox 的当前状态
			int checkBoxState = IsDlgButtonChecked(hDlg, IDC_CHECKISHOTP);


			// 获取 ComboBox 的当前选择项
			int comboBoxIndex = SendMessage(hAlgorithm, CB_GETCURSEL, 0, 0)+1;
			//TCHAR comboBoxText[100] = L"";
			//SendMessage(hAlgorithm, CB_GETLBTEXT, comboBoxIndex, (LPARAM)comboBoxText);

			otpinfo.algorithm = comboBoxIndex;
			otpinfo.digits = trackBarValue;
			otpinfo.friendly_name = ws2s(edit1Text);
			otpinfo.secret = ws2s(edit2Text);
			otpinfo.addition_param = _wtoi(edit3Text);
			otpinfo.type = checkBoxState == BST_CHECKED;

			if (otpinfo.type == 0 && otpinfo.addition_param == 0)
			{
				MessageBox(hDlg, errorTimeEqualsZero, error, MB_ICONWARNING | MB_OK);
				otpinfo.addition_param = 30;
			}

			if (otpinfo.secret.empty())
			{
				MessageBox(hDlg, errorNoSecret, error, MB_ICONERROR | MB_OK);
				return (INT_PTR)TRUE;
			}

			for (wchar_t i : otpinfo.secret)
			{
				if (!(i >= '2' && i <= '7' || i >= 'A' && i <= 'Z'))
				{
					MessageBox(hDlg, errorErrorEncodeBase32, error, MB_ICONERROR | MB_OK);

					return (INT_PTR)TRUE;
				}
			}

			if (isOTPDIALOGEDIT >= 0)
			{

				string guidText = otpInfos[isOTPDIALOGEDIT].secret;

				//string s = otpinfo.secret;
				CurrentKeys[guidText] = 0;
				EncryptedDataMap[guidText] = EncryptData(otpinfo.secret);
				otpinfo.secret = guidText;
				otpInfos[isOTPDIALOGEDIT] = otpinfo;
				if (otpinfo.type == 1)
				{
					ListView_SetItemText(hListView, isOTPDIALOGEDIT, 1, viewTip);
				}
			}
			else
			{
				GUID guid;
				HRESULT hr = CoCreateGuid(&guid);
				TCHAR guidTEXT[96];
				hr = StringFromGUID2(guid, guidTEXT, 96);
				string s = ws2s(guidTEXT);

				CurrentKeys.emplace(s, 0);


				EncryptedDataMap[s] = EncryptData(otpinfo.secret);
				otpinfo.secret = s;
				otpInfos.push_back(otpinfo);


				//EnableWindow(buttonEdit, TRUE);
				addItem(hListView, otpinfo);
			}
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		}

		break;
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK HotpClientViewerProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		// Create a larger font for the password, it has 6-8 digits, and takes up most of the dialog
		DWORD dwFontSize = 100;
		HFONT hfont = CreateFont(dwFontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Consolas");
		SendMessage(GetDlgItem(hDlg, IDC_PASSWORD), WM_SETFONT, (WPARAM)hfont, TRUE);
		OTP otp(HOTP);
		otp.setAlgorithm(otpInfos[isOTPDIALOGEDIT].algorithm);
		string guidText = otpInfos[isOTPDIALOGEDIT].secret;
		string currentSecret = DecryptData(EncryptedDataMap[guidText]);

		string digitalPassword = otp.generateOTP(currentSecret, 1, otpInfos[isOTPDIALOGEDIT].digits, otpInfos[isOTPDIALOGEDIT].addition_param);

		wstring convertedStr = padZero(s2ws(digitalPassword), otpInfos[isOTPDIALOGEDIT].digits);
		convertedStr.insert(convertedStr.begin() + convertedStr.size() / 2, L' ');

		SetWindowText(GetDlgItem(hDlg, IDC_PASSWORD), convertedStr.c_str());
		otpInfos[isOTPDIALOGEDIT].addition_param++;
		currentSecret.clear();
	}
	return (INT_PTR)TRUE;
	break;
	case WM_COMMAND:
	{
		switch LOWORD(wParam)
		{
		case IDOK:
		{

			EndDialog(hDlg, LOWORD(wParam));
			//otpInfos[isOTPDIALOGEDIT].addition_param++;
			EnableWindow(buttonEdit, FALSE);
			EnableWindow(buttonDelete, FALSE);
			isOTPDIALOGEDIT = -1;
			return (INT_PTR)TRUE;
		}
		case IDCANCEL:
		{

			EndDialog(hDlg, LOWORD(wParam));

			EnableWindow(buttonEdit, FALSE);
			EnableWindow(buttonDelete, FALSE);
			isOTPDIALOGEDIT = -1;
			return (INT_PTR)TRUE;
		}
		}
	}
	return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
}

int stopThisResyncThread = 0;
/**
* SNTP Resync Procedure
* This Function is for SNTP Resync Procedure, it will update the time from the server
**/
DWORD WINAPI sntpResyncProc(LPVOID lpParameter)
{
	HWND hWnd = (HWND)lpParameter;
	stopThisResyncThread = 0;
	if (sntpServers.size() > 0)
		if (sntpClient.updateSNTPTimeStamp(sntpServers[0].c_str(), 123, false) >= 0 || sntpClient.updateSNTPTimeStamp(sntpServers[0].c_str(), 123, true) >= 0)
		{
			if(hWnd!=NULL && !stopThisResyncThread)
			    PostMessage(hWnd, WM_USER, NULL, NULL);
			return 0;
		}
		else if (sntpServers.size() > 1)
		{
			if (sntpClient.updateSNTPTimeStamp(sntpServers[1].c_str(), 123, false) >= 0 || sntpClient.updateSNTPTimeStamp(sntpServers[1].c_str(), 123, true) >= 0)
			{
				if (hWnd != NULL && !stopThisResyncThread)
				    PostMessage(hWnd, WM_USER, NULL, NULL);
				return 0;
			}
		}
	if (hWnd != NULL)
		PostMessage(hWnd, WM_USER, NULL, NULL);
	return 0;

}

INT_PTR CALLBACK timeServerManager(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	static HANDLE miniThread;
	HWND primaryServer = GetDlgItem(hDlg, IDC_PRIMARYSERVER);
	HWND secondaryServer = GetDlgItem(hDlg, IDC_SECONDARYSERVER);
	HWND alwaysUseNetworkTime = GetDlgItem(hDlg, IDC_ALWAYSNETTIME);
	HWND IDCSNTPSTATUS = GetDlgItem(hDlg, IDC_STATICSYNC);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		if (sntpServers.size() > 0)SetWindowText(primaryServer, s2ws(sntpServers[0]).c_str());
		if (sntpServers.size() > 1)SetWindowText(secondaryServer, s2ws(sntpServers[1]).c_str());

		SendMessage(alwaysUseNetworkTime, BM_SETCHECK, alwaysUseNetTime, NULL);

		//otpInfos[isOTPDIALOGEDIT].addition_param++;
		/*RECT rect = { 0,0,MulDiv(310, GetDpiForWindow(hDlg), 96) ,MulDiv(177, GetDpiForWindow(hDlg), 96) };
		SetWindowPos(hDlg, HWND_TOPMOST, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE);*/
	}
	return (INT_PTR)TRUE;
	break;
	case WM_COMMAND:
	{
		switch LOWORD(wParam)
		{

		case IDOK:
		{
			sntpServers.resize(2);
			WCHAR buffer[256];
			GetWindowText(primaryServer, buffer, 256);
			sntpServers[0] = ws2s(buffer);
			GetWindowText(secondaryServer, buffer, 256);
			sntpServers[1] = ws2s(buffer);
			alwaysUseNetTime = IsDlgButtonChecked(hDlg, IDC_ALWAYSNETTIME) == BST_CHECKED;

			int8_t lastUpdateTime = (getCurrentMillSecond() - sntpClient.getLastUpdate()) / 60000;
			if (sntpClient.getStatus() > 0 && lastUpdateTime < 1200000) {
				swprintf_s(buffer, lastSync, lastUpdateTime);
				SetWindowText(IDCSNTPSTATUS, buffer);

			}
			else
			{
				SetWindowText(IDCSNTPSTATUS, lastSyncFailed);
			}
			stopThisResyncThread = 1;
			EndDialog(hDlg, LOWORD(wParam));
			//otpInfos[isOTPDIALOGEDIT].addition_param++;
			EnableWindow(buttonEdit, FALSE);
			EnableWindow(buttonDelete, FALSE);
			isOTPDIALOGEDIT = -1;
			return (INT_PTR)TRUE;
		}
		case IDCANCEL:
		{
			stopThisResyncThread = 1;
			EndDialog(hDlg, LOWORD(wParam));

			EnableWindow(buttonEdit, FALSE);
			EnableWindow(buttonDelete, FALSE);
			isOTPDIALOGEDIT = -1;
			return (INT_PTR)TRUE;
		}
		case IDC_RESYNC:
		{
			//otpInfos[isOTPDIALOGEDIT].addition_param++;
			HWND reSync = GetDlgItem(hDlg, IDC_RESYNC);
			EnableWindow(reSync, FALSE);
			sntpServers.resize(2);
			WCHAR buffer[256];
			GetWindowText(primaryServer, buffer, 256);
			sntpServers[0] = ws2s(buffer);
			GetWindowText(secondaryServer, buffer, 256);
			sntpServers[1] = ws2s(buffer);
			miniThread = CreateThread(NULL, 0, sntpResyncProc, hDlg, 0, NULL);
			alwaysUseNetTime = IsDlgButtonChecked(hDlg, IDC_ALWAYSNETTIME) == BST_CHECKED;

			return (INT_PTR)TRUE;
		}
		default:
			return (INT_PTR)TRUE;

		}
		break;
	case WM_USER:
	{
		HWND reSync = GetDlgItem(hDlg, IDC_RESYNC);
		WCHAR buffer[256];
		int8_t lastUpdateTime = (getCurrentMillSecond() - sntpClient.getLastUpdate()) / 60000;
		if (sntpClient.getStatus() >= 0 && lastUpdateTime < 1200000) {

			swprintf_s(buffer, lastSync, lastUpdateTime);
			SetWindowText(IDCSNTPSTATUS, buffer);
		}
		else
		{
			SetWindowText(IDCSNTPSTATUS, lastSyncFailed);
		}
		EnableWindow(reSync, TRUE);
	}
	}
	return (INT_PTR)TRUE;
	}
	return (INT_PTR)FALSE;
}

