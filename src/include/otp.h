#pragma once

#include "common.h"
#include "hmac.h"
#include "encode.h"

enum OTP_Type {
	TOTP,
	HOTP
};

class OTP {
public:
	OTP();
	OTP(OTP_Type type);
	~OTP();

	/*
	* @brief this function is to generate One-Time Password
	* @param secret the secret key
	* @param secret_type the secret type, 0 for unknown, 1 for base32, 2 for base64, 3 for hex
	* @param digits the length of the OTP, default is 6
	* @param addition_param the addition param, for TOTP, it is the time step, for HOTP, it is the counter.
	* @return the OTP string password
	**/
	string generateOTP(string secret,int secret_type = 1, int digits = 6, int64_t addition_param = 30);

	/*
	* @brief this function is to set the algorithm of the OTP
	* @param algorithm the algorithm name
	*/
	void setAlgorithm(int algorithm);
private :
	OTP_Type type;
	AlgorithmName algorithm = SHA_1;
	int encode_type;
};