#pragma once

#include "MainWindow.g.h"
#include "OTPItem.h"
#include "globaldata.h"
//namespace OTPListData {
//	class OTPItemViewModel;
//    {
//    public:
//        winrt::hstring FriendlyName;
//        winrt::hstring Secret;
//
//        winrt::hstring RemainingTimeText;
//        winrt::hstring currentCode;
//        float Progress;
//
//        int OTPType;
//        int OTPAlgorithm;
//        int OTPDigits;
//        int period;
//        int counter;
//        OTPItem() = default;
//    };
//};



namespace winrt::OTP_Client_WinUI3::implementation
{
    
    struct MainWindow : MainWindowT<MainWindow>
    {
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::OTP_Client_WinUI3::OTPItem> OTPItems()
        {
            return m_otpItem;
        }
        MainWindow()
        {
            ExtendsContentIntoTitleBar(true);
            SetTitleBar(TitleBarMainWindow());
           
            winrt::OTP_Client_WinUI3::OTPItem otpItem = {}; // Create an instance of OTPItem
            otpItem.FriendlyName(L"Example OTP Item"); // Example friendly name
			otpItem.RemainingTimeText(L"30"); // Example remaining time text
            otpItem.Progress(100.0); // Example remaining time text
			otpItem.SecretDigits(L"123456"); // Example secret
            otpItem.ProgressColor(L"");

            OTPItems().Append(otpItem);


            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);
        void AddOTPClickHandler(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void AdvancedOptionsButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void IsHotpBox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void IsHotpBox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void Button_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void CancelButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void SNTPDialogCancelButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void ManageClockButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OTPDialogOK_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    private:
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::OTP_Client_WinUI3::OTPItem> m_otpItem{
            winrt::single_threaded_observable_vector<winrt::OTP_Client_WinUI3::OTPItem>() };
    public:
        
        GlobalConfiguration* globalConfig = GlobalConfiguration::getInstance();
        vector<OTPInfo> otpInfos = globalConfig->getOTPConfig();
        vector<string> sntpServers = globalConfig->getSNTPServers();
    };
    
}

namespace winrt::OTP_Client_WinUI3::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
