// Description: i18n class for internationalization.
// Date: 2025/03/20

#include <string>
#include <fstream>
#include <filesystem>
#pragma once
#include <iostream>
#include <map>
#include "nlohmann/json.hpp"

enum i18nLangSupported {
	ZH_CN,
	EN_US
};

class i18nClient {
public:
	static i18nClient *getInstence() {  
		if (instance == nullptr) {
			static i18nClient local_instance; // 局部静态实例，在首次调用时初始化
			instance = &local_instance;
		}
		return instance; };
	std::wstring get(std::string key);
	void i18nClient::setLang(i18nLangSupported lang) { this->lang = lang; };
private:
	static i18nClient* instance;
	std::map<std::string, std::wstring> zh_cn;
	std::map<std::string, std::wstring> en_us;
	
	i18nLangSupported lang;
	i18nClient(const i18nClient&) = delete;
	i18nClient& operator=(const i18nClient&) = delete;
	i18nClient();
};