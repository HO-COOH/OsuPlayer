#include "pch.h"
#include "BackgroundAcrylic.h"
#include <winrt/Windows.Graphics.Effects.h>


std::future<void> HostBackdropAcrylicBrush::BuildHostBackdropAcrylicBrushAsync()
{
	winrt::Microsoft::Graphics::Canvas::Effects::ColorSourceEffect luminosityColorEffect;
	luminosityColorEffect.Name() = L"LuminosityColor";
	luminosityColorEffect.Color() = LuminosityColor;

	auto [amount1, amount2] = TintOpacityToArithmeticCompositeEffectSourceAmount(
		TintOpacity,
		OpacityMinThreshold
	);

	winrt::Microsoft::Graphics::Canvas::Effects::ArithmeticCompositeEffect luminosityBlendingEffect;

	return std::async([] {});
}

std::pair<float, float> HostBackdropAcrylicBrush::TintOpacityToArithmeticCompositeEffectSourceAmount(float tintOpacity, float minThreshold)
{
	return {};
}

void HostBackdropAcrylicBrush::OnConnected() const
{

}
