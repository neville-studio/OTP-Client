// Last Modified: 2021/04/21

#include "nlohmann/json.hpp"
#include "common.h"

struct OTPInfo
{
	OTP_Type type;
	HashAlgorithm algorithm;
	int digits;
	int addition_param;
};


class GlobalConfiguration
{
public:
	~GlobalConfiguration()=default;
	static GlobalConfiguration* getInstance() { return config; };
	void setConfig(string config);
	string getConfig();
	vector<string> getSNTPServers() {return sntp_servers};
	OTPConfig getOTPConfig() { return otp_config };
private:
	GlobalConfiguration config;
	GlobalConfiguration();

	vector<string> sntp_servers;
	vector<OTPInfo> otp_config;
};