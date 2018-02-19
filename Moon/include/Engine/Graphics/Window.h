#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <string>

namespace Moon::Graphics {

	struct WindowPosition {
		int x;
		int y;
	};

	class Window {

		public:
			Window(std::string title, int width, int height);
			~Window() noexcept;

			//Member Getters
			std::string GetTitle() const;
			int GetWidth() const;
			int GetHeight() const;
			SDL_Window* GetWindow() const;
			SDL_GLContext GetGLContext() const;

			//Member Setters
			void SetTitle(std::string);
			void SetSize(int width, int height);
			void SetWidth(int);
			void SetHeight(int);

			//Methods
			void DestroyWindow();
			int GetPositionX() const;
			int GetPositionY() const;
			WindowPosition GetPosition() const;

		private:
			std::string _title;
			int _width;
			int _height;
			SDL_Window* _window;
			SDL_GLContext _glContext;
	};

}