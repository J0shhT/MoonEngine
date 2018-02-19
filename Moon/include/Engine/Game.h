#pragma once

#include "Graphics/Window.h"
#include <iostream>
#include <vector>

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

			//Member Setters
			

			//Methods
			void ProcessEvents();
			void Update();
			void Render();
			void Exit(std::string error = "");
			
		private:
			static Game* instance;
			bool _isRunning;
			Graphics::Window& _targetWindow;
			//std::vector<Object::Object> _gameObjects;
			//std::vector<Object::Renderable> _renderableObjects;

	};

}