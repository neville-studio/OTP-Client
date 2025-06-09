#include "hexString.h"

char hexString[] = "0123456789ABCDEF";
vector<byte> hexStringDecode(string in)
{
	vector<byte> result;
	int flag = 0;
	byte part = (byte)0;
	for (char i : in)
	{
		i = toupper(i);
		if (i > 'A' && i < 'F')
			part = (part << 4) | (byte)(i - 'A' + 10);
		else if (isdigit(i))
			part = (part << 4) | (byte)(i - '0');
		else
			throw "invalid Hex String.";
		flag++;
		if (flag >= 2)
		{
			flag = 0;
			result.push_back(part);
			part = (byte)0x00;
		}
	}
	return result;
}

string hexStringEncode(vector<byte>& in)
{
	string res;
	for (byte i : in)
	{
		res.push_back(hexString[(int)(i >> 4) & 0xF]);
		res.push_back(hexString[(int)i & 0xF]);
	}
	return res;
}

string convert_hex_by_uint_32(vector<uint32_t>& in)
{
	vector<byte> s;
	for (uint32_t part : in)
	{
		s.push_back(byte(part >> 24));
		s.push_back(byte(part >> 16));
		s.push_back(byte(part >> 8));
		s.push_back(byte(part));
	}
	return hexStringEncode(s);
}

vector<uint32_t> hexStringDecode2uint32_tVector(string in)
{
	vector<uint32_t> result;
	int flag = 0;
	uint32_t part = 0;
	for (char i : in)
	{
		i = toupper(i);
		if (i >= 'A' && i <= 'F')
			part = (part << 4) | (uint32_t)(i - 'A' + 10);
		else if (isdigit(i))
			part = (part << 4) | (uint32_t)(i - '0');
		else if (i == ' ')
			continue;
		else
			throw "invalid Hex String.";
		flag++;
		if (flag >= 8)
		{
			flag = 0;
			result.push_back(part);
			part = 0x00;
		}
	}
	if (flag > 0)
	{
		result.push_back(part << ((8-flag) * 4));
	}
	return result;
}

bool isHexEncode(string s, bool ignoreSpace = true) {
	for (auto i : s)
	{
		if (!(isdigit(i) || (i >= 'A' && i <= 'F')))
			return false;
	}
	return true;
}
bool isHexEncode(wstring s, bool ignoreSpace = true) {
	for (auto i : s)
	{
		if (!(isdigit(i) || (i >= 'A' && i <= 'F')))
			return false;
	}
	return true;
};