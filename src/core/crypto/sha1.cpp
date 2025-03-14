/** 
* @file sha1.cpp
* @brief SHA-1 implementation
* @ingroup core-crypto
* @author Neville Studio <2840772674@qq.com>
*/
#include "sha1.h"

inline static uint32_t rotateLeft(uint32_t x, int n) {
	return  (x << n) | (x >> (32 - n));
}
inline static uint32_t rotateRight(uint32_t x, int n) {
	return x = (x >> n) | (x << (32 - n));
}

SHA1::SHA1() {

}

void SHA1::messageByBase32(string message) {
	unsigned long long data = 0;
	int count = 0;
	// Convert the message to upper case
	for (char str : message) {
		str = toupper(str);
		int base = 0;
		if (str >= 'A' && str <= 'Z') {
			base = str - 'A';
		}
		else if (str >= '2' && str <= '7') {
			base = str - '2' + 26;
		}
		else {
			base = -1;
			throw "Invalid Base32";
		}
		data = data * 32 + base;
		count += 5;
		if (count >= 32) {
			this->message.push_back(data >> (count - 32));
			count -= 32;
			data &= ((long long)1 << count) - 1;
		}
	}
	this->size = this->message.size() * 32 + count;
	this->message.push_back((data << (32 - count)) | (1ULL << (31-count)));
	
}


void SHA1::messageByBase64(string message) {
	unsigned long long data = 0;
	int count = 0;
	// Convert the message to upper case
	for (char str : message) {
		int base = 0;
		if (str >= 'A' && str <= 'Z') {
			base = str - 'A';
		}
		else if (str >= 'a' && str <= 'z') {
			base = str - 'a' + 26;
		}
		else if (str >= '0' && str <= '9') {
			base = str - '0' + 52;
		}
		else if (str == '+') {
			base = 62;
		}
		else if (str == '/') {
			base = 63;
		}
		else {
			base = -1;
			throw "Invalid Base64";
		}
		data = data * 64 + base;
		count += 6;
		if (count >= 32) {
			this->message.push_back(data >> (count - 32));
			count -= 32;
			data &= ((long long)1 << count) - 1;
		}
	}
	this->size = this->message.size() * 32 + count;
	this->message.push_back(data << (32 - count) | (1ULL << (31-count)));
}

void SHA1::messageByHexString(string message) {
	unsigned long long data = 0;
	int count = 0;
	// Convert the message to upper case
	for (char str : message) {
		str = toupper(str);
		int base = 0;
		if (str >= 'A' && str <= 'F') {
			base = str - 'A' + 10;
		}
		else if (str >= '0' && str <= '9') {
			base = str - '0';
		}
		else if(str == ' ')
		{
			continue;
		}
		else {
			base = -1;
			throw "Invalid Hex String";
		}
		data = data * 16 + base;
		count += 4;
		
		if (count >= 32) {
			this->message.push_back(data >> (count - 32));
			count -= 32;
			data &= ((long long)1 << count) - 1;
		}
	}
	this->size = this->message.size() * 32 + count;
	this->message.push_back((data << (32 - count)) | (1ULL << (31 - count)));
}

void SHA1::fillMessage() {
	// Append k bits '0', where k is the minimum number >= 0 such that the resulting message
	// length (in bits) is congruent to 448 (mod 512)
	while ((message.size() + 2) % 16 != 0) {
		message.push_back(0);
	}
	// Append the length of the message (before pre-processing), in bits, as a 64-bit big-endian integer
	message.push_back(this->size >> 32);
	message.push_back(this->size & 0xffffffff);
}

void SHA1::processMessage() {
	H[0] = 0x67452301;
	H[1] = 0xEFCDAB89;
	H[2] = 0x98BADCFE;
	H[3] = 0x10325476;
	H[4] = 0xC3D2E1F0;
	vector <uint32_t> temp(16);
	int counter = 0;
	for (uint64_t i = 0; i < message.size(); i++)
	{
		temp[counter++] = message[i];
		if (counter == 16)
		{
			processChunk(temp);
			counter = 0;
		}
	}
	
}
inline uint32_t f1(uint32_t b, uint32_t c, uint32_t d)
{
	return (b & c) | (~b & d);
}

inline uint32_t f2(uint32_t b, uint32_t c, uint32_t d)
{
	return b ^ c ^ d;
}

inline uint32_t f3(uint32_t b, uint32_t c, uint32_t d)
{
	return (b&c)|(b&d)|(c&d);
}

void SHA1::processChunk(vector<uint32_t> &in) {
	if (in.size() != 16) { throw "Error! chunk not enough"; };
	vector<uint32_t> result;
	uint32_t a = H[0], b = H[1], c = H[2], d = H[3], e = H[4];
	uint32_t w_block[80] = {};
	for (int i = 0; i < 80; i++)
	{
		if (i < 16)w_block[i] = w_block[i] = in[i];
		else
		{
			w_block[i] = rotateLeft(w_block[i - 3] ^ w_block[i - 8] ^ w_block[i - 14] ^ w_block[i - 16], 1);
		}
	}
	uint32_t kt[4] = { 0x5A827999 , 0x6ED9EBA1 , 0x8F1BBCDC , 0xCA62C1D6 };
	for (int i = 0; i < 80; i++)
	{
		uint32_t t = 0;
		if (i < 20)
			t = rotateLeft(a, 5) + f1(b, c, d) + e + kt[0] + w_block[i];
		else if(i < 40)
			t = rotateLeft(a, 5) + f2(b, c, d) + e + kt[1] + w_block[i];
		else if(i < 60)
			t = rotateLeft(a, 5) + f3(b, c, d) + e + kt[2] + w_block[i];
		else 
			t = rotateLeft(a, 5) + f2(b, c, d) + e + kt[3] + w_block[i];
		e = d;
		d = c;
		c = rotateLeft(b, 30);
		b = a;
		a = t;
	}
	H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e;
}



string SHA1::getHashMessage(string message,int message_type,int output_type)
{
	
	if (message_type == 1)
		messageByBase32(message);
	else if (message_type == 2)
		messageByBase64(message);
	else if (message_type == 3)
		messageByHexString(message);
	else
		throw "Invalid Type";
	
	fillMessage();
	processMessage();
	vector<uint32_t> convert = { H[0],H[1],H[2],H[3],H[4] };
	if (output_type == 1)
		return convert_base32_from_uint32_t(convert);
	else if (output_type == 2)
		return convert_base64_from_uint32_t(convert);
	else if (output_type == 3)
		return convert_hex_by_uint_32(convert);
	return "";

}

vector<uint32_t> SHA1::getHashMessageByuint32(vector<uint32_t> message, size_t message_size)
{
	if (message_size % 32 != 0)
	{
		message[message_size / 32] = message[message_size / 32] | (1 << (31 - (message_size % 32))) &~(1 << (31 - (message_size % 32))-1);
	}
	else {
		message.push_back(0x80000000);
	}
	this->message = message;
	this->size = message_size;
	fillMessage();
	processMessage();
	vector<uint32_t> convert = { H[0],H[1],H[2],H[3],H[4] };
	return convert;
}