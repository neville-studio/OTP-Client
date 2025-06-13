#pragma once

#include "MainWindow.g.h"
#include "OTPItem.h"
#include "globaldata.h"
#include "hmac.h"
#include "otp.h"
#include <string.h>


void calculateThread();
void SNTPClientUpdateThreadProc();
void ReadDataFromFile();

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
            initData();
            ExtendsContentIntoTitleBar(true);
            SetTitleBar(TitleBarMainWindow());
           
			winrt::Microsoft::UI::Xaml::DispatcherTimer timer;
            
            // 设置时间间隔为一秒
            timer.Interval(std::chrono::milliseconds(100));

            // 注册 Tick 事件处理程序
            timer.Tick([this](const IInspectable&, const IInspectable&)
            {
                    // 更新 TextBlock 文本
                    UIUpdate();
            });

            // 开始定时器
            timer.Start();

            // Build a new Thread
            std::thread thread1(calculateThread);
            thread1.detach();
            std::thread thread2(SNTPClientUpdateThreadProc);
            thread2.detach();
            //TrySet
   //         winrt::OTP_Client_WinUI3::OTPItem otpItem = {}; // Create an instance of OTPItem
   //         otpItem.FriendlyName(L"Example OTP Item"); // Example friendly name
			//otpItem.RemainingTimeText(L"30"); // Example remaining time text
   //         otpItem.Progress(100.0); // Example remaining time text
			//otpItem.SecretDigits(L"123456"); // Example secret
   //         otpItem.ProgressColor(L"");

   //         OTPItems().Append(otpItem);


            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }
        void updateFormatter();
        void UIUpdate();

        
        void SNTPThread();
        //void addProgressAnimation(UIElement const& element, float progress);

        //void winrt::OTP_Client_WinUI3::implementation::MainWindow::OnContainerContentChanging(IInspectable const&, ContainerContentChangingEventArgs const& args);

        int32_t MyProperty();
        void MyProperty(int32_t value);
        void initData();
        void AddOTPClickHandler(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void AdvancedOptionsButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void IsHotpBox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void IsHotpBox_Unchecked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void CancelButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void SNTPDialogCancelButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void ManageClockButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OTPDialogOK_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    private:
        winrt::Windows::Foundation::Collections::IObservableVector<winrt::OTP_Client_WinUI3::OTPItem> m_otpItem{
            winrt::single_threaded_observable_vector<winrt::OTP_Client_WinUI3::OTPItem>() };
    //public:
        
        
    public:
        void OtpListBox_DoubleTapped(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::DoubleTappedRoutedEventArgs const& e);
        void HOTPShowSecret_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void ShowHOTPDialog_PrimaryButtonClick(winrt::Microsoft::UI::Xaml::Controls::ContentDialog const& sender, winrt::Microsoft::UI::Xaml::Controls::ContentDialogButtonClickEventArgs const& args);

        long long current_edit_index = -1;
        void ItemEditButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void ItemDeleteButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void ApplyOrSyncClockServerButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void ApplyClockServerButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void Window_Closed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::WindowEventArgs const& args);
    };
    
}

namespace winrt::OTP_Client_WinUI3::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
