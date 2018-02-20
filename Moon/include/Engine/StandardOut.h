#pragma once

#include "include/Common.h"

#include <Windows.h>

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
		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		switch (type)
		{
			case OutputType::Warning:
				SetConsoleTextAttribute(consoleHandle, 14);
				break;
			case OutputType::Error:
				SetConsoleTextAttribute(consoleHandle, 12);
				break;
			case OutputType::Debug:
				SetConsoleTextAttribute(consoleHandle, 13);
				break;
			default:
				SetConsoleTextAttribute(consoleHandle, 15);
				break;
		}
		if (type == OutputType::Error)
		{
			std::cerr << value;
		}
		else
		{
			std::cout << value;
		}
		SetConsoleTextAttribute(consoleHandle, 15);
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