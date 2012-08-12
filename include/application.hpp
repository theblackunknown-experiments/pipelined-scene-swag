#ifndef BR_APPLICATION_H
#define BR_APPLICATION_H

#include <utility>

#include <bpipe/parameter/parameter_database.hpp>
#include <bpipe/scheduler/scheduler.hpp>

namespace br {
    namespace app {

        class Application
        {
        public:
            explicit Application( std::size_t width, std::size_t height );

            void setViewportDimension( std::size_t width, std::size_t height );

            void processSceneTasks ( );
            void renderScene       ( );

        private:
            std::pair<std::size_t, std::size_t>           viewDimension;

            bpipe::ParameterDatabase                      data;
            bpipe::Scheduler::CollectionSharedPointerStep update_steps;
            bpipe::Scheduler::CollectionSharedPointerStep render_steps;

            bpipe::Scheduler                              scheduler;
        };        

    }
}

#endif // !BR_APPLICATION_H

