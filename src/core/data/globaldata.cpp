#include "globaldata.h"

void GlobalConfiguration::setConfig(string config)
{
	nlohmann::json j = nlohmann::json::parse(config);
	this->sntp_servers = j["time_servers"].get<vector<string>>();
	this->otp_config.clear();

	for (auto& item : j["otps"])
	{
		OTPInfo otp_info = {};
		otp_info.type = (OTP_Type)item["type"];
		otp_info.algorithm = (HashAlgorithm)item["algorithm"];
		otp_info.digits = item["digits"];
		otp_info.addition_param = item["addition_param"];
		this->otp_config.push_back(otp_info);
	}
}

string GlobalConfiguration::getConfig()
{
	nlohmann::json j;
	j["time_servers"] = this->sntp_servers;
	j["otps"] = nlohmann::json::array(otp_config);
	return "";
}