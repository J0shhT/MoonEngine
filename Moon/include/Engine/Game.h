#pragma once

#include "Graphics/Window.h"
#include "Object/Object.h"
#include <iostream>
#include <map>
#include <memory>
#include <assert.h>

namespace Moon {

	class Game {

		public:
			Game(Graphics::Window&);
			~Game() noexcept;

			//Singleton Getter
			static Game* singleton();

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
			static Game* instance;
			bool _isRunning;
			Graphics::Window& _targetWindow;
			std::shared_ptr<Object::Object> _rootObject;
			std::map<std::string, std::shared_ptr<Object::Object>> _gameObjects;

	};

	//CreateGameObject Methods
	template<typename T> std::shared_ptr<T> Game::CreateGameObject(std::string name, std::shared_ptr<Object::Object> parent)
	{
		assert(parent != nullptr);

		//Create object
		std::shared_ptr<T> object = std::make_shared<T>();

		if (name != "")
		{
			object->SetName(name);
		}
		object->SetParent(parent);

		//Add object to object lookup table
		this->_gameObjects[object->GetId()] = object;

		return object;
	}
	template<typename T> std::shared_ptr<T> Game::CreateGameObject(std::string name)
	{
		return this->CreateGameObject<T>(name, this->GetRootObject());
	}
	template<typename T> std::shared_ptr<T> Game::CreateGameObject(std::shared_ptr<Object::Object> parent)
	{
		return this->CreateGameObject<T>("", parent);
	}
	template<typename T> std::shared_ptr<T> Game::CreateGameObject()
	{
		return this->CreateGameObject<T>("", this->GetRootObject());
	}

}