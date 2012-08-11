#ifndef BR_APPLICATION_H
#define BR_APPLICATION_H

namespace br {
    namespace app {

        class Application
        {
        public:
            explicit Application( int width, int height );

            void setViewportDimension( int width, int height );

            void processSceneTasks ( );
            void renderScene       ( );
        };        

    }
}

#endif // !BR_APPLICATION_H

