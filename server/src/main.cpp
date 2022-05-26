#include <string>

#include <spdlog/spdlog.h>

#include "application.h"

int main(int argc, char ** argv)
{
    try {
        int port = 55123;
        int save_period_sec = 5;

        if (argc == 2) {
            port = std::stoi(argv[1]);
        }

        if (argc == 3) {
            save_period_sec = std::stoi(argv[2]);
        }

        average::Application app(port, save_period_sec);
        return app.run();
    }
    catch (std::exception &e) {
        spdlog::error("Exception: {}", e.what());
    }

    return 1;
}
