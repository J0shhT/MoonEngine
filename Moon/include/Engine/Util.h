#pragma once

#include <memory>

namespace Moon::Util {

	template <typename T>
	bool IsWeakPtrInitialized(std::weak_ptr<T> const& weak) {
		return !(!weak.owner_before(std::weak_ptr<T>{}) && !std::weak_ptr<T>{}.owner_before(weak));
	}

}