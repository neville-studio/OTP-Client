#include "pch.h"
#include "OTPItem.h"
#include "OTPItem.g.cpp"
#include <winrt/Windows.UI.h>

namespace winrt::OTP_Client_WinUI3::implementation
{
    winrt::hstring OTPItem::FriendlyName() { return m_friendlyName; }
    void OTPItem::FriendlyName(winrt::hstring const& value)
    {
        if (m_friendlyName != value)
        {
            m_friendlyName = value;
            RaisePropertyChanged(L"FriendlyName");
        }
    }

    winrt::hstring OTPItem::SecretDigits() { return m_secretDigits; }
    void OTPItem::SecretDigits(winrt::hstring const& value)
    {
        if (m_secretDigits != value)
        {
            m_secretDigits = value;
            RaisePropertyChanged(L"SecretDigits");
        }
    }

    winrt::hstring OTPItem::RemainingTimeText() { return m_remainingTimeText; }
    void OTPItem::RemainingTimeText(winrt::hstring const& value)
    {
        if (m_remainingTimeText != value)
        {
            m_remainingTimeText = value;
            RaisePropertyChanged(L"RemainingTimeText");
        }
    }
   /* winrt::hstring OTPItem::ProgressColor() { return m_remainingTimeText; }
    void OTPItem::ProgressColor(winrt::hstring const& value)
    {
        if (m_progressColor != value)
        {
            m_progressColor = value;
            RaisePropertyChanged(L"ProgressColor");
        }
    }*/

    float OTPItem::Progress() { return m_progress; }
    void OTPItem::Progress(float const& value)
    {
        if (m_progress != value)
        {
            m_progress = value;
            RaisePropertyChanged(L"Progress");
            updateProgressColor();
        }
    }

    bool OTPItem::IsHOTP() { return m_ishotp; }
    void OTPItem::IsHOTP(bool const& value)
    {
        if (m_ishotp != value)
        {
            m_ishotp = value;
            RaisePropertyChanged(L"IsHOTP");
        }
    }

    winrt::event_token OTPItem::PropertyChanged(
        winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        return m_propertyChanged.add(handler);
    }

    void OTPItem::PropertyChanged(winrt::event_token const& token) noexcept
    {
        m_propertyChanged.remove(token);
    }

    void OTPItem::RaisePropertyChanged(winrt::hstring const& propertyName)
    {
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
    }

    winrt::Windows::UI::Color OTPItem::ProgressColor() {
        return m_progressColor;
    }
    void OTPItem::updateProgressColor() {
        using namespace winrt::Windows::UI;
        Color color;
        color.A = 255; // Alpha is fixed

        if (m_progress <= 50)
        {
            // Transition from Red to Gold (0-50 progress mapped to 0-1)
            float t = m_progress / 50.0f;
            color.R = static_cast<uint8_t>(255); // Red stays at 255
            color.G = static_cast<uint8_t>(255 * t); // Green increases with t
            color.B = static_cast<uint8_t>(0); // Blue stays at 0
        }
        else
        {
            // Transition from Gold to LimeGreen (51-100 progress mapped to 0-1)
            float t = (m_progress - 50) / 50.0f;
            color.R = static_cast<uint8_t>(255 * (1 - t)); // Red decreases with t
            color.G = static_cast<uint8_t>(255); // Green stays at 255
            color.B = static_cast<uint8_t>(0 + 164 * t); // Blue increases with t towards LimeGreen's blue component
        }
        ProgressColor(color);
        //if (m_progress < 20) 
        //    ProgressColor(Colors::Red());
        //else if (m_progress < 67) 
        //    ProgressColor(Colors::Gold());
        //else 
        //    ProgressColor(Colors::LimeGreen());
       
    }
    void OTPItem::ProgressColor(winrt::Windows::UI::Color const& value) {
        
        if (m_progressColor != value)
        {
            m_progressColor = value;
            RaisePropertyChanged(L"ProgressColor");
        }
    }
}