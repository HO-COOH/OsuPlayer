#pragma once

#include <winrt/Windows.UI.Xaml.Data.h>
namespace Utils
{
	template<typename Self>
	class PropertyChangeHelper
	{
	private:
		winrt::event<winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
	public:
		winrt::event_token PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& value)
		{
			return m_propertyChanged.add(value);
		}

		void PropertyChanged(winrt::event_token const& token)
		{
			m_propertyChanged.remove(token);
		}

		void raisePropertyChange(wchar_t const* propertyName)
		{
			m_propertyChanged(*(static_cast<Self*>(this)), winrt::Windows::UI::Xaml::Data::PropertyChangedEventArgs{ propertyName });
		}
	};
}