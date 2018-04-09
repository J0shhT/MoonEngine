/*

	Moon Engine - /Engine/Lua/Vector2Bridge.cpp

	Updated: April 8th, 2018
	Contributers: @J0shhT

	**SEE HEADER (.h) FILE FOR DOCUMENTATION OF INTERFACES**

*/

#include "include/Engine/Lua/Vector2Bridge.h"

#include "include/Engine/Util.h"
#include "include/Engine/StandardOut.h"

using namespace Moon;

/*
	OpenLibrary

	Registers all Vector2Bridge library functions into the specified lua_State
*/
void Lua::Vector2Bridge::OpenLibrary(lua_State* state)
{
	//Check whether or not the Vector2 table already exists
	lua_getglobal(state, "Vector2");
	if (!lua_istable(state, -1))
	{
		lua_pop(state, 1);
		lua_newtable(state);
	}

	//Vector2.new()
	lua_pushcfunction(state, Lua::Vector2Bridge::constructor);
	lua_setfield(state, 1, "new");

	lua_setglobal(state, "Vector2");
}

/*
	RegisterType

	Registers the Vector2 type with the specified lua_State
*/
void Lua::Vector2Bridge::RegisterType(lua_State* state)
{
	luaL_newmetatable(state, Lua::Vector2Bridge::Type);
	
	//Metamethods
	luaL_Reg metamethods[] = {
		{ "__index", Lua::Vector2Bridge::__index },
		{ "__newindex", Lua::Vector2Bridge::__newindex },
		{ "__add", Lua::Vector2Bridge::__add },
		{ "__sub", Lua::Vector2Bridge::__sub },
		{ "__mul", Lua::Vector2Bridge::__mul },
		{ "__div", Lua::Vector2Bridge::__div },
		{ "__unm", Lua::Vector2Bridge::__unm },
		{ "__eq", Lua::Vector2Bridge::__eq },
		{ "__lt", Lua::Vector2Bridge::__lt },
		{ "__le", Lua::Vector2Bridge::__le },
		{ "__tostring", Lua::Vector2Bridge::__tostring },
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
		{ "X", Lua::Vector2Bridge::getX },
		{ "Y", Lua::Vector2Bridge::getY },
		{ "Magnitude", Lua::Vector2Bridge::getMagnitude },
		{ "Area", Lua::Vector2Bridge::getArea },
		{ "Angle", Lua::Vector2Bridge::getAngle },
		{ NULL, NULL }
	};
	luaL_setfuncs(state, getters, 0);
	lua_rawset(state, -3);

	//Property Setters
	lua_pushstring(state, "__setters");
	lua_newtable(state);
	luaL_Reg setters[]{
		{ "X", Lua::ReadOnlyProperty },
		{ "Y", Lua::ReadOnlyProperty },
		{ "Magnitude", Lua::ReadOnlyProperty },
		{ "Area", Lua::ReadOnlyProperty },
		{ "Angle", Lua::ReadOnlyProperty },
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

	Pushes the specified Physics::Vector2 onto the Lua stack as a userdata.
*/
void Lua::Vector2Bridge::Push(lua_State* state, Moon::Physics::Vector2* vec2)
{
	Moon::Physics::Vector2* udata = (Moon::Physics::Vector2*)lua_newuserdata(state, sizeof(*vec2));
	*udata = *vec2;
	luaL_getmetatable(state, Lua::Vector2Bridge::Type);
	lua_setmetatable(state, -2);
}

/*
	Get

	Converts a Lua userdata Vector2 at the specified index on the 
	stack into a Physics::Vector2 and returns it.
*/
Moon::Physics::Vector2* Lua::Vector2Bridge::Get(lua_State* state, int index)
{
	return (Moon::Physics::Vector2*)luaL_checkudata(state, index, Lua::Vector2Bridge::Type);
}

/*
	**Lua Documentation**

	Vector2 Vector2.new(number x, number y)

	Creates a new Vector2 with the specified x and y values.
*/
int Lua::Vector2Bridge::constructor(lua_State *state)
{
	double x, y;
	if (lua_gettop(state) == 2)
	{
		x = luaL_optnumber(state, 1, 0.0);
		y = luaL_optnumber(state, 2, 0.0);
		Moon::Physics::Vector2 vec(x, y);
		Lua::Vector2Bridge::Push(state, &vec);
		return 1;
	}
	else if (lua_gettop(state) == 0)
	{
		x = 0.0;
		y = 0.0;
		Moon::Physics::Vector2 vec(x, y);
		Lua::Vector2Bridge::Push(state, &vec);
		return 1;
	}
	else {
		return luaL_error(state, "Vector2.new() requires either none or two arguments");
	}
}

/********************
	Metamethods
********************/

/*
	**Lua Documentation**

	Vector2.__index (Metamethod)

	This is called by Lua when a property of Vector2 is indexed.
*/
int Lua::Vector2Bridge::__index(lua_State* state)
{
	std::string member;
	Moon::Physics::Vector2* vec = Lua::Vector2Bridge::Get(state, 1);
	if (vec != nullptr)
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

	Vector2.__newindex (Metamethod)

	This is called by Lua when a property of Vector2 is trying to be set.
*/
int Lua::Vector2Bridge::__newindex(lua_State* state)
{
	std::string member;
	Moon::Physics::Vector2* vec = Lua::Vector2Bridge::Get(state, 1);
	if (vec != nullptr)
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

	Vector2.__add (Metamethod)

	This is called by Lua when a Vector2 is added with another Vector2
*/
int Lua::Vector2Bridge::__add(lua_State* state)
{
	Moon::Physics::Vector2* lhs = Lua::Vector2Bridge::Get(state, 1);
	Moon::Physics::Vector2* rhs = Lua::Vector2Bridge::Get(state, 2);
	if (lhs != nullptr && rhs != nullptr)
	{
		Moon::Physics::Vector2 vec = *lhs;
		vec += *rhs;
		Lua::Vector2Bridge::Push(state, &vec);
		return 1;
	}
	return 0;
}

/*
	**Lua Documentation**

	Vector2.__sub (Metamethod)

	This is called by Lua when a Vector2 is subtracted by another Vector2
*/
int Lua::Vector2Bridge::__sub(lua_State* state)
{
	Moon::Physics::Vector2* lhs = Lua::Vector2Bridge::Get(state, 1);
	Moon::Physics::Vector2* rhs = Lua::Vector2Bridge::Get(state, 2);
	if (lhs != nullptr && rhs != nullptr)
	{
		Moon::Physics::Vector2 vec = *lhs;
		vec -= *rhs;
		Lua::Vector2Bridge::Push(state, &vec);
		return 1;
	}
	return 0;
}

/*
	**Lua Documentation**

	Vector2.__mul (Metamethod)

	This is called by Lua when a Vector2 is multiplied by another Vector2
*/
int Lua::Vector2Bridge::__mul(lua_State* state)
{
	Moon::Physics::Vector2* lhs = Lua::Vector2Bridge::Get(state, 1);
	Moon::Physics::Vector2* rhs = Lua::Vector2Bridge::Get(state, 2);
	if (lhs != nullptr && rhs != nullptr)
	{
		Moon::Physics::Vector2 vec = *lhs;
		vec *= *rhs;
		Lua::Vector2Bridge::Push(state, &vec);
		return 1;
	}
	return 0;
}

/*
	**Lua Documentation**

	Vector2.__div (Metamethod)

	This is called by Lua when a Vector2 is divided by another Vector2
*/
int Lua::Vector2Bridge::__div(lua_State* state)
{
	Moon::Physics::Vector2* lhs = Lua::Vector2Bridge::Get(state, 1);
	Moon::Physics::Vector2* rhs = Lua::Vector2Bridge::Get(state, 2);
	if (lhs != nullptr && rhs != nullptr)
	{
		Moon::Physics::Vector2 vec = *lhs;
		vec /= *rhs;
		Lua::Vector2Bridge::Push(state, &vec);
		return 1;
	}
	return 0;
}

/*
	**Lua Documentation**

	Vector2.__unm (Metamethod)

	This is called by Lua when a Vector2 is used with the unary operator (-)
*/
int Lua::Vector2Bridge::__unm(lua_State* state)
{
	Moon::Physics::Vector2* vec = Lua::Vector2Bridge::Get(state, 1);
	if (vec != nullptr)
	{
		vec->SetX(-(vec->GetX()));
		vec->SetY(-(vec->GetX()));
		Lua::Vector2Bridge::Push(state, vec);
		return 1;
	}
	return 0;
}

/*
	**Lua Documentation**

	Vector2.__eq (Metamethod)

	This is called by Lua when a Vector2 is compared to another
	Vector3 using the equals operator (==)
*/
int Lua::Vector2Bridge::__eq(lua_State* state)
{
	Moon::Physics::Vector2* lhs = Lua::Vector2Bridge::Get(state, 1);
	Moon::Physics::Vector2* rhs = Lua::Vector2Bridge::Get(state, 2);
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

	Vector2.__lt (Metamethod)

	This is called by Lua when a Vector2 is compared to another 
	Vector2 with the less-than operator (<)
*/
int Lua::Vector2Bridge::__lt(lua_State* state)
{
	Moon::Physics::Vector2* lhs = Lua::Vector2Bridge::Get(state, 1);
	Moon::Physics::Vector2* rhs = Lua::Vector2Bridge::Get(state, 2);
	if (lhs != nullptr && rhs != nullptr)
	{
		lua_pushboolean(state, *lhs < *rhs);
	}
	else
	{
		lua_pushboolean(state, false);
	}
	return 1;
}

/*
	**Lua Documentation**

	Vector2.__le (Metamethod)

	This is called by Lua when a Vector2 is compared to another 
	Vector2 with the less-than-or-equal-to operator (<=)
*/
int Lua::Vector2Bridge::__le(lua_State* state)
{
	Moon::Physics::Vector2* lhs = Lua::Vector2Bridge::Get(state, 1);
	Moon::Physics::Vector2* rhs = Lua::Vector2Bridge::Get(state, 2);
	if (lhs != nullptr && rhs != nullptr)
	{
		lua_pushboolean(state, *lhs <= *rhs);
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
int Lua::Vector2Bridge::__tostring(lua_State* state)
{
	Moon::Physics::Vector2* vec = Lua::Vector2Bridge::Get(state, 1);
	if (vec != nullptr)
	{
		std::ostringstream vecAsString;
		vecAsString << "(" << std::to_string(vec->GetX()) << ", " << std::to_string(vec->GetY()) << ")";
		lua_pushstring(state, vecAsString.str().c_str());
		return 1;
	}
	return 0;
}

/************************
	Property Getters
************************/

/*
	**Lua Documentation**

	Vector2.X (Property)

	The X component of the Vector2
*/
int Lua::Vector2Bridge::getX(lua_State* state)
{
	Moon::Physics::Vector2* vec = Lua::Vector2Bridge::Get(state, 1);
	lua_pushnumber(state, vec->GetX());
	return 1;
}

/*
	**Lua Documentation**

	Vector2.Y (Property)

	The Y component of the Vector2
*/
int Lua::Vector2Bridge::getY(lua_State* state)
{
	Moon::Physics::Vector2* vec = Lua::Vector2Bridge::Get(state, 1);
	lua_pushnumber(state, vec->GetY());
	return 1;
}

/*
	**Lua Documentation**

	Vector2.Magnitude (Property)

	The length of the Vector2
*/
int Lua::Vector2Bridge::getMagnitude(lua_State* state)
{
	Moon::Physics::Vector2* vec = Lua::Vector2Bridge::Get(state, 1);
	lua_pushnumber(state, vec->GetMagnitude());
	return 1;
}

/*
	**Lua Documentation**

	Vector2.Area (Property)

	The area of the Vector2
*/
int Lua::Vector2Bridge::getArea(lua_State* state)
{
	Moon::Physics::Vector2* vec = Lua::Vector2Bridge::Get(state, 1);
	lua_pushnumber(state, vec->GetArea());
	return 1;
}

/*
	**Lua Documentation**

	Vector2.Angle (Property)

	The angle that the Vector2 would form in radians
*/
int Lua::Vector2Bridge::getAngle(lua_State* state)
{
	Moon::Physics::Vector2* vec = Lua::Vector2Bridge::Get(state, 1);
	lua_pushnumber(state, vec->GetAngle());
	return 1;
}