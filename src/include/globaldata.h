// Last Modified: 2021/04/21

#include "nlohmann/json.hpp"
#include "common.h"
using namespace std;

struct OTPInfo
{
	int type = 0;			  // 0: TOTP, 1: HOTP
	int algorithm = 1;        // SHA1
	int digits = 6;			  // 6
	int addition_param = 30;  // 30
	string friendly_name;
	string secret;
	int secret_type = 1; // 1: base32, 2: base64, 3: hex
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
	void setConfig(wstring config);
	void setUseNetworkTime(bool use_network_time) { this->use_network_time = use_network_time; };
	void setSNTP_servers(vector<string> servers) { sntp_servers = servers; };
	void setOTPConfig(vector<OTPInfo> otp_config) { this->otp_config = otp_config; };
	
	string getConfig();
	vector<string> getSNTPServers() { return sntp_servers; };
	vector<OTPInfo> getOTPConfig() { return otp_config; };
	bool getUseNetworkTime() { return use_network_time; };
private:
	static GlobalConfiguration *config;
	
	bool use_network_time = false;

	vector<string> sntp_servers;
	vector<OTPInfo> otp_config;
	GlobalConfiguration& operator=(const GlobalConfiguration&) = delete;
	GlobalConfiguration(const GlobalConfiguration&) = delete;

	GlobalConfiguration() = default;;
};