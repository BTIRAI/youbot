#include "lua_condition_node.h"

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}



void looptable(lua_State *L1, lua_State* L2)

{
    //lua_createtable(L2, 0, 4);
    lua_newtable (L2);
    lua_pushglobaltable(L1);       // Get global table
    lua_pushnil(L1);               // put a nil key on stack
    while (lua_next(L1,-2) != 0) { // key(-1) is replaced by the next key(-1) in table(-2)
       // std::cout << lua_tostring(L1,-2) << std::endl;  // Get key(-2) name
        if (lua_isnumber(L1, -1))
        {
            std::cout << "is number" << std::endl;
            std::cout << lua_tostring(L1, -2) << " : " << lua_tonumber(L1, -1) << std::endl;
            lua_pushnumber(L2, lua_tonumber(L1, -1));
            lua_setglobal(L2, lua_tostring(L1, -2));
        }
        else if (lua_istable(L1, -1))
        {
//            std::cout << "is table" << std::endl;
//            std::cout << lua_tostring(L1, -2) << std::endl;
//            lua_pushstring(L2, lua_tostring(L1, -2));
//            lua_newtable(L2);
//            looptable(L1, L2);
//            lua_settable(L2, -3);
        }
//        lua_pushinteger();
//        lua_pushnil();

// TODO PUSH TABLE

        else if (lua_isboolean(L1, -1))
        {
            std::cout <<  lua_tostring(L1, -2)<< " is boolean. Value: "<<  lua_toboolean(L1, -1) << std::endl;
            std::cout << "Variable name set" << std::endl;
            lua_pushboolean(L2, lua_toboolean(L1, -1));
            lua_setglobal(L2, lua_tostring(L1, -2));
            std::cout << "Variable value set" << std::endl;
        }
        lua_pop(L1,1);               // remove value(-1), now key on top at(-1)
    }
    lua_pop(L1,1);
}





BT::LuaConditionNode::LuaConditionNode(std::string name, std::string filename, lua_State *lua_state) : BT::ConditionNode::ConditionNode(name)
{

    filename_ = filename;
    lua_state_ = luaL_newstate();
    luaL_openlibs(lua_state_);

    luaL_dofile(lua_state_, filename_.c_str());

    // call the lua function init

     lua_getglobal(lua_state_, "init");

    // does the call, 0 input, 1 output (fourth argument has error-handling use)

    if (lua_pcall(lua_state_, 0, 1, 0) != 0)
    {
            std::cout << "ERROR:  error running function init()" <<
                     lua_tostring(lua_state_, -1) << std::endl;
    }

     bool lua_return = lua_toboolean(lua_state_, -1);


     if(!lua_isboolean(lua_state_, -1))
     {

         if(lua_isnil(lua_state_, -1))
         {
             // the script returned NIL. Probably the user forgot to return a value
             std::cout << "ERROR: The script " << get_name()  << " returned NIL in " <<
                          "init() (did you forget to return true or false?)"<< std::endl;
         }
         else
         {
             // the script returned a generic error message
             std::cout  << lua_tostring(lua_state_, -1) << std::endl;
         }
         std::cout << "Something went wrong in the intialization of " << get_name() << std::endl;
     }
     else
     {
        // lua_return is boolean
         if(!lua_return)
         {
             std::cout << "WARNING: " << get_name() << " did not initialize correcly "<< std::endl;
         }
     }



}

BT::ReturnStatus BT::LuaConditionNode::Tick()
{

    luaL_dofile(lua_state_, filename_.c_str());

    // call the lua function tick
    lua_getglobal(lua_state_, "tick");

    // does the call, 0 input, 1 output (fourth argument has error-handling use)

    if (lua_pcall(lua_state_, 0, 1, 0) != 0)
    {
            std::cout << "ERROR:  error running function tick()" <<
                     lua_tostring(lua_state_, -1) << std::endl;
    }
    // retrieveing the return status
    bool lua_return = lua_toboolean(lua_state_, -1);


    // if the return is not a boolean, the Lua script returned somethig else, either nil (error in the return value)
    //or a generic error message
    if(!lua_isboolean(lua_state_, -1))
    {

        if(lua_isnil(lua_state_, -1))
        {
            // the script returned NIL. Probably the user forgot to return a value
            std::cout << "ERROR: The script " << get_name()  << " returned NIL (did you forget to return true or false?)"<< std::endl;
        }
        else
        {
            // the script returned a generic error message
            std::cout  << lua_tostring(lua_state_, -1) << std::endl;
        }
        std::cout << "Something went wrong in" << get_name() << std::endl;
        return BT::FAILURE; //TODO make BT::EXIT
    }

    //lua_close(lua_state_);

    if(lua_return)
    {
        return BT::SUCCESS;
    }
    else
    {
        return BT::FAILURE;
    }

}

void BT::LuaConditionNode::Finalize()
{
   lua_close(lua_state_);
}



