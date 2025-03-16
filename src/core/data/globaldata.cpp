#include "globaldata.h"

void GlobalConfiguration::setConfig(string config)
{
	nlohmann::json j = nlohmann::json::parse(config);
	this->sntp_servers = j["time_servers"].get<vector<string>>();
	this->otp_config.clear();

	for (auto& item : j["otps"])
	{
		OTPInfo otp_info = {};
		otp_info.type = item["type"];
		otp_info.algorithm = item["algorithm"];
		otp_info.digits = item["digits"];
		otp_info.addition_param = item["addition_param"];
		otp_info.friendly_name = item["friendly_name"];
		otp_info.secret = item["secret"];
		this->otp_config.push_back(otp_info);
	}
}
GlobalConfiguration *GlobalConfiguration:: config = nullptr;
string GlobalConfiguration::getConfig()
{
	nlohmann::json j;
	j["time_servers"] = this->sntp_servers;
	for (OTPInfo otpinfo : otp_config)
	{
		nlohmann::json item;
		item["type"] = otpinfo.type;
		item["algorithm"] = otpinfo.algorithm;
		item["digits"] = otpinfo.digits;
		item["addition_param"] = otpinfo.addition_param;
		item["friendly_name"] = otpinfo.friendly_name;
		item["secret"] = otpinfo.secret;
		j["otps"].push_back(item);
	}
	return j.dump(-1);
}