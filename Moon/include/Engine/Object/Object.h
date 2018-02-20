#pragma once

#include <string>
#include <map>
#include <memory>

#define DEFINE_OBJECT_CONSTRUCTOR() \
			this->_guid = boost::lexical_cast<std::string>(boost::uuids::random_generator()())

#define DeleteGameObject(o) o->Delete(); o.reset()

namespace Moon::Object {

	class Object: public std::enable_shared_from_this<Object> {

		public:
			Object();
			~Object() noexcept;

			//Member Getters
			std::string GetType() const;
			std::string GetId() const;
			std::map<std::string, std::weak_ptr<Object>> GetChildren() const;
			std::weak_ptr<Object> GetParent() const;
			std::string GetName() const;
			bool IsLocked() const;

			//Member Setters
			void SetParent(std::shared_ptr<Object>);
			void SetName(std::string);
			void Lock();
			void Unlock();

			//Methods
			int GetChildrenCount() const;
			std::shared_ptr<Object> FindChildById(std::string) const;
			std::shared_ptr<Object> FindFirstChild(std::string) const;
			bool IsA(std::string type) const;
			void Delete();

			void AddChild(std::shared_ptr<Object>);
			void RemoveChild(std::string guid);

		private:
			//Methods
			void _forceDelete();

			//Members
			const std::string _objectType = "Object";
			std::string _guid;
			std::map<std::string, std::weak_ptr<Object>> _children;
			std::weak_ptr<Object> _parent;
			std::string _name = "Object";
			bool _isLocked = false;

	};

}