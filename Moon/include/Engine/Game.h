#pragma once

#include "Graphics/Window.h"
#include "Object/Object.h"
#include <iostream>
#include <map>
#include <memory>

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
			

			//Methods
			template<typename T> std::shared_ptr<T> CreateGameObject(std::shared_ptr<Object::Object>parent = nullptr);
			int GetGameObjectCount() const;
			void ProcessEvents();
			void Update();
			void Render();
			void Exit(std::string error = "");
			
		private:
			static Game* instance;
			bool _isRunning;
			Graphics::Window& _targetWindow;
			std::shared_ptr<Object::Object> _rootObject;
			std::map<std::string, std::shared_ptr<Object::Object>> _gameObjects;

	};

	template<typename T>
	std::shared_ptr<T> Game::CreateGameObject(std::shared_ptr<Object::Object> parent)
	{
		//Create object
		std::shared_ptr<T> object = std::make_shared<T>();

		//Set parent
		if (parent == nullptr)
		{
			//All objects *must* have a parent, so use the root object
			object->SetParent(this->GetRootObject());
		}
		else
		{
			object->SetParent(parent);
		}

		//Add object to object lookup table
		this->_gameObjects[object->GetId()] = object;

		return object;
	}

}