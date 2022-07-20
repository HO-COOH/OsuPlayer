#pragma once

#include <optional>
#include <functional>

namespace Utils
{
	template<typename Self>
	struct LazyInterface
	{
		auto& get()
		{
			auto self = static_cast<Self*>(this);
			if (self->m_value.has_value())
				return self->m_value.value();
			self->m_value.emplace(self->m_initializer());
			return *self->m_value;
		}

		void reset()
		{
			static_cast<Self*>(this)->m_value.reset();
		}
	};

	template<typename Initializer>
	class LazyStatic : public LazyInterface<LazyStatic<Initializer>>
	{
		friend LazyInterface<LazyStatic<Initializer>>;
		Initializer m_initializer;
		std::optional<decltype(m_initializer())> m_value;
	public:
		LazyStatic(Initializer&& initializer) : m_initializer{ std::forward<Initializer>(initializer) } {}
	};

	template<typename T>
	class LazyDynamic : public LazyInterface<LazyDynamic<T>>
	{
		friend LazyInterface<LazyDynamic<T>>;
		std::function<T()> m_initializer;
		std::optional<T> m_value;
	public:
		template<typename Initializer>
		LazyDynamic(Initializer&& initializer) : m_initializer{ std::forward<Initializer>(initializer) } {}

		template<typename Initializer>
		void setInitializer(Initializer&& initializer)
		{
			m_initializer = std::move(initializer);
		}
	};
}