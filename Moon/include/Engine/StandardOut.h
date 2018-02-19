#pragma once

#include <iostream>
#include <string>
#include <sstream>

#define print(t, v) Moon::StandardOut::Print<t>(Moon::StandardOut::OutputType::Message, v)
#define append(t, v) Moon::StandardOut::Append<t>(Moon::StandardOut::OutputType::Message, v)

namespace Moon::StandardOut {

	enum OutputType {
		Message,
		Warning,
		Error,
		Debug
	};

	template<typename T> void Append(OutputType, T value);
	template<typename T> void Print(OutputType, T value);

	template<typename T>
	void Append(OutputType type, T value)
	{
		//todo: have special console text colors
		if (type == OutputType::Error)
		{
			std::cerr << value;
		}
		else
		{
			std::cout << value;
		}
	}

	template<typename T>
	void Print(OutputType type, T value)
	{
		Append<T>(type, value);
		if (type == OutputType::Error)
		{
			std::cerr << std::endl;
		}
		else
		{
			std::cout << std::endl;
		}
	}

}