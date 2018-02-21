#pragma once

#include "include/Common.h"

namespace Moon::Util {

	template <typename T>
	inline void Clamp(T &value, T min, T max)
	{
		if (value < min)
			value = min;
		if (value > max)
			value = max;
	}
	template <typename T>
	inline void ClampMin(T &value, T min)
	{
		if (value < min)
			value = min;
	}
	template <typename T>
	inline void ClampMax(T &value, T max)
	{
		if (value > max)
			value = max;
	}

	template <typename T>
	inline bool IsWeakPtrInitialized(std::weak_ptr<T> const& weak)
	{
		return !(!weak.owner_before(std::weak_ptr<T>{}) && !std::weak_ptr<T>{}.owner_before(weak));
	}

}