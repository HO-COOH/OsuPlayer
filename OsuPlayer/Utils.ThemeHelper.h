#pragma once
#include "pch.h"

namespace Utils
{
	class ThemeHelper
	{
	public:
		static winrt::Windows::UI::Xaml::ElementTheme ActualTheme();
		static winrt::Windows::UI::Xaml::ElementTheme RootTheme();
		static void RootTheme(winrt::Windows::UI::Xaml::ElementTheme theme);
		static void Initialize();

		static bool IsDarkTheme();
		static void UpdateTitlebarColors();
	private:
		constexpr inline static auto SelectedAppThemeKey = L"SelectedAppTheme";
		static inline winrt::Windows::UI::Xaml::Window CurrentApplicationWindow{ nullptr };
		static inline winrt::Windows::UI::ViewManagement::UISettings uiSettings;
		static inline winrt::event_revoker<winrt::Windows::UI::ViewManagement::IUISettings3> uiSettingsEventRevoker;

		static void UiSettings_ColorValuesChanged(winrt::Windows::UI::ViewManagement::UISettings sender, winrt::Windows::Foundation::IInspectable const& args);


	};

}