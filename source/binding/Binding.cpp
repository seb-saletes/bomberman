/*
** binding.cpp for luatest in /home/care_j/work/luatest
**
** Made by care_j
** Login   <care_j@epitech.net>
**
*/

#include "Binding.hpp"

bbman::Binding::Binding(void)
{

}


bbman::Binding::~Binding(void)
{

}

int  bbman::Binding::runAI(size_t id)
{
  luabridge::LuaRef aiFct = luabridge::getGlobal(this->luaState, "ai");
  luabridge::LuaRef ret = aiFct(id);
  int move = ret["move"].cast<int>();
  if (ret["bomb"] == 1)
    move += 10;
  return (move);
}

void bbman::Binding::init(const std::string &scriptName)
{
  try {
    this->luaState = luaL_newstate();
    luaL_openlibs(this->luaState);
    luaL_dofile(this->luaState, scriptName.data());
    lua_pcall(this->luaState, 0, 0, 0);
    luabridge::getGlobalNamespace(this->luaState).addFunction("findPath", findPath)
    .addFunction("getPos", getPos)
    .addFunction("setMood", setMood)
    .addFunction("getMood", getMood)
    .addFunction("iAmSafe", iAmSafe)
    .addFunction("directionIsSafe", directionIsSafe);
  }
  catch (luabridge::LuaException const& e) {
    throw (e);
  }
}
