#include "sha224.h"

SHA224::SHA224() {
}


vector<uint32_t> SHA224::convert_uint32_t_from_string(string message, int message_type) {
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

void SHA224::fillMessage() {
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
	
	message.resize(((message_size + 1) / 512+1) * 16 , 0);
	size_t last = message.size() - 1;
	message[last - 1] = message_size >> 32;
	message[last] = message_size & 0xffffffff;
	this->message = message;
}

//uint32_t rotateLeft(uint32_t value, int shift)
//{
//	return (value << shift) | (value >> (32 - shift));
//}
//uint32_t rotateRight(uint32_t value, int shift)
//{
//	return (value >> shift) | (value << (32 - shift));
//}


void SHA224::processMessage()
{
	vector<uint32_t> message = this->message;
	vector<uint32_t> hash = this->hash;
	vector<uint32_t> w(64);
	H[0] = 0xC1059ED8;
	H[1] = 0x367CD507;
	H[2] = 0x3070DD17;
	H[3] = 0xF70E5939;
	H[4] = 0xFFC00B31;
	H[5] = 0x68581511;
	H[6] = 0x64F98FA7;
	H[7] = 0xBEFA4FA4;
	size_t i = 0;
	for (i = 0; i < message.size(); i++)
	{
		w[i % 16] = message[i];
		if (i % 16 == 15)
		{
			processMessageChunk(w);
			fill(w.begin(), w.end(), 0);
		}
	}
	if(i % 16!=0)processMessageChunk(w);
}
uint32_t inline getWord(int i, uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4)
{
	uint32_t s0 = rotateRight(i1, 7) ^ rotateRight(i1, 18) ^ (i1 >> 3);
	uint32_t s1 = rotateRight(i2, 17) ^ rotateRight(i2, 19) ^ (i2 >> 10);
	return i3 + s0 + i4 + s1;
}


void SHA224::processMessageChunk(vector<uint32_t>& in)
{
	vector<uint32_t> w(64);
	// Extend the message
	for (int i = 0; i < in.size(); i++)
	{
		if (i < 16)w[i]=in[i];
		else {
			w[i] = getWord(i, w[i - 15], w[i - 2], w[i-16], w[i-7]);
		}
	}

	//Compress Message
	uint32_t const_k[64] = {
		0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
		0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,	0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
		0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC,	0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
		0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7,	0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
		0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,	0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
		0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3,	0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
		0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5,	0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
		0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,	0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
	};uint32_t a = H[0], b = H[1], c = H[2], d = H[3], e = H[4], f = H[5], g = H[6], h = H[7];
	for (int i = 0; i < 64; i++) {
		
		uint32_t s1 = rotateRight(e, 6) ^ rotateRight(e, 11) ^ rotateRight(e, 25);
		uint32_t ch = (e & f) ^ ((~e) & g);
		uint32_t temp1 = h + s1 + ch + const_k[i] + w[i];
		uint32_t s0 = rotateRight(a, 2) ^ rotateRight(a, 13) ^ rotateRight(a, 22);
		uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
		uint32_t temp2 = s0 + maj;
		h = g;
		g = f;
		f = e;
		e = d + temp1;
		d = c;
		c = b;
		b = a;
		a = temp1 + temp2;
		
	}H[0] += a; H[1] += b; H[2] += c; H[3] += d; H[4] += e; H[5] += f; H[6] += g; H[7] += h;
}



vector<uint32_t> SHA224::getHashMessageByuint32(vector<uint32_t> message, size_t message_size) {
	
	this->message = message;
	this->size = message_size;
	this->fillMessage();
	this->processMessage();
	this->hash = vector<uint32_t>(H, H + 7);
	//this->extendMessage();
	//vector<uint32_t> result = sha224.getHashMessageByuint32(sha224.message, message_size);
	
	return this->hash;
}


string SHA224::getHashMessage(string message, int message_type, int output_type) {
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

