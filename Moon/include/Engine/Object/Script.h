#pragma once

#include "include/Common.h"

#include "Object.h"

#include "include/Engine/ContentProvider.h"
#include "include/Engine/Lua/LuaHandler.h"

namespace Moon::Object {

	class Script : public Object {

	public:
		Script();
		virtual ~Script() noexcept;

		//Member Getters
		bool IsLoaded() const;

		//Member Setters


		//Methods
		void LoadFromFile(std::string filePath);
		void LoadFromString(std::string& code);
		void Execute();

	protected:
		//Members
		bool _isLoaded;
		ContentId _scriptContentId;
		std::string _threadId;

	};

}