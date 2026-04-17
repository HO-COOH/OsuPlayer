#pragma once
#include <format>

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Mode>(Mode const& mode)
	{
		switch (mode)
		{
			case Mode::Osu: return L"Mode.Osu";
			case Mode::Taiko: return L"Mode.Taiko";
			case Mode::Catch: return L"Mode.Catch";
			case Mode::Mania: return L"Mode.Mania";
		}
		throw std::exception{ "Mode.Unknown" };
	}

	template<>
	inline std::wstring ToString<SampleSet>(SampleSet const& mode)
	{
		switch (mode)
		{
			case SampleSet::None: return L"SampleSet.Auto";
			case SampleSet::Normal: return L"SampleSet.Normal";
			case SampleSet::Soft: return L"SampleSet.Soft";
			case SampleSet::Drum: return L"SampleSet.Drum";
		}
		throw std::exception{ "SampleSet.Unknown" };
	}

	template<>
	inline std::wstring ToString<HitObject::HitSample>(HitObject::HitSample const& hitSample)
	{
		return std::format(L"{}:{}:{}:{}:",
			static_cast<int>(hitSample.normalSet),
			static_cast<int>(hitSample.additionSet),
			hitSample.index,
			hitSample.volume
			//hitSample.filename
		);
	}

	template<>
	inline std::wstring ToString<Slider::CurveType>(Slider::CurveType const& curveType)
	{
		return std::wstring{ static_cast<wchar_t>(curveType) };
	}

	template<>
	inline std::wstring ToString<Coord>(Coord const& coord)
	{
		return std::format(L"{},{}", coord.x, coord.y);
	}

	template<>
	inline std::wstring ToString<std::vector<int>>(std::vector<int> const& vec)
	{
		return std::accumulate(
			std::next(vec.begin()),
			vec.end(),
			std::to_wstring(vec.front()),
			[](std::wstring const& lhs, int rhs) {
				return std::format(L"{}, {}", lhs, rhs);
			}
		);
	}

	template<>
	inline std::wstring ToString<std::vector<HitObject::HitSoundFlags>>(std::vector<HitObject::HitSoundFlags> const& hitSounds)
	{
		return std::accumulate(
			std::next(hitSounds.begin()),
			hitSounds.end(),
			std::to_wstring(hitSounds.front().raw()),
			[](std::wstring const& lhs, HitObject::HitSoundFlags rhs)
			{
				return std::format(L"{}|{}", lhs, rhs.raw());
			}
		);
	}

	template<>
	inline std::wstring ToString<std::vector<Slider::EdgeSet>>(std::vector<Slider::EdgeSet> const& edgeSets)
	{
		return std::accumulate(
			std::next(edgeSets.begin()),
			edgeSets.end(),
			std::format(L"{}:{}", static_cast<int>(edgeSets.front().normalSet), static_cast<int>(edgeSets.front().additionSet)),
			[](std::wstring const& lhs, Slider::EdgeSet rhs)
			{
				return std::format(L"{}|{}:{}", lhs, static_cast<int>(rhs.normalSet), static_cast<int>(rhs.additionSet));
			}
		);
	}
}