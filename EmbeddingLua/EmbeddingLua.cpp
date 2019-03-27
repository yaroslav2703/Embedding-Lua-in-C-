#include <iostream>
#include <string>


extern "C" {
#include "Lua535\include\lua.h"
#include "Lua535\include\lauxlib.h"
#include "Lua535\include\lualib.h"
}

#ifdef _WIN32
#pragma comment(lib,"lua535/liblua53.a")
#endif 


bool CheckLua(lua_State *L, int r)
{
	if (r != LUA_OK)
	{
		std::string errormsg = lua_tostring(L, -1);
		std::cout << errormsg << std::endl;
		return false;
	}
	return true;
}

int lua_HostFunction(lua_State *L)
{
	float a = (float)lua_tonumber(L, 1);
	float b = (float)lua_tonumber(L, 2);
	std::cout << "[CPP S4] HostFunction(" << a << ", " << b << ") called from Lua" << std::endl;
	float c = a * b;
	lua_pushnumber(L, c);
	return 1;
}



int main() {

	struct Player
	{  
		std::string title;
		std::string name;
		std::string family;
		int level;
	} player;

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	lua_register(L, "HostFunction", lua_HostFunction);

	if(CheckLua(L, luaL_dofile(L, "ex.lua"))) {

		// Stage 1: Just read simple variables
		std::cout << "[CPP] Stage 1 - Read Simple Variables" << std::endl;
		lua_getglobal(L, "a");
		if (lua_isnumber(L, -1)) std::cout << "[CPP S1] a = " << (int)lua_tointeger(L, -1) << std::endl;
		lua_getglobal(L, "b");
		if (lua_isnumber(L, -1)) std::cout << "[CPP S1] b = " << (int)lua_tointeger(L, -1) << std::endl;
		lua_getglobal(L, "c");
		if (lua_isnumber(L, -1)) std::cout << "[CPP S1] c = " << (int)lua_tointeger(L, -1) << std::endl;
		lua_getglobal(L, "d");
		if (lua_isstring(L, -1)) std::cout << "[CPP S1] d = " << lua_tostring(L, -1) << std::endl << std::endl;

		// Stage 2: Read Table Object
		std::cout << "[CPP] Stage 2 - Read Table (Key/Value pairs)" << std::endl;
		lua_getglobal(L, "player");
		if (lua_istable(L, -1))
		{
			lua_pushstring(L, "Name");
			lua_gettable(L, -2);
			player.name = lua_tostring(L, -1);
			lua_pop(L, 1);

			lua_pushstring(L, "Family");
			lua_gettable(L, -2);
			player.family = lua_tostring(L, -1);
			lua_pop(L, 1);

			lua_pushstring(L, "Title");
			lua_gettable(L, -2);
			player.title = lua_tostring(L, -1);
			lua_pop(L, 1);

			lua_pushstring(L, "Level");
			lua_gettable(L, -2);
			player.level = (int)lua_tointeger(L, -1);
			lua_pop(L, 1);
		}
		std::cout << "[CPP S2] " << player.title << " " << player.name << " of " << player.family << " [Lvl: " << player.level << "]" << std::endl << std::endl;

		// Stage 3: Call Lua Function
		std::cout << "[CPP] Stage 3 - Call Lua Function" << std::endl;
		lua_getglobal(L, "CalledFromCPP1");
		if (lua_isfunction(L, -1))
		{
			lua_pushnumber(L, 5.0f);
			lua_pushnumber(L, 6.0f);
			std::cout << "[CPP S3] Calling 'CalledFromCPP1' in lua script" << std::endl;
			if (CheckLua(L, lua_pcall(L, 2, 1, 0)))
			{
				std::cout << "[CPP S3] 'CalledFromCPP1' returned " << (float)lua_tonumber(L, -1) << std::endl << std::endl;
			}
		}

		// Stage 4: Call Lua Function, which calls C++ Function
		std::cout << "[CPP] Stage 4 - Call Lua Function, whcih in turn calls C++ Function" << std::endl;
		lua_getglobal(L, "CalledFromCPP2");
		if (lua_isfunction(L, -1))
		{
			lua_pushnumber(L, 5.0f);
			lua_pushnumber(L, 6.0f);
			std::cout << "[CPP S4] Calling 'CalledFromCPP2' in lua script" << std::endl;
			if (CheckLua(L, lua_pcall(L, 2, 1, 0)))
			{
				std::cout << "[CPP S4] 'CalledFromCPP2' returned " << (float)lua_tonumber(L, -1) << std::endl << std::endl;
			}
		}
		
	}
	system("pause");
	lua_close(L);

	return 0;
}