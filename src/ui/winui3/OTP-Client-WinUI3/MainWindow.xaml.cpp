#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

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
    //SNTPDialog().ShowAsync();
}
//void winrt::OTP_Client_WinUI3::implementation::MainWindow::Button_Click_1(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
//{
//
//}
