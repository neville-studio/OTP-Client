#include "MainWinForm.h"


#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 100
HINSTANCE hInst;
HWND buttonImport, buttonExport, buttonAdd, buttonEdit, buttonDelete, buttonNetworkTime;
HWND hListView;
WCHAR interval[MAX_LOADSTRING];
WCHAR counter[MAX_LOADSTRING];

GlobalConfiguration *globalConfig = GlobalConfiguration :: getInstance();
vector<OTPInfo> otpInfos = globalConfig->getOTPConfig();
vector<string> sntpServers = globalConfig->getSNTPServers();
INT_PTR CALLBACK OTP_Client(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT isOTPDIALOGEDIT = -2;

void setLangTextFromi18n(HWND hWnd) {
	i18nClient* i18n = i18nClient::getInstence();
	SetWindowText(hWnd, i18n->get("windowTitle").c_str());
	SetWindowText(buttonImport, i18n->get("import").c_str());
	SetWindowText(buttonExport, i18n->get("export").c_str());
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
    
    if (IsWindowsVistaOrGreater())
    {
		SetProcessDPIAware();
    }
    HWND hWnd = CreateWindow(L"MainWinForm", L"OTP客户端", WS_OVERLAPPEDWINDOW ^WS_MAXIMIZE ^ WS_MAXIMIZEBOX ^ WS_SIZEBOX,
        CW_USEDEFAULT, 0, 650,400, NULL, NULL, hInstance, NULL);

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        AddControls(hWnd);
		setLangTextFromi18n(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	case WM_NOTIFY:
        if (((LPNMHDR)lParam)->idFrom == 0 && ((LPNMHDR)lParam)->code == NM_CUSTOMDRAW) {
            LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)lParam;

            switch (lplvcd->nmcd.dwDrawStage) {
            case CDDS_PREPAINT:
                return CDRF_NOTIFYITEMDRAW;

            case CDDS_ITEMPREPAINT:
                return CDRF_NOTIFYSUBITEMDRAW;

            case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
                if (lplvcd->iSubItem == 2) { // 第三列（过期时间）
                    RECT rect;
                    ListView_GetSubItemRect(lplvcd->nmcd.hdr.hwndFrom, lplvcd->nmcd.dwItemSpec, 2, LVIR_BOUNDS, &rect);
                    HDC hdc = lplvcd->nmcd.hdc;

                    int progressWidth = ( time(NULL) % 30) / 30;
                    HBRUSH hBrush = CreateSolidBrush(RGB(0, 128, 0)); // 绿色
                    FillRect(hdc, &rect, hBrush);
                    DeleteObject(hBrush);

                    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
                    SelectObject(hdc, hPen);
                    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
                    DeleteObject(hPen);

                    char buffer[32];
                    sprintf_s(buffer,32, "%d", int(time(NULL) % 30));
                    DrawTextA(hdc, buffer, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                    return CDRF_SKIPDEFAULT;
                }
                break;
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
			break;
		case IDC_BUTTON_NETWORK_TIME:
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
    case WM_SETTINGCHANGE: {
        if (wParam == SPI_SETNONCLIENTMETRICS ||
            (lParam != NULL && std::wstring((LPCWSTR)lParam) == L"intl")) {
			setLangFromi18n();
			setLangTextFromi18n(hWnd);
        }
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void AddControls(HWND hWnd) {
    // 创建ListView
    DWORD dwStyle = //WS_TABSTOP |
        WS_CHILD | WS_CLIPCHILDREN |
        WS_BORDER |
        WS_VISIBLE |
        LVS_SINGLESEL | 
        LVS_REPORT;
    hListView = CreateWindowEx(LVS_EX_DOUBLEBUFFER,  // ex style
        WC_LISTVIEW,                     // class name - defined in commctrl.h
        TEXT(""),                        // dummy text
        dwStyle,                         // style
        10,                              // x position
        10,                             // y position
        600,                             // width
        300,                             // height
        hWnd,                            // parent
        (HMENU)110,                       // ID
        hInst,                           // instance
        NULL);

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
    buttonImport = CreateWindow(L"BUTTON", L"导入", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 320, 80, 30, hWnd, (HMENU)IDC_BUTTON_IMPORT, hInst, NULL);
    buttonExport = CreateWindow(L"BUTTON", L"导出", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        100, 320, 80, 30, hWnd, (HMENU)IDC_BUTTON_EXPORT, hInst, NULL);
    buttonAdd = CreateWindow(L"BUTTON", L"新增", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        190, 320, 80, 30, hWnd, (HMENU)IDC_BUTTON_ADD, hInst, NULL);
    buttonEdit = CreateWindow(L"BUTTON", L"修改", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        280, 320, 80, 30, hWnd, (HMENU)IDC_BUTTON_EDIT, hInst, NULL);
	if (otpInfos.size() == 0)
		EnableWindow(buttonEdit, FALSE);

    buttonDelete = CreateWindow(L"BUTTON", L"删除", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        370, 320, 80, 30, hWnd, (HMENU)IDC_BUTTON_DELETE, hInst, NULL);
    buttonNetworkTime = CreateWindow(L"BUTTON", L"是否使用网络时间", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        460, 320, 150, 30, hWnd, (HMENU)IDC_BUTTON_NETWORK_TIME, hInst, NULL);

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
    SendMessage(buttonExport, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
    SendMessage(buttonImport, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));
    SendMessage(buttonNetworkTime, WM_SETFONT, (WPARAM)font, MAKELPARAM(TRUE, 0));


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

    // 获取参考按钮的矩形
    if (GetWindowRect(hRefButton, &rectRefButton)) {
        // 将屏幕坐标转换为客户区坐标
        ScreenToClient(hwnd, (LPPOINT)&rectRefButton.left);
        ScreenToClient(hwnd, (LPPOINT)&rectRefButton.right);

        // 获取需要对齐的按钮1的矩形
        if (GetWindowRect(hButtonToAlign1, &rectButtonToAlign1)) {
            // 将屏幕坐标转换为客户区坐标
            ScreenToClient(hwnd, (LPPOINT)&rectButtonToAlign1.left);
            ScreenToClient(hwnd, (LPPOINT)&rectButtonToAlign1.right);

            // 使用SetWindowPos设置按钮1的新位置，保持其宽度和高度不变
            SetWindowPos(hButtonToAlign1, NULL, 
                         rectButtonToAlign1.left, rectRefButton.top, 0,0,
                         SWP_NOZORDER| SWP_NOSIZE);
        }

        // 获取需要对齐的按钮2的矩形
        if (GetWindowRect(hButtonToAlign2, &rectButtonToAlign2)) {
            // 将屏幕坐标转换为客户区坐标
            ScreenToClient(hwnd, (LPPOINT)&rectButtonToAlign2.left);
            ScreenToClient(hwnd, (LPPOINT)&rectButtonToAlign2.right);

            // 使用SetWindowPos设置按钮2的新位置，保持其宽度和高度不变
            SetWindowPos(hButtonToAlign2, NULL, 
                         rectButtonToAlign2.left, rectRefButton.top, 0,0,
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


    // 计算按钮的新 Y 坐标
    int buttonHeight = buttonRect.bottom - buttonRect.top;

    // 对话框的高度减去按钮的高度，作为按钮的 Y 坐标
    int newY = dlgRect.bottom - buttonHeight;

    // 为了美观，可以留一些边距（例如5像素）
    int margin = 10;
    newY -= margin;

    // 设置按钮的新位置，保持 X 坐标、宽度和高度不变
    SetWindowPos(hButton, NULL,
        pt.x, // 保持原来的 X 坐标
        newY,
        0,0,
        SWP_NOZORDER | SWP_NOSIZE);
}


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
		SendMessage(hAlgorithm, CB_ADDSTRING, 0, (LPARAM)L"SHA1");
        SendMessage(hDigitLength, TBM_SETRANGE, NULL, 0x00080004);

        if (isOTPDIALOGEDIT>0)
        {
			
			SetWindowText(hName, s2ws(otpInfos[isOTPDIALOGEDIT].friendly_name).c_str());
			SetWindowText(hSecret, s2ws(otpInfos[isOTPDIALOGEDIT].friendly_name).c_str());
			SetWindowText(hAdditionEdit, to_wstring(otpInfos[isOTPDIALOGEDIT].addition_param).c_str());
			SendMessage(hDigitLength, TBM_SETPOS, TRUE, otpInfos[isOTPDIALOGEDIT].digits);
            int res = 0x00040008;
			
			SendMessage(hAlgorithm, CB_SETCURSEL, 0, 0);
			

            RECT rect = { 0,0,MulDiv(450, GetDpiForWindow(hDlg), 96) ,MulDiv(140, GetDpiForWindow(hDlg), 96) };
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
			HWND hAdVancedButton = GetDlgItem(hDlg, IDC_ADVANCEDBUTTON);
			//GetWindowRect(hAdVancedButton, &rect);
			HWND hDialogOK = GetDlgItem(hDlg, IDOK);
			HWND hDialogCancel = GetDlgItem(hDlg, IDCANCEL);

			wchar_t buffer[50];
            GetWindowText(hAdVancedButton, buffer, 50);
			wstring str = buffer;
			str[str.size() - 2] = L'>';
            str[str.size() - 1] = L'>';
			SetWindowTextW(hAdVancedButton, str.c_str());
            

			RECT OKRect, CancelRect;
            ShowWindow(hAlgorithm, FALSE);
			ShowWindow(hDigitLength, FALSE);
            ShowWindow(hAdditionEdit, FALSE);
            SendMessage(hDigitLength, TBM_SETPOS, TRUE, 6);
			SendMessageW(hAlgorithm, CB_SETCURSEL, 0, 0);
			SetWindowText(hAdditionEdit, L"30");
            AlignButtons(hDlg, hAdVancedButton,hDialogOK, hDialogCancel);
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
            int comboBoxIndex = SendMessage(hAlgorithm, CB_GETCURSEL, 0, 0);
            TCHAR comboBoxText[100]=L"";
            SendMessage(hAlgorithm, CB_GETLBTEXT, comboBoxIndex, (LPARAM)comboBoxText);

			otpinfo.algorithm = ws2s(comboBoxText);
			otpinfo.digits = trackBarValue;
			otpinfo.friendly_name = ws2s(edit1Text);
			otpinfo.secret = ws2s(edit2Text);
			otpinfo.addition_param = _wtoi(edit3Text);
			otpinfo.type = checkBoxState == BST_CHECKED;

            if (isOTPDIALOGEDIT >= 0)
            {
				otpInfos[isOTPDIALOGEDIT] = otpinfo;
			}
			else
			{
				otpInfos.push_back(otpinfo);
            }
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        }
       
		break;
	}
	return (INT_PTR)FALSE;
}