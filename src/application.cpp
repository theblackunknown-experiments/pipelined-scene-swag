#include "application.hpp"

#include <iostream>

#include <bpipe/scheduler/result.hpp>


namespace br {
    namespace app {

Application::Application(std::size_t width, std::size_t height)
    : viewDimension ( std::make_pair(width, height) )
    , data          ( )
    , update_steps  ( )
    , render_steps  ( )
    , scheduler     ( )
{
    //TODO
    //1.    Setup the scene
    //1.1   Ground plane ?
    //1.2   Load Initial mesh
    //1.2.1 Position it within the scene
    //2.    Setup initial scene viewport
    //3.    Setup camera
}
    
void Application::setViewportDimension( std::size_t width, std::size_t height )
{
    viewDimension = std::make_pair(width, height);
}
  
void Application::processSceneTasks( )
{
    const auto results = scheduler.executeSynchronous( 
        update_steps,
        data);
    
    for(const auto& result : results)
    {
        const auto step_name   = result.first;
        const auto step_result = result.second;
        std::cout << step_name << " step has " << (step_result.success ? "succeeds" : "failed") << "." << std::endl;
    }
}
  
void Application::renderScene( )
{                                                 
    /*const auto results = */scheduler.executeSynchronous( 
        render_steps,
        data);
    
    //for(auto result : results)
    //{
    //    const auto step_name   = result.first;
    //    const auto step_result = result.second;
    //    std::cout << step_name << " step has " << (step_result.success ? "succeeds" : "failed") << "." << std::endl;
    //}
}
  
    }
}
