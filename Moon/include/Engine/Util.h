#pragma once

#include <memory>

#include "GameHandler.h"

namespace Moon::Util {

	std::shared_ptr<Object::Object> GetRootObject()
	{
		return Moon::GameHandler::singleton()->GetRootObject();
	}

	template <typename T>
	bool IsWeakPtrInitialized(std::weak_ptr<T> const& weak) {
		return !(!weak.owner_before(std::weak_ptr<T>{}) && !std::weak_ptr<T>{}.owner_before(weak));
	}

}