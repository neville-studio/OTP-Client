#include "pch.h"
#include "OTPItem.h"
#include "OTPItem.g.cpp"

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
    winrt::hstring OTPItem::ProgressColor() { return m_remainingTimeText; }
    void OTPItem::ProgressColor(winrt::hstring const& value)
    {
        if (m_progressColor != value)
        {
            m_progressColor = value;
            RaisePropertyChanged(L"ProgressColor");
        }
    }

    float OTPItem::Progress() { return m_progress; }
    void OTPItem::Progress(float const& value)
    {
        if (m_progress != value)
        {
            m_progress = value;
            RaisePropertyChanged(L"Progress");
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
}