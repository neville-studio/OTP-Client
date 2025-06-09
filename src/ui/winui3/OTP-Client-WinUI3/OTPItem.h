#pragma once

#include "pch.h"
#include "OTPItem.g.h"

namespace winrt::OTP_Client_WinUI3::implementation
{
    struct OTPItem : OTPItemT<OTPItem>
    {
        OTPItem() = default;

        // 需要绑定的属性
        winrt::hstring FriendlyName();
        void FriendlyName(winrt::hstring const& value);

        winrt::hstring SecretDigits();
        void SecretDigits(winrt::hstring const& value);

        winrt::hstring RemainingTimeText();
        void RemainingTimeText(winrt::hstring const& value);

        winrt::Windows::UI::Color ProgressColor ();
        void ProgressColor(winrt::Windows::UI::Color const& value);

        void updateProgressColor();

        float Progress();
        void Progress(float const& value);

        bool IsHOTP();
        void IsHOTP(bool const& value);

        //winrt::hstring ProgressColor();
        //void ProgressColor(winrt::hstring const& value);

        winrt::event_token PropertyChanged(
            winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;


        

    private:
        winrt::hstring m_friendlyName;
        winrt::hstring m_secretDigits;
        winrt::hstring m_remainingTimeText;
        winrt::Windows::UI::Color m_progressColor;
		//winrt::hstring m_progressColor{ L"" };
        bool m_ishotp;
		float m_progress{ 100.0f };
        //double m_salary{ 0.0 };
        winrt::event<winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

        void RaisePropertyChanged(winrt::hstring const& propertyName);
    };
}

namespace winrt::OTP_Client_WinUI3::factory_implementation
{
    struct OTPItem : OTPItemT<OTPItem, implementation::OTPItem>
    {
    };
}