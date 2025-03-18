#include <iostream>
#include <string>
#include <vector>


#include <Windows.h>

//std::wstring DecryptData(std::wstring data);
std::vector<BYTE> EncryptData(std::string data);

std::string DecryptData(std::vector<BYTE> data);
std::vector<BYTE> EncryptData(std::wstring data);

std::string encodeBase64FromBYTE(std::vector<BYTE> in);
std::vector<BYTE> decodeBase64ToBYTE(std::string in);

bool WriteBytesToFile(const std::string& filePath, const std::vector<BYTE>& byteData);
std::vector<BYTE> ReadDataFromFile(const std::string& filePath);