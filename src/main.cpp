#include <iostream>
#include "hmac.h"
using namespace std;

int main() {
	cout << "Hello, CMake" << endl;

	HMAC hmac;
	hmac.setHashAlgorithm(AlgorithmName::SHA_1);
	cout << hmac.getHashMessage("YWJjZGVmZ2hpamts", 2, "YWJjZGVmZ2hpamts") << endl;
	//SHA1 sha1;
	/*sha1.messageByBase32("MFRGGZDFMZTWQ2LK");*/
	
	//cout << sha1.getHashMessage("YWJjZGVmZ2hpamts",2,2) << endl;
	return 0;
}