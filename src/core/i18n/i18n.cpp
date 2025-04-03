#include "i18n.h"


std::wstring i18nClient::get(std::string key) {
	switch (lang) {
	case ZH_CN:
		return zh_cn[key];
	case EN_US:
		return en_us[key];
	default:
		return en_us[key];
	}

	
}

i18nClient* i18nClient::instance = nullptr;

i18nClient::i18nClient() {
	en_us = {
		{"add", L"Add"},
		{"edit" , L"Edit"},
		{"delete" , L"Delete"},
		{"import" , L"Import"},
		{"export" , L"Export"},
		{"networkTime" , L"NetTime Manage"},
		{"localTime" , L"Use LocalTime"},
		{"name" , L"Name"},
		{"windowTitle" , L"One-time Password Viewer Client"},
		{"currentVersion" , L"Current version"},
		{"oncePassword" , L"Current Password"},
		{"remainTime" , L"remain Times"},
		{"remainTimeTip" , L"%llds"},
		{"copyright", L"Copyright © 2025 Neville Studio, all rights reserved."},
		{"xpEol", L"To get support, please update your Operating System."}
	};
	zh_cn = {
		{"add", L"添加"},
		{"edit" , L"编辑"},
		{"delete" , L"删除"},
		{"import" , L"导入"},
		{"export" , L"导出"},
		{"networkTime" , L"管理时钟服务"},
		{"localTime" , L"使用本地时间"},
		{"name" , L"名称"},
		{"windowTitle" , L"单次密码验证查看客户端"},
		{"currentVersion" , L"当前版本"},
		{"oncePassword" , L"当前密码"},
		{"remainTime" , L"过期时间"},
		{"remainTimeTip" , L"%lld秒"},
		{"copyright", L"Copyright © 2025 Neville Studio, all rights reserved."},
	    {"xpEol", L"要继续获得支持，您的计算机操作系统需要更新"}
	};
}