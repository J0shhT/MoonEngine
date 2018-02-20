#pragma once

#include "include/Common.h"

#include "include/Engine/Graphics/Window.h"

#include "include/Engine/Object/Object.h"

#include <GL/glew.h>
#include <GL/glut.h>

namespace Moon {

	class GameHandler {

		public:
			GameHandler(Graphics::Window&);
			~GameHandler() noexcept;

			//Singleton Getter
			static GameHandler* singleton();

			//Member Getters
			bool IsRunning() const;
			Graphics::Window& GetTargetWindow() const;
			std::shared_ptr<Object::Object> GetRootObject() const;

			//Member Setters
			

			//CreateGameObject Methods
			template<typename T> std::shared_ptr<T> CreateGameObject(
				std::string name,
				std::shared_ptr<Object::Object> parent
			);
			template<typename T> std::shared_ptr<T> CreateGameObject(
				std::string name
			);
			template<typename T> std::shared_ptr<T> CreateGameObject(
				std::shared_ptr<Object::Object> parent
			);
			template<typename T> std::shared_ptr<T> CreateGameObject();

			//Methods
			int GetGameObjectCount() const;
			void ProcessEvents();
			void Update();
			void Render();
			void Exit(std::string error = "");

			//Friends
			friend class Object::Object;
			
		private:
			static GameHandler* instance;
			bool _isRunning;
			Graphics::Window& _targetWindow;
			std::shared_ptr<Object::Object> _rootObject;
			std::map<std::string, std::shared_ptr<Object::Object>> _gameObjects;

	};

	//CreateGameObject Methods
	template<typename T> std::shared_ptr<T> GameHandler::CreateGameObject(std::string name, std::shared_ptr<Object::Object> parent)
	{
		assert(parent != nullptr);

		//Create object
		std::shared_ptr<T> object;
		try
		{
			object = std::make_shared<T>();
		}
		catch (const std::runtime_error& error)
		{

			Moon::StandardOut::Append<std::string>(Moon::StandardOut::OutputType::Error,
				"GameHandler::CreateGameObject() - Error creating game object ["
			);
			Moon::StandardOut::Append<std::string>(Moon::StandardOut::OutputType::Error, typeid(T).name());
			Moon::StandardOut::Append<std::string>(Moon::StandardOut::OutputType::Error, "] (");
			Moon::StandardOut::Append<std::string>(Moon::StandardOut::OutputType::Error, error.what());
			Moon::StandardOut::Print<std::string>(Moon::StandardOut::OutputType::Error, ")");
			return nullptr;

		}

		if (name != "")
		{
			object->SetName(name);
		}
		object->SetParent(parent);

		//Add object to object lookup table
		this->_gameObjects[object->GetId()] = object;

		return object;
	}
	template<typename T> std::shared_ptr<T> GameHandler::CreateGameObject(std::string name)
	{
		return this->CreateGameObject<T>(name, this->GetRootObject());
	}
	template<typename T> std::shared_ptr<T> GameHandler::CreateGameObject(std::shared_ptr<Object::Object> parent)
	{
		return this->CreateGameObject<T>("", parent);
	}
	template<typename T> std::shared_ptr<T> GameHandler::CreateGameObject()
	{
		return this->CreateGameObject<T>("", this->GetRootObject());
	}

}