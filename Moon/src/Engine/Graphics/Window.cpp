#include "include/Engine/Graphics/Window.h"

#include "include/Engine/GameHandler.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

#include <GL/glew.h>
#include <GL/glut.h>

using namespace Moon::Graphics;

//Constructor
Window::Window(std::string title, int width, int height):
	_title(title), _width(width), _height(height)
{

	//Initialize SDL GL attributes (must do before window creation)
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);

	//Initialize SDL video subsystem (if it isn't already)
	if (!SDL_WasInit(SDL_INIT_VIDEO))
	{
		if (SDL_Init(SDL_INIT_VIDEO) == 0)
		{
			StandardOut::Print<std::string>(StandardOut::OutputType::Info,
				"Window::Window() - SDL video subsystem initialized"
			);
		}
		else
		{
			throw std::runtime_error("Window::Window() - SDL video subsystem failed to initialize");
		}
	}

	//Create the SDL window
	this->_window = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN
	);
	if (this->_window != NULL)
	{
		StandardOut::Print<std::string>(StandardOut::OutputType::Info, "Window::Window() - SDL window created");
	}
	else
	{
		throw std::runtime_error("Window::Window() - SDL window creation failed");
	}

	//Initialize GLContext
	this->_glContext = SDL_GL_CreateContext(this->_window);

	//Initialize GLEW
	if (glewInit() == GLEW_OK)
	{
		StandardOut::Print<std::string>(StandardOut::OutputType::Info,
			"Window::Window() - GLEW initialized"
		);
	}
	else
	{
		throw std::runtime_error("Window::Window() - GLEW initialization failed");
	}

}

//Destructor
Window::~Window()
{
	if (this->_window != nullptr)
	{
		this->DestroyWindow();
	}
}

//Member Getters
std::string Window::GetTitle() const
{
	return this->_title;
}
int Window::GetWidth() const
{
	return this->_width;
}
int Window::GetHeight() const
{
	return this->_height;
}
SDL_Window* Window::GetWindow() const
{
	return this->_window;
}
SDL_GLContext Window::GetGLContext() const
{
	return this->_glContext;
}

//Member Setters
void Window::SetTitle(std::string title)
{
	this->_title = title;
	SDL_SetWindowTitle(this->_window, title.c_str());
}
void Window::SetSize(int width, int height)
{
	this->_width = width;
	this->_height = height;
	SDL_SetWindowSize(this->_window, width, height);
}
void Window::SetWidth(int value)
{
	this->SetSize(value, this->_height);
}
void Window::SetHeight(int value)
{
	this->SetSize(this->_width, value);
}

//Methods
void Window::DestroyWindow()
{
	StandardOut::Print<std::string>(StandardOut::OutputType::Info, "Window::DestroyWindow() - SDL window destroyed");
	SDL_DestroyWindow(this->_window);
	this->_window = nullptr;
}
int Window::GetPositionX() const
{
	int value;
	SDL_GetWindowPosition(this->_window, &value, NULL);
	return value;
}
int Window::GetPositionY() const
{
	int value;
	SDL_GetWindowPosition(this->_window, NULL, &value);
	return value;
}
WindowPosition Window::GetPosition() const
{
	WindowPosition position;
	SDL_GetWindowPosition(this->_window, &position.x, &position.y);
	return position;
}
void Window::DispalyWindow(bool isVisible) const
{
	if (isVisible)
	{
		SDL_ShowWindow(this->_window);
	}
	else
	{
		SDL_HideWindow(this->_window);
	}
}
