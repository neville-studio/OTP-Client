#include <iostream>
#include "sntp.h"
using namespace std;

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