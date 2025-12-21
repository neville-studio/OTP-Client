#include "sm3.h"

const uint32_t t[64] = {0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 
0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 0x79cc4519, 
0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 
0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a,
0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a,
0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a,
0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a,
0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a, 0x7a879d8a,
};

inline uint32_t FF(int j, uint32_t x, uint32_t y, uint32_t z)
{
	if (j < 16)
		return x ^ y ^ z;
	else
		return (x & y) | (x & z) | (y & z);	
}

inline uint32_t GG(int j, uint32_t x, uint32_t y, uint32_t z)
{
	if (j < 16)
		return x ^ y ^ z;
	else
		return (x & y) | (~x & z);
}

inline uint32_t P0(uint32_t x)
{
	return x ^ rotateLeft(x, 9) ^ rotateLeft(x, 17);
}

inline uint32_t P1(uint32_t x)
{
	return x ^ rotateLeft(x, 15) ^ rotateLeft(x, 23);
}

void SM3::fillMessage() {
	size_t message_size = this->size;
	vector<uint32_t> message = this->message;

	// Add 1 bit to the message
	
	if (message_size % 32 != 0)
	{
		message[message_size / 32] |= 1 << (31 - message_size % 32);
	}
	else
	{
		message.push_back(0x80000000);
	}

	message.resize(((message_size + 1) / 512 + 1) * 16, 0);
	size_t last = message.size() - 1;
	message[last - 1] = message_size >> 32;
	message[last] = message_size & 0xffffffff;
	this->message = message;
}
void SM3::processChunk(vector<uint32_t> &in)
{
	// Expand the message
	vector<uint32_t> w(68);
	vector<uint32_t> w1(64);
	for (int i = 0; i < 16; i++)
	{
		w[i] = in[i];
	}
	for (int i = 16; i < 68; i++)
	{
		w[i] = P1(w[i - 16] ^ w[i - 9] ^ rotateLeft(w[i - 3], 15)) ^ rotateLeft(w[i - 13], 7) ^ w[i - 6];
	}
	for (int i = 0; i < 64; i++)
	{
		w1[i] = w[i] ^ w[i + 4];
	}

	// Compress the message

	uint32_t a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7];
	for (int j = 0; j < 64; j++)
	{
		uint32_t ss1 = rotateLeft(rotateLeft(a, 12) + e + rotateLeft(t[j], j ), 7);
		uint32_t ss2 = ss1 ^ rotateLeft(a, 12);
		uint32_t tt1 = FF(j, a, b, c) + d + ss2 + w1[j];
		uint32_t tt2 = GG(j, e, f, g) + h + ss1 + w[j];
		d = c;
		c = rotateLeft(b, 9);
		b = a;
		a = tt1;
		h = g;
		g = rotateLeft(f, 19);
		f = e;
		e = P0(tt2);
	}
	H[0] ^= a; H[1] ^= b; H[2] ^= c; H[3] ^= d; H[4] ^= e; H[5] ^= f; H[6] ^= g; H[7] ^= h;

}
void SM3::processMessage() {
	H[0] = 0x7380166f;
	H[1] = 0x4914b2b9;
	H[2] = 0x172442d7;
	H[3] = 0xda8a0600;
	H[4] = 0xa96f30bc;
	H[5] = 0x163138aa;
	H[6] = 0xe38dee4d;
	H[7] = 0xb0fb0e4e;
	vector <uint32_t> temp(80);
	//int counter = 0;
	size_t i = 0;
	for (i = 0; i < message.size(); i++)
	{
		temp[i % 16] = message[i];
		if (i%16 == 15)
		{
			processChunk(temp);
			fill(temp.begin(), temp.end(), 0);
		}
	}
	if (i % 16 != 0)processChunk(temp);
}

vector<uint32_t> SM3::getHashMessageByuint32(vector<uint32_t> message, size_t message_size) {

	this->message = message;
	this->size = message_size;

	this->fillMessage();
	this->processMessage();
	this->hash = vector<uint32_t>(H, H + 8);
	//this->extendMessage();
	//vector<uint32_t> result = sha224.getHashMessageByuint32(sha224.message, message_size);

	return this->hash;
}
vector<uint32_t> SM3::convert_uint32_t_from_string(string message, int message_type) {
	vector<uint32_t> result;
	switch (message_type) {
	case 1:
		result = base32Decode2uint32_tVector(message);
		this->size = message.size() * 5;
		break;
	case 2:
		result = base64Decode2uint32_tVector(message);
		this->size = message.size() * 6;
		break;
	case 3:
		result = hexStringDecode2uint32_tVector(message);
		this->size = message.size() * 4;
		break;
	}
	return result;
}

string SM3::getHashMessage(string message, int message_type, int output_type) {
	this->input_type = message_type;
	this->message = convert_uint32_t_from_string(message, message_type);
	vector<uint32_t> result = getHashMessageByuint32(this->message, this->size);
	this->hash = result;
	string resultStr = "";
	if (output_type == 1) {
		resultStr = convert_base32_from_uint32_t(result);
	}
	else if (output_type == 2) {
		resultStr = convert_base64_from_uint32_t(result);
	}
	else if (output_type == 3) {
		resultStr = convert_hex_by_uint_32(result);
	}
	return resultStr;
}
