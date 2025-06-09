#include "pch.h"
#include "MainWindow.xaml.h"
#include "sntp.h"
#include <cstdio>
#include <iostream>
#include <thread>
#include <mutex>
#include "DataHandler.h"
#include <winrt/Windows.Globalization.NumberFormatting.h>
//#include <winrt/Windows.UI.Composition.h>
//#include <winrt/Windows.UI.Xaml.Hosting.h>

#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

SNTPClient sntpClient;
map<string, size_t> timesCache;
map<wstring, vector<BYTE>> dataCache;
mutex dataCacheMutex;
GlobalConfiguration* globalConfig = GlobalConfiguration::getInstance();
vector<OTPInfo> otpInfos = globalConfig->getOTPConfig();
vector<string> sntpServers = globalConfig->getSNTPServers();
map<string, wstring> digitsCache;

wstring padZero(wstring s, int count)
{
	if (s.size() > count)return s;
	return wstring(L"00000000").substr(0, count - s.size()) + s;
}

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.
int64_t getCurrentMillSecond(bool usingNetTime = false) {
	if (!usingNetTime) {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
	else {
		int64_t resultTimeStamp = sntpClient.getResultTimestamp() + std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - sntpClient.getLastUpdate();
		if (resultTimeStamp < 0)
		{
			OutputDebugString(L"获取时钟失败");
		}
		return resultTimeStamp >= 0 ? resultTimeStamp : std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
	return 0;
}
namespace winrt::OTP_Client_WinUI3::implementation
{
    int32_t MainWindow::MyProperty()
    {

        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
}

void winrt::OTP_Client_WinUI3::implementation::MainWindow::AddOTPClickHandler(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    OTPDialog().ShowAsync();
	FriendlyNameBox().Text(L"");
	SecretBox().Text(L"");
	HotpCounterBox().Value(0);
	TotpStepBox().Value(30);
	SecretLengthBox().Value(6);
	IsHotpBox().IsChecked(false);
	EncodingBox().SelectedIndex(0);
	AlgorithmBox().Items().Clear();
	
	vector<wstring> AvailableAlgorithms = HashAlgorithmInterfase::getAvailableHashAlgorithms();
	for (wstring s : AvailableAlgorithms)
	{
		
		AlgorithmBox().Items().Append(box_value(s));
	}
	AlgorithmBox().SelectedIndex(0);

	updateFormatter();
	


}

void winrt::OTP_Client_WinUI3::implementation::MainWindow::updateFormatter()
{
	auto decimalFormatter = winrt::Windows::Globalization::NumberFormatting::DecimalFormatter();
	auto incrementNumberRounder = winrt::Windows::Globalization::NumberFormatting::IncrementNumberRounder();
	incrementNumberRounder.Increment(1.0);
	incrementNumberRounder.RoundingAlgorithm(winrt::Windows::Globalization::NumberFormatting::RoundingAlgorithm::RoundHalfUp);
	decimalFormatter.IntegerDigits(1);
	decimalFormatter.FractionDigits(0);
	decimalFormatter.NumberRounder(incrementNumberRounder);
	HotpCounterBox().NumberFormatter(decimalFormatter);
	TotpStepBox().NumberFormatter(decimalFormatter);
}

void winrt::OTP_Client_WinUI3::implementation::MainWindow::AdvancedOptionsButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
	AdvancedOptionsPanel().Visibility(AdvancedOptionsPanel().Visibility() == winrt::Microsoft::UI::Xaml::Visibility::Visible ? winrt::Microsoft::UI::Xaml::Visibility::Collapsed : winrt::Microsoft::UI::Xaml::Visibility::Visible);
	AdvancedOptionsButton().Content(box_value(AdvancedOptionsPanel().Visibility() == winrt::Microsoft::UI::Xaml::Visibility::Visible ? L"高级选项 <<" : L"高级选项 >>"));
	
   /* AdvancedOptionsButton().Content(
        (winrt::Windows::Foundation::IInspectable) "你好，世界"
    );*/
}

void winrt::OTP_Client_WinUI3::implementation::MainWindow::IsHotpBox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
	Interval().Visibility(winrt::Microsoft::UI::Xaml::Visibility::Collapsed);
	Counter().Visibility(winrt::Microsoft::UI::Xaml::Visibility::Visible);
}

void winrt::OTP_Client_WinUI3::implementation::MainWindow::IsHotpBox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    Interval().Visibility(winrt::Microsoft::UI::Xaml::Visibility::Visible);
    Counter().Visibility(winrt::Microsoft::UI::Xaml::Visibility::Collapsed);
}

void winrt::OTP_Client_WinUI3::implementation::MainWindow::Button_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{

}

void winrt::OTP_Client_WinUI3::implementation::MainWindow::CancelButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    OTPDialog().Hide();
}

void winrt::OTP_Client_WinUI3::implementation::MainWindow::SNTPDialogCancelButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    SNTPDialog().Hide();
}

void winrt::OTP_Client_WinUI3::implementation::MainWindow::ManageClockButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    SNTPDialog().ShowAsync();
}

void winrt::OTP_Client_WinUI3::implementation::MainWindow::OTPDialogOK_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    wstring friendlyname = FriendlyNameBox().Text().c_str();
    wstring secret = SecretBox().Text().c_str();
    bool isHotp = IsHotpBox().IsChecked().GetBoolean();

	int32_t digits = SecretLengthBox().Value();
	int32_t interval = TotpStepBox().Value();
	if (interval <= 0)interval = 30;
	int32_t counter = HotpCounterBox().Value();

	int32_t encode = EncodingBox().SelectedIndex() + 1;
	//if (encode == 0) encode = 1;
    int32_t algorithm = AlgorithmBox().SelectedIndex() + 1;

    OTPInfo otpInfo;
	otpInfo.algorithm = algorithm;
	otpInfo.secret_type = encode;
	otpInfo.friendly_name = to_string(friendlyname);
	otpInfo.secret = to_string(secret);
	otpInfo.type = isHotp;
	otpInfo.digits = digits;
	otpInfo.addition_param = !isHotp? interval: digits;
	//guid guid1;
	//GUID().

	GUID guid;
	HRESULT r1 = CoCreateGuid(&guid);
	TCHAR r[48];
	r1 = StringFromGUID2(guid, r, 48);
	wstring guidStr(r);

	dataCache[guidStr] = EncryptData(otpInfo.secret);
	otpInfo.secret = to_string(guidStr); // Store the GUID instead of the actual secret
	/*GUID tpGuid = winrt::guid();
	wstring guidStr = to_wstring(tpGuid.Data4);*/

	otpInfos.push_back(otpInfo);

	OTP otp(HOTP);
	otp.setAlgorithm(otpInfo.algorithm);
	

    winrt::OTP_Client_WinUI3::OTPItem otpItem;
	otpItem.FriendlyName(friendlyname);
	if (otpInfo.type == 0) {
		timesCache[otpInfo.secret] = getCurrentMillSecond() / otpInfo.addition_param;
		string otpRes = otp.generateOTP(DecryptData(dataCache[wstring(otpInfo.secret.begin(), otpInfo.secret.end())]), otpInfo.secret_type, otpInfo.digits, getCurrentMillSecond() / otpInfo.addition_param / 1000);
		wstring otpRes_w(otpRes.begin(), otpRes.end());
		otpItem.SecretDigits(padZero(otpRes_w, otpInfo.digits));
		 
		float remain = (interval * 1000 - getCurrentMillSecond() % (interval * 1000)) / 1000.0;
		wchar_t remainText[51];
		swprintf_s(remainText, L"%.1fs", remain);
		wstring remainingTime = remainText;
		otpItem.RemainingTimeText(remainingTime);
		otpItem.Progress(remain / interval * 100);
	}
	else
	{
		otpItem.SecretDigits(L"计次密钥，请双击查看");
	}
	//otpItem.ProgressColor(L"#FF0000");
	OTPItems().Append(otpItem);
	


	//otpItem.IsHotp(isHotp);

	OTPDialog().Hide();

    //SNTPDialog().ShowAsync();
}




void winrt::OTP_Client_WinUI3::implementation::MainWindow::UIUpdate()
{
	int count = 0;
	for(int i = 0; i < OTPItems().Size(); i++)
	{
		//if (OTPItems().GetAt(i).) continue;
		if (otpInfos[i].type == 1)continue;
		
		int interval = otpInfos[i].addition_param;
		float remain = (interval * 1000 - getCurrentMillSecond() % (interval * 1000)) / 1000.0;
		wchar_t remainText[51];
		swprintf_s(remainText, L"%.1fs", remain);
		wstring remainingTime = remainText;
		winrt::OTP_Client_WinUI3::OTPItem otpItem = OTPItems().GetAt(i);
		if (digitsCache[otpInfos[i].secret] != otpItem.SecretDigits())
		{
			otpItem.SecretDigits(digitsCache[otpInfos[i].secret]);
		}
		otpItem.RemainingTimeText(remainingTime);
		otpItem.Progress(remain / interval * 100);
	}
}

void calculateThread() {
	while (1)
	{
		for (OTPInfo otpInfo: otpInfos)
		{
			if (otpInfo.type == 1)continue;
			unsigned long long usingtime = getCurrentMillSecond() / otpInfo.addition_param / 1000;
			if (usingtime == timesCache[otpInfo.secret])continue;
			OTP otp(HOTP);
			timesCache[otpInfo.secret] = usingtime;
			string otpRes = otp.generateOTP(DecryptData(dataCache[wstring(otpInfo.secret.begin(), otpInfo.secret.end())]), otpInfo.secret_type, otpInfo.digits, usingtime);
			digitsCache[otpInfo.secret] = padZero(wstring(otpRes.begin(), otpRes.end()), otpInfo.digits);
			//wstring otpRes_w(otpRes.begin(), otpRes.end());
		}
		std::this_thread::sleep_for(std::chrono::microseconds(100));
		/*std::thread:(1000);*/
	}
}
void winrt::OTP_Client_WinUI3::implementation::MainWindow::SNTPThread() {}

