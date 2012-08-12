
#include <string>
#include <memory>
#include <iostream>

#include <functional>
                      
#include <GL/glew.h>

#include <GL/freeglut.h>

#include "error.hpp"
#include "application.hpp"

std::unique_ptr<br::app::Application> main_app;

const std::size_t WindowWidth                       ( 800u );
const std::size_t WindowHeight                      ( 600u );
   
/*
 * These constants are used to control how many update are made during idle time
 */
const std::size_t TimeFrame                         ( 1000u ); //milliseconds
const std::size_t FunctionCallsPerTimeFrameThreshold( 30u   ); 

void idle_callback( )
{
    if( main_app )
    {
        //Measure time since last call to idle, 
        //We restrict the number of call per time frame to doing unecessary job 
        //which can result into heating the system
        static std::size_t function_call_counter = 0u;
        static std::size_t accumulated_time      = 0u;
        static std::size_t previous_tick         = 0u;

        const auto current_tick = glutGet(GLUT_ELAPSED_TIME);

        accumulated_time += (current_tick - previous_tick);
        previous_tick = current_tick;
        
        //We need to reset the time accumulator, in order to enable function calls for the next time frame
        if( accumulated_time > TimeFrame )
        {
            //Guard if a lot of time has passed since last call, but retain remainder
            accumulated_time      = accumulated_time % TimeFrame;
            function_call_counter = 0u;
        }

        //Determine if we should dispatch an update
        if( function_call_counter < FunctionCallsPerTimeFrameThreshold )
        {
            main_app->processSceneTasks( );
            ++function_call_counter;
        }
    }
}

void display_callback( )
{
    if( main_app )
    {
        main_app->renderScene( );
    }
}

void reshape_callback ( int w, int h )
{       
    if( main_app )
    {
        main_app->setViewportDimension( w, h );
    }
}

int main(int argc, char* argv[])
{     
    //Free glut
    {
        glutInit(&argc, argv);
        //Context
        //ERROR: Doesn't work on my linux, using open-source driver...
        //glutInitContextVersion( 3, 3 );
        //glutInitContextProfile( GLUT_CORE_PROFILE );
        //glutInitContextFlags  ( GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG );
        //Window
        const auto screen_width = glutGet( GLUT_SCREEN_WIDTH );
        const auto screen_height = glutGet( GLUT_SCREEN_HEIGHT );
        glutInitWindowPosition ( (screen_width / 2) - (WindowWidth / 2), (screen_height / 2) - (WindowHeight / 2));
        glutInitWindowSize     (WindowWidth, WindowHeight);
        glutInitDisplayString(
            " rgba"    //Color model              
            " single"  //Single buffered
            " red~8"   //Red buffer >=8bits
            " green~8" //Green buffer >=8bits
            " blue~8"  //Blue buffer >=8bits
            " alpha~8" //Alpha buffer >=8bits
            );
        glutCreateWindow( "Box renderer" );
    }
    //Glew
    {
        glewExperimental = GL_TRUE;
        const auto init_status = glewInit( );
        if( init_status != GLEW_OK )
        {
            std::string error_description( reinterpret_cast<const char*>( glewGetErrorString( init_status ) ) );
            std::cerr << "Glew initialization failed: " << error_description << std::endl;
            return static_cast<int>( br::error::Application::GLEW_INITIALIZATION );
        }
    }

    //Information at startup
    {
        const auto glut_version = glutGet( GLUT_VERSION );
        const auto vmajor = glut_version / 10000;
        const auto vminor = (glut_version % 10000) / 100;
        const auto vpatch = (glut_version % 10000) % 100;
        const auto glew_version = std::string( reinterpret_cast<const char*>( glewGetString( GLEW_VERSION ) ) );

        std::cout << "Freeglut version: " << vmajor << "." << vminor << "-" << vpatch << std::endl;  
        std::cout << "GLEW version    : " << glew_version << std::endl;

        const std::function<GLboolean(void)> OpenGLVersionQuery[] = {
        		[] ( ) -> GLboolean { return GLEW_VERSION_4_1; },
        		[] ( ) -> GLboolean { return GLEW_VERSION_4_0; },
        		[] ( ) -> GLboolean { return GLEW_VERSION_3_3; },
        		[] ( ) -> GLboolean { return GLEW_VERSION_3_2; },
        		[] ( ) -> GLboolean { return GLEW_VERSION_3_1; },
        		[] ( ) -> GLboolean { return GLEW_VERSION_3_0; },
        		[] ( ) -> GLboolean { return GLEW_VERSION_2_1; },
        		[] ( ) -> GLboolean { return GLEW_VERSION_2_0; },
        		[] ( ) -> GLboolean { return GLEW_VERSION_1_5; },
        		[] ( ) -> GLboolean { return GLEW_VERSION_1_4; },
        		[] ( ) -> GLboolean { return GLEW_VERSION_1_3; },
        		[] ( ) -> GLboolean { return GLEW_VERSION_1_2_1; },
        		[] ( ) -> GLboolean { return GLEW_VERSION_1_2; },
        		[] ( ) -> GLboolean { return GLEW_VERSION_1_1; },
        };

		std::cout << "OpenGL API compatible: " << std::endl;
        const std::size_t openGLVersionCount   = sizeof(OpenGLVersionQuery) / sizeof(OpenGLVersionQuery[0]);
        for(std::size_t openGLVersionChecker = 0; openGLVersionChecker < openGLVersionCount; ++openGLVersionChecker)
        {
        	if( const GLboolean version_identifier = OpenGLVersionQuery[openGLVersionChecker]( ) )
        	{
        		std::string version_title;
        		switch(openGLVersionChecker)
        		{
        		case  0: version_title = "4.1"; break;
        		case  1: version_title = "4.0"; break;
        		case  2: version_title = "3.3"; break;
        		case  3: version_title = "3.2"; break;
        		case  4: version_title = "3.1"; break;
        		case  5: version_title = "3.0"; break;
        		case  6: version_title = "2.1"; break;
        		case  7: version_title = "2.0"; break;
        		case  8: version_title = "1.5"; break;
        		case  9: version_title = "1.4"; break;
        		case 10: version_title = "1.3"; break;
        		case 11: version_title = "1.2.1"; break;
        		case 12: version_title = "1.2"; break;
        		case 13: version_title = "1.1"; break;
        		}
        		std::cout << "\t-" << version_title << std::endl;
        	}
        }
    }

    //Setup global application 
    main_app = std::unique_ptr<br::app::Application>( new br::app::Application(800, 600) );

    //Setup Callback
    glutIdleFunc    ( idle_callback );
    glutDisplayFunc ( display_callback );
    glutReshapeFunc ( reshape_callback );

    //Reset timer just before main loop (ignore elapsed time during initialization)
    glutGet(GLUT_ELAPSED_TIME);
    glutMainLoop( );

    return 0;
}
