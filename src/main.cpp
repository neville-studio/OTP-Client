#include <iostream>
#include "sntp.h"
//#include "i18n.h"
using namespace std;
#ifdef WIN32
#include <Windows.h>
#include "ui\win32\MainWinForm.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	
	return mainWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}




#else
int main() {
	cout << "Hello, CMake" << endl;

	SNTPClient sntp;
	int64_t timestamp = sntp.getSNTPTimestamp("time.windows.com", 123);
	cout << "Timestamp: " << timestamp << endl;
	//SHA1 sha1;
	/*sha1.messageByBase32("MFRGGZDFMZTWQ2LK");*/

	//cout << sha1.getHashMessage("YWJjZGVmZ2hpamts",2,2) << endl;
	return 0;
}
#endif