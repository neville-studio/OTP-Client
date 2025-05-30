
#include "DataHandler.h"

#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "crypt32.lib")
using namespace std;
bool WriteBytesToFile(const std::string& filePath, const std::vector<BYTE>& byteData) {
	// 打开或创建文件
	HANDLE hFile = CreateFileA(
		filePath.c_str(),                
		GENERIC_WRITE,                   
		0,                               
		NULL,                            
		CREATE_ALWAYS,                   
		FILE_ATTRIBUTE_NORMAL,           
		NULL                             
	);

	if (hFile == INVALID_HANDLE_VALUE) {
		//std::cerr << "Failed to create or open file. Error code: " << GetLastError() << std::endl;
		return false;
	}

	// 写入数据
	DWORD bytesWritten;
	BOOL result = WriteFile(
		hFile,                           
		byteData.data(),                 
		static_cast<DWORD>(byteData.size()), 
		&bytesWritten,                   
		NULL                              
	);

	if (!result || bytesWritten != byteData.size()) {
		//std::cerr << "Failed to write data to file. Error code: " << GetLastError() << std::endl;
		CloseHandle(hFile);
		return false;
	}

	// 关闭文件句柄
	CloseHandle(hFile);

	return true;
}

std::vector<BYTE> ReadDataFromFile(const std::string& filePath) {
	// 打开文件
	HANDLE hFile = CreateFileA(
		filePath.c_str(),                
		GENERIC_READ,                    
		0,                               
		NULL,                            
		OPEN_EXISTING,                   
		FILE_ATTRIBUTE_NORMAL,           
		NULL                             
	);
	if (hFile == INVALID_HANDLE_VALUE) {
		//std::cerr << "Failed to open file. Error code: " << GetLastError() << std::endl;
		return std::vector<BYTE>();
	}
	// 获取文件大小
	DWORD fileSize = GetFileSize(hFile, NULL);
	if (fileSize == INVALID_FILE_SIZE) {
		//std::cerr << "Failed to get file size. Error code: " << GetLastError() << std::endl;
		CloseHandle(hFile);
		return std::vector<BYTE>();
	}
	// 读取数据
	std::vector<BYTE> byteData(fileSize);
	DWORD bytesRead;
	BOOL result = ReadFile(
		hFile,                           // 文件句柄
		byteData.data(),                 // 读取数据的缓冲区
		fileSize,                        // 要读取的字节数
		&bytesRead,                      // 实际读取的字节数
		NULL                             // 不使用重叠结构
	);
	if (!result || bytesRead != fileSize) {
		//std::cerr << "Failed to read data from file. Error code: " << GetLastError() << std::endl;
		CloseHandle(hFile);
		return std::vector<BYTE>();
	}
	// 关闭文件句柄
	CloseHandle(hFile);
	return byteData;
}



std::string encodeBase64FromBYTE(vector<BYTE> in)
{
	std::string out;
	int val = 0, valb = -6;
	for (BYTE c : in) {
		val = (val << 8) + c;
		valb += 8;
		while (valb >= 0) {
			out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
	while (out.size() % 4) out.push_back('=');
	return out;
}

vector<BYTE> decodeBase64ToBYTE(string in)
{
	vector<BYTE> out;
	vector<int> T(256, -1);
	for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;
	int val = 0, valb = -8;
	for (char c : in) {
		if (T[c] == -1) break;
		val = (val << 6) + T[c];
		valb += 6;
		if (valb >= 0) {
			out.push_back((val >> valb) & 0xFF);
			valb -= 8;
		}
	}
	return out;
}


std::wstring ReadDataFromFile(){
	// Read data from file
	return L"";
}

void WriteDataToFile(std::wstring DATA) {
	// Write data to file
}


std::vector<BYTE> EncryptData(wstring data) {
	DATA_BLOB DataIn, dataOut;

	DataIn.pbData = reinterpret_cast<BYTE*>(const_cast<wchar_t*>(data.data()));
	DataIn.cbData = data.size() * sizeof(wchar_t);
	
	BOOL result = CryptProtectData(&DataIn, NULL, NULL, NULL, NULL, 0, &dataOut);
	if (!result) {
		return vector<BYTE>();
	} 
	std::vector<BYTE> res(dataOut.pbData, dataOut.pbData + dataOut.cbData);
	LocalFree(dataOut.pbData);
	return res;
}
std::string DecryptData(std::vector<BYTE> data) {
	DATA_BLOB DataIn, dataOut;

	DataIn.pbData = const_cast<BYTE *>(data.data());
	DataIn.cbData = data.size() * sizeof(char);

	BOOL result = CryptUnprotectData(&DataIn, NULL, NULL, NULL, NULL, 0, &dataOut);
	if (!result) {
		return "";
	}
	std::string res = std::string(reinterpret_cast<char *>(dataOut.pbData), dataOut.cbData);
	LocalFree(dataOut.pbData);
	return res;
}

vector<BYTE> EncryptData(string data) {
	DATA_BLOB DataIn, dataOut;

	DataIn.pbData = reinterpret_cast<BYTE*>(const_cast<char*>(data.data()));
	DataIn.cbData = data.size() * sizeof(char);

	BOOL result = CryptProtectData(&DataIn, NULL, NULL, NULL, NULL, 0, &dataOut);
	if (!result) {
		return vector<BYTE>();
	}
	std::vector<BYTE> res(dataOut.pbData, dataOut.pbData + dataOut.cbData);
	LocalFree(dataOut.pbData);
	return res;
}

string DecryptData(string data) {
	DATA_BLOB DataIn, dataOut;
	DataIn.pbData = (BYTE*)data.c_str();
	DataIn.cbData = data.size() * sizeof(wchar_t);
	BOOL result = CryptUnprotectData(&DataIn, NULL, NULL, NULL, NULL, 0, &dataOut);
	if (!result) {
		return "";
	}
	return string((char*)dataOut.pbData, sizeof(dataOut));
}