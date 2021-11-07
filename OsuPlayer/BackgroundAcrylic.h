#pragma once

#include <pch.h>
#include <future>
#include <tuple>

class HostBackdropAcrylicBrush : public winrt::Windows::UI::Xaml::Media::XamlCompositionBrushBase
{
public:
	std::future<void> BuildHostBackdropAcrylicBrushAsync();
protected:
	void OnConnected() const;
private:
	static std::pair<float, float> TintOpacityToArithmeticCompositeEffectSourceAmount(float tintOpacity, float minThreshold);

	winrt::Windows::UI::Color LuminosityColor;
	float TintOpacity;
	constexpr inline static float OpacityMinThreshold = 0.3f;
};
