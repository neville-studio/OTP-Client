#include "otp.h"

OTP::OTP() {
	this->type = TOTP;
	this->algorithm = SHA_1;
}

OTP::OTP(OTP_Type type) {
	this->type = type;
}

OTP::~OTP() {
}
string OTP::generateOTP(string secret, int secret_type,int digits, int addition_param) {
	if (this->algorithm == UNKNOWN)
	{
		return "The OTP is unknown.";
	}
	HMAC hmac(this->algorithm);
	uint64_t counter;
	size_t key_len = secret.size();
	vector<uint32_t> converted_key;
	switch (secret_type)
	{
	case 1:
		key_len *= 5;
		converted_key = base32Decode2uint32_tVector(secret);
		break;
	case 2:
		key_len *= 6;
		converted_key = base64Decode2uint32_tVector(secret);
		break;
	case 3:
		key_len *= 4;
		converted_key = hexStringDecode2uint32_tVector(secret);
		break;
	}
	switch (this->type)
	{
	case TOTP:
		counter = time(NULL) / addition_param;
		break;
	case HOTP:
		counter = addition_param;
		break;
	default:
		counter = 0;
		break;
	}
	vector<uint32_t> message = {(uint32_t)(counter >> 32 ), (uint32_t)counter};
	vector<uint32_t> result = hmac.getHashMessage(message, converted_key, 8);

	uint32_t offset = result[result.size() - 1] & 0x0f;
	//  11 2,3 3,0 3,1 3,2
	uint32_t bin_code = (((result[offset / 4] & ((1ULL << ((4 - offset % 4) * 8)) -1 ))) << ((offset % 4) * 8)) | (result[offset / 4 + 1] >> ((4 -(offset % 4)) * 8));
	bin_code = bin_code & 0x7fffffff;


	/*vector<byte> byte_result;
	for (uint32_t singleNum : result)
	{
		byte_result.push_back((byte)(singleNum >> 24) );
		byte_result.push_back((byte)(singleNum >> 16) );
		byte_result.push_back((byte)(singleNum >> 8) );
		byte_result.push_back((byte)singleNum );
	}

	uint32_t bin_code = uint32_t(byte_result[offset] & (byte)0x7f) << 24 |
		uint32_t(byte_result[offset + 1] & (byte)0xff) << 16 |
		uint32_t(byte_result[offset + 2] & (byte)0xff) << 8 |
		uint32_t(byte_result[offset + 3] & (byte)0xff);*/
	string otp = to_string(bin_code % (uint64_t)pow(10, digits));
	return otp;

}

void OTP::setAlgorithm(AlgorithmName algorithm)
{
	this->algorithm = algorithm;
}