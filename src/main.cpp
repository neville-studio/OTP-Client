#include <iostream>
#include "sntp.h"
#include "core\crypto\sha3.h"
using namespace std;
#ifdef _WIN32
#include <Windows.h>
#include "ui\win32\MainWinForm.h"


/**
*  @brief Check if the current Windows version is Windows 10 version 1809 or greater. This function is used to determine whether to use WinUI 3 or WinForm.
 *
 * 
 * @return true if the current Windows version is Windows 10 version 1809 or greater
 * @return false if the current Windows version is not Windows 10 version 1809 or greater
 */
//bool IsWindows10Version17763OrGreater() {
//	OSVERSIONINFOEX osvi = { sizeof(OSVERSIONINFOEX), 0, 0, 0, 0, {0}, 0, 0, 0, VER_NT_WORKSTATION, 0 };
//	DWORDLONG conditionMask = 0;
//	int op = VER_GREATER_EQUAL;
//
//	// 设置操作系统版本号为 10.0.17763
//	osvi.dwMajorVersion = 10;
//	osvi.dwMinorVersion = 0;
//	osvi.dwBuildNumber = 17763;
//
//	VER_SET_CONDITION(conditionMask, VER_MAJORVERSION, op);
//	VER_SET_CONDITION(conditionMask, VER_MINORVERSION, op);
//	VER_SET_CONDITION(conditionMask, VER_BUILDNUMBER, op);
//
//	return VerifyVersionInfo(&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER, conditionMask);
//}


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	//test_SHA3();
	return mainWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}




#else
int main() {
	cout << "Hello, CMake" << endl;

	//SNTPClient sntp;
	//int64_t timestamp = sntp.getSNTPTimestamp("time.windows.com", 123);
	//cout << "Timestamp: " << timestamp << endl;
	//SHA1 sha1;
	/*sha1.messageByBase32("MFRGGZDFMZTWQ2LK");*/

	//cout << sha1.getHashMessage("YWJjZGVmZ2hpamts",2,2) << endl;
	return 0;
}
#endif