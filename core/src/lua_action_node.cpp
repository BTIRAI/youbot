#include "lua_action_node.h"
extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}



BT::LuaActionNode::LuaActionNode(std::string name, std::string filename, lua_State *lua_state, BlackBoardCmd* blackboard_cmd) : BT::ActionNode::ActionNode(name)
{
    filename_ = filename;
    lua_state_ = lua_state;
    blackboard_cmd_ = blackboard_cmd;

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

BT::ReturnStatus BT::LuaActionNode::Tick()
{
    set_status(BT::RUNNING);


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
    set_lua_script_done(true);


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
        return BT::FAILURE;
    }
    else
    {

        if(lua_return)
        {
            set_status(BT::SUCCESS);
            return BT::SUCCESS;
        }
        else
        {
            set_status(BT::FAILURE);
            return BT::FAILURE;
        }
    }

}

void BT::LuaActionNode::Finalize()
{
    Halt();
    while (!lua_script_done())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

}

void BT::LuaActionNode::Halt()
{
    set_status(BT::HALTED);
    while (!lua_script_done());
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

bool BT::LuaActionNode::lua_script_done()
{
    std::lock_guard<std::mutex> LockGuard(lua_script_done_mutex_);

    return lua_script_done_;
}

void BT::LuaActionNode::set_lua_script_done(bool lua_script_done)
{
    std::lock_guard<std::mutex> LockGuard(lua_script_done_mutex_);
    lua_script_done_ = lua_script_done;
}

int BT::LuaActionNode::lua_is_halted(lua_State* L)
{
    lua_pushboolean(L, is_halt_requested());
    return 1; //number of returning values
}


void BT::LuaActionNode::LuaWriteToBlackboard(lua_State* L, std::string name, std::string type, yarp::os::Value value)
{

}



void BT::LuaActionNode::LineHookFunc(lua_State *L, lua_Debug *ar)
{
    if(ar->event == LUA_HOOKLINE)
        if(is_halt_requested() == true)
            luaL_error(L, "HALTED");
}
