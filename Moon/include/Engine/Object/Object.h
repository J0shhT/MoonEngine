#pragma once

#include "include/Common.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

#define DEFINE_OBJECT_CONSTRUCTOR(t) \
			this->_objectType = t; \
			this->_name = t; \
			this->_guid = boost::lexical_cast<std::string>(boost::uuids::random_generator()()); \
			this->_createdAt = std::chrono::high_resolution_clock::now() \

#define DEFINE_SINGLETON_OBJECT(o) \
			if (o::_isInstanced) { throw std::runtime_error("Cannot create more than one singleton object"); } \
			o::_isInstanced = true \

#define DeleteGameObject(o) if (o->Delete()) { o.reset(); }

namespace Moon::Object {

	class Object : public std::enable_shared_from_this<Object> {

		public:
			Object();
			virtual ~Object() noexcept;

			//Member Getters
			std::string GetType() const;
			std::string GetId() const;
			std::map<std::string, std::weak_ptr<Object>> GetChildren() const;
			std::shared_ptr<Object> GetParent() const;
			std::string GetName() const;
			bool IsLocked() const;

			//Member Setters
			void SetParent(std::shared_ptr<Object>);
			void SetName(std::string);
			void Lock();
			void Unlock();

			//Methods
			int GetChildrenCount() const;
			double GetAge() const;
			std::shared_ptr<Object> FindChildById(std::string) const;
			std::shared_ptr<Object> FindFirstChild(std::string) const;
			template<typename T> bool IsA();
			bool Delete();

			void AddChild(std::shared_ptr<Object>);
			void RemoveChild(std::string guid);

		protected:
			//Methods
			void _forceDelete();

			//Members
			std::string _objectType;
			std::string _guid;
			std::chrono::time_point<std::chrono::steady_clock> _createdAt;
			std::map<std::string, std::weak_ptr<Object>> _children;
			std::weak_ptr<Object> _parent;
			std::string _name;
			bool _isLocked = false;

			//For singleton objects, does nothing otherwise
			static bool _isInstanced;

	};

	template<typename T>
	bool Object::IsA()
	{
		return (dynamic_cast<T*>(this) != nullptr);
	}

}