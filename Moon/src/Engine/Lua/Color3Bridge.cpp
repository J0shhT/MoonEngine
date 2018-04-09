/*

	Moon Engine - /Engine/Lua/Color3Bridge.cpp

	Updated: April 9th, 2018
	Contributers: @J0shhT

	**SEE HEADER (.h) FILE FOR DOCUMENTATION OF INTERFACES**

*/

#include "include/Engine/Lua/Color3Bridge.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

using namespace Moon;

/*
	OpenLibrary

	Registers all Color3Bridge library functions into the specified lua_State
*/
void Lua::Color3Bridge::OpenLibrary(lua_State* state)
{
	//Check whether or not the Color3 table already exists
	lua_getglobal(state, "Color3");
	if (!lua_istable(state, -1))
	{
		lua_pop(state, 1);
		lua_newtable(state);
	}

	//Color3.new()
	lua_pushcfunction(state, Lua::Color3Bridge::constructor);
	lua_setfield(state, 1, "new");

	//Color3.fromRGB()
	lua_pushcfunction(state, Lua::Color3Bridge::fromRGB);
	lua_setfield(state, 1, "fromRGB");

	lua_setglobal(state, "Color3");
}

/*
	RegisterType

	Registers the Color3 type with the specified lua_State
*/
void Lua::Color3Bridge::RegisterType(lua_State* state)
{
	luaL_newmetatable(state, Lua::Color3Bridge::Type);

	//Metamethods
	luaL_Reg metamethods[] = {
		{ "__index", Lua::Color3Bridge::__index },
		{ "__newindex", Lua::Color3Bridge::__newindex },
		{ "__add", Lua::Color3Bridge::__add },
		{ "__sub", Lua::Color3Bridge::__sub },
		{ "__mul", Lua::Color3Bridge::__mul },
		{ "__div", Lua::Color3Bridge::__div },
		{ "__eq", Lua::Color3Bridge::__eq },
		{ "__tostring", Lua::Color3Bridge::__tostring },
		{ NULL, NULL }
	};
	luaL_setfuncs(state, metamethods, 0);

	//Methods
	lua_pushstring(state, "__methods");
	lua_newtable(state);
	luaL_Reg methods[]{
		{ NULL, NULL }
	};
	luaL_setfuncs(state, methods, 0);
	lua_rawset(state, -3);

	//Property Getters
	lua_pushstring(state, "__getters");
	lua_newtable(state);
	luaL_Reg getters[]{
		{ "R", Lua::Color3Bridge::getR },
		{ "G", Lua::Color3Bridge::getG },
		{ "B", Lua::Color3Bridge::getB },
		{ NULL, NULL }
	};
	luaL_setfuncs(state, getters, 0);
	lua_rawset(state, -3);

	//Property Setters
	lua_pushstring(state, "__setters");
	lua_newtable(state);
	luaL_Reg setters[]{
		{ "R", Lua::ReadOnlyProperty },
		{ "G", Lua::ReadOnlyProperty },
		{ "B", Lua::ReadOnlyProperty },
		{ NULL, NULL }
	};
	luaL_setfuncs(state, setters, 0);
	lua_rawset(state, -3);

	lua_pushstring(state, "__metatable");
	lua_pushstring(state, "This metatable is locked");
	lua_rawset(state, -3);

	lua_pop(state, 1);
}

/*
	Push

	Pushes the specified Graphics::Color3 onto the Lua stack as a userdata.
*/
void Lua::Color3Bridge::Push(lua_State* state, Moon::Graphics::Color3* color)
{
	Moon::Graphics::Color3* udata = (Moon::Graphics::Color3*)lua_newuserdata(state, sizeof(*color));
	*udata = *color;
	luaL_getmetatable(state, Lua::Color3Bridge::Type);
	lua_setmetatable(state, -2);
}

/*
	Get

	Converts a Lua userdata Color3 at the specified index on the
	stack into a Graphics::Color3 and returns it.
*/
Moon::Graphics::Color3* Lua::Color3Bridge::Get(lua_State* state, int index)
{
	return (Moon::Graphics::Color3*)luaL_checkudata(state, index, Lua::Color3Bridge::Type);
}

/*
	**Lua Documentation**

	Color3 Color3.new(number r, number g, number b)

	Creates a new Color3 with the specified normalized r, g, and b values.
*/
int Lua::Color3Bridge::constructor(lua_State *state)
{
	float r, g, b;
	if (lua_gettop(state) == 3)
	{
		r = luaL_optnumber(state, 1, 1.0f);
		g = luaL_optnumber(state, 2, 1.0f);
		b = luaL_optnumber(state, 3, 1.0f);
		Moon::Graphics::Color3 color(r, g, b);
		Lua::Color3Bridge::Push(state, &color);
		return 1;
	}
	else if (lua_gettop(state) == 0)
	{
		r = 1.0f;
		g = 1.0f;
		b = 1.0f;
		Moon::Graphics::Color3 color(r, g, b);
		Lua::Color3Bridge::Push(state, &color);
		return 1;
	}
	else {
		return luaL_error(state, "Color3.new() requires either none or three arguments");
	}
}

/*
	**Lua Documentation**

	Color3 Color3.fromRGB(number r, number g, number b)

	Creates a new Color3 with the specified r, g, and b values (from 0-255).
*/
int Lua::Color3Bridge::fromRGB(lua_State *state)
{
	float r, g, b;
	if (lua_gettop(state) == 3)
	{
		r = luaL_optnumber(state, 1, 255.0f) / 255.0f;
		g = luaL_optnumber(state, 2, 255.0f) / 255.0f;
		b = luaL_optnumber(state, 3, 255.0f) / 255.0f;
		Moon::Graphics::Color3 color(r, g, b);
		Lua::Color3Bridge::Push(state, &color);
		return 1;
	}
	else if (lua_gettop(state) == 0)
	{
		r = 1.0f;
		g = 1.0f;
		b = 1.0f;
		Moon::Graphics::Color3 color(r, g, b);
		Lua::Color3Bridge::Push(state, &color);
		return 1;
	}
	else {
		return luaL_error(state, "Color3.fromRGB() requires either none or three arguments");
	}
}

/********************
	Metamethods
********************/

/*
	**Lua Documentation**

	Color3.__index (Metamethod)

	This is called by Lua when a property of Color3 is indexed.
*/
int Lua::Color3Bridge::__index(lua_State* state)
{
	std::string member;
	Moon::Graphics::Color3* color = Lua::Color3Bridge::Get(state, 1);
	if (color != nullptr)
	{
		member = luaL_checkstring(state, 2);
		lua_getmetatable(state, 1);
		lua_getfield(state, -1, "__getters");
		lua_getfield(state, -1, member.c_str());
		if (lua_iscfunction(state, -1)) {
			//The property exists, clean the stack, call the getter, and return
			lua_remove(state, -2);
			lua_remove(state, -2);
			lua_pushvalue(state, 1);
			lua_call(state, 1, 1);
			return 1;
		}
		else {
			//The property does not exist, maybe it's a method?
			lua_pop(state, 2);
			lua_getfield(state, -1, "__methods");
			lua_getfield(state, -1, member.c_str());
			if (lua_iscfunction(state, -1)) {
				//The method exists, clean the stack, and return the lua CFunction
				lua_remove(state, -2);
				lua_remove(state, -3);
				return 1;
			}
			else {
				lua_pop(state, 3);
				return luaL_error(state, "Attempt to index field '%s' (a nil value)", member.c_str());
			}
		}
	}
	return 0;
}

/*
	**Lua Documentation**

	Color3.__newindex (Metamethod)

	This is called by Lua when a property of Color3 is trying to be set.
*/
int Lua::Color3Bridge::__newindex(lua_State* state)
{
	std::string member;
	Moon::Graphics::Color3* color = Lua::Color3Bridge::Get(state, 1);
	if (color != nullptr)
	{
		member = luaL_checkstring(state, 2);
		lua_getmetatable(state, 1);
		lua_getfield(state, -1, "__setters");
		lua_getfield(state, -1, member.c_str());
		if (lua_iscfunction(state, -1)) {
			//The property exists, clean the stack, call the setter, and return
			lua_remove(state, -2);
			lua_remove(state, -2);
			lua_pushvalue(state, 1);
			lua_call(state, 1, 1);
			return 1;
		}
		else {
			//The property does not exist, throw error
			lua_pop(state, lua_gettop(state));
			return luaL_error(state, "Attempt to index field '%s' (a nil value)", member.c_str());
		}
	}
	return 0;
}

/*
	**Lua Documentation**

	Color3.__add (Metamethod)

	This is called by Lua when a Color3 is added with another Color3
*/
int Lua::Color3Bridge::__add(lua_State* state)
{
	Moon::Graphics::Color3* lhs = Lua::Color3Bridge::Get(state, 1);
	Moon::Graphics::Color3* rhs = Lua::Color3Bridge::Get(state, 2);
	if (lhs != nullptr && rhs != nullptr)
	{
		Moon::Graphics::Color3 color = *lhs;
		color += *rhs;
		Lua::Color3Bridge::Push(state, &color);
		return 1;
	}
	return 0;
}

/*
	**Lua Documentation**

	Color3.__sub (Metamethod)

	This is called by Lua when a Color3 is subtracted by another Color3
*/
int Lua::Color3Bridge::__sub(lua_State* state)
{
	Moon::Graphics::Color3* lhs = Lua::Color3Bridge::Get(state, 1);
	Moon::Graphics::Color3* rhs = Lua::Color3Bridge::Get(state, 2);
	if (lhs != nullptr && rhs != nullptr)
	{
		Moon::Graphics::Color3 color = *lhs;
		color -= *rhs;
		Lua::Color3Bridge::Push(state, &color);
		return 1;
	}
	return 0;
}

/*
	**Lua Documentation**

	Color3.__mul (Metamethod)

	This is called by Lua when a Color3 is multiplied by another Color3
*/
int Lua::Color3Bridge::__mul(lua_State* state)
{
	Moon::Graphics::Color3* lhs = Lua::Color3Bridge::Get(state, 1);
	Moon::Graphics::Color3* rhs = Lua::Color3Bridge::Get(state, 2);
	if (lhs != nullptr && rhs != nullptr)
	{
		Moon::Graphics::Color3 color = *lhs;
		color *= *rhs;
		Lua::Color3Bridge::Push(state, &color);
		return 1;
	}
	return 0;
}

/*
	**Lua Documentation**

	Color3.__div (Metamethod)

	This is called by Lua when a Color3 is divided by another Color3
*/
int Lua::Color3Bridge::__div(lua_State* state)
{
	Moon::Graphics::Color3* lhs = Lua::Color3Bridge::Get(state, 1);
	Moon::Graphics::Color3* rhs = Lua::Color3Bridge::Get(state, 2);
	if (lhs != nullptr && rhs != nullptr)
	{
		Moon::Graphics::Color3 color = *lhs;
		color /= *rhs;
		Lua::Color3Bridge::Push(state, &color);
		return 1;
	}
	return 0;
}

/*
	**Lua Documentation**

	Color3.__eq (Metamethod)

	This is called by Lua when a Color3 is compared to another
	Color3 using the equals operator (==)
*/
int Lua::Color3Bridge::__eq(lua_State* state)
{
	Moon::Graphics::Color3* lhs = Lua::Color3Bridge::Get(state, 1);
	Moon::Graphics::Color3* rhs = Lua::Color3Bridge::Get(state, 2);
	if (lhs != nullptr && rhs != nullptr)
	{
		lua_pushboolean(state, *lhs == *rhs);
	}
	else
	{
		lua_pushboolean(state, false);
	}
	return 1;
}

/*
**Lua Documentation**

Vector2.__tostring (Metamethod)

This is called by Lua when a Vector2 is used with the tostring() function
*/
int Lua::Color3Bridge::__tostring(lua_State* state)
{
	Moon::Graphics::Color3* color = Lua::Color3Bridge::Get(state, 1);
	if (color != nullptr)
	{
		lua_pushstring(state, color->ToString().c_str());
		return 1;
	}
	return 0;
}

/************************
	Property Getters
************************/

/*
	**Lua Documentation**

	Color3.R (Property)

	The red component of the Color3 (from 0-1)
*/
int Lua::Color3Bridge::getR(lua_State* state)
{
	Moon::Graphics::Color3* color = Lua::Color3Bridge::Get(state, 1);
	lua_pushnumber(state, color->GetRed());
	return 1;
}

/*
	**Lua Documentation**

	Color3.G (Property)

	The green component of the Color3 (from 0-1)
*/
int Lua::Color3Bridge::getG(lua_State* state)
{
	Moon::Graphics::Color3* color = Lua::Color3Bridge::Get(state, 1);
	lua_pushnumber(state, color->GetGreen());
	return 1;
}

/*
	**Lua Documentation**

	Color3.B (Property)

	The blue component of the Color3 (from 0-1)
*/
int Lua::Color3Bridge::getB(lua_State* state)
{
	Moon::Graphics::Color3* color = Lua::Color3Bridge::Get(state, 1);
	lua_pushnumber(state, color->GetBlue());
	return 1;
}