#include "base64.h"

char base64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
vector<byte> base64Decode(string base64String)
{
	vector<byte> result;
	int sub_size = 0;
	int part = 0;
	for (char s : base64String)
	{
		if (isupper(s))
		{
			part = part << 6 | (s - 'A');
		}
		else if (islower(s))
		{
			part = part << 6 | (s - 'a' + 26);
		}
		else if (isdigit(s))
		{
			part = part << 6 | (s - '0' + 52);
		}
		else if (s == '+')
		{
			part = part << 6 | 62;
		}
		else if (s == '/')
		{
			part = part << 6 | 63;
		}
		else if (isspace(s))
		{
			continue;
		}
		else
		{
			throw "Invalid Base64";
		}
		sub_size += 6;
		if (sub_size >= 8)
		{
			sub_size -= 8;
			result.push_back((byte)((part >> sub_size) & 0xFF));
		}
	}

	return result;
}

string base64Encode(vector<byte>& input)
{
	string result;
	uint32_t part = 0;
	int have_bases = 0;
	for (byte partical : input)
	{
		part = part << 8 | (uint32_t)partical;
		have_bases += 8;
		while (have_bases>=6)
		{
			result.push_back(base64Chars[part >> (have_bases - 6)]);
			have_bases -= 6;
			part &= (1 << have_bases)-1;
		}
	}
	if (part)
	{
		part <<= 8;
		result.push_back(base64Chars[part >> 6]);
		result.push_back(base64Chars[part & 0x3F]);
	}
	return result;
}

string convert_base64_from_uint32_t(vector<uint32_t> data)
{
	string result;
	uint32_t part = 0;
	vector<byte> bytearray;
	for (uint32_t partical : data)
	{
		bytearray.push_back((byte)(partical >> 24));
		bytearray.push_back((byte)(partical >> 16));
		bytearray.push_back((byte)(partical >> 8));
		bytearray.push_back((byte)(partical));
	}
	return base64Encode(bytearray);
}

vector<uint32_t> base64Decode2uint32_tVector(string input)
{
	vector<byte> bytearray = base64Decode(input);
	vector<uint32_t> result;
	uint32_t part = 0;
	int have_bases = 0;
	for (byte partical : bytearray)
	{
		part = part << 8 | (uint32_t)partical;
		have_bases += 8;
		while (have_bases >= 32)
		{
			have_bases -= 32;
			result.push_back(part >> have_bases);
			part &= (1 << have_bases) - 1;
		}
	}
	if(have_bases>0)
		result.push_back(part << (31-have_bases));
	return result;
}

