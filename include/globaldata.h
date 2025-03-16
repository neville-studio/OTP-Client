// Last Modified: 2021/04/21

#include "nlohmann/json.hpp"
#include "common.h"


struct OTPInfo
{
	int type;			// 0: TOTP, 1: HOTP
	string algorithm;   // SHA1
	int digits;			// 6
	int addition_param; // 30
	string friendly_name;
	string secret;
};


class GlobalConfiguration
{
public:
	~GlobalConfiguration()=default;
	static GlobalConfiguration* getInstance() {
		if (config == nullptr) {
			static GlobalConfiguration instance;
			config = &instance;
		}
	    return config;
	};
	void setConfig(string config);
	string getConfig();
	vector<string> getSNTPServers() { return sntp_servers; };
	vector<OTPInfo> getOTPConfig() { return otp_config; };
private:
	static GlobalConfiguration *config;
	

	vector<string> sntp_servers;
	vector<OTPInfo> otp_config;
	GlobalConfiguration& operator=(const GlobalConfiguration&) = delete;
	GlobalConfiguration(const GlobalConfiguration&) = delete;

	GlobalConfiguration() = default;;
};