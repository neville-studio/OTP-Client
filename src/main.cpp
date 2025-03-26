#include <iostream>
#include "sntp.h"
#include "core\crypto\sha224.h"
//#include "i18n.h"
using namespace std;
#ifdef WIN32
#include <Windows.h>
#include "ui\win32\MainWinForm.h"
//#include "ui\winui3\WinUI3Entry.h"

/**
*  @brief Check if the current Windows version is Windows 10 version 1809 or greater. This function is used to determine whether to use WinUI 3 or WinForm.
 *
 * 
 * @return true if the current Windows version is Windows 10 version 1809 or greater
 * @return false if the current Windows version is not Windows 10 version 1809 or greater
 */



int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	
	SHA224 sha224;
	//OutputDebugStringA(sha224.getHashMessage("AAAAAAAA", 2, 2).c_str());
	//OutputDebugStringA(sha224.getHashMessage("AAAAAAAA", 1, 1).c_str());
	OutputDebugStringA(sha224.getHashMessage("616263", 3, 3).c_str());






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