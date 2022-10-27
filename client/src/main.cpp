#include "application.h"

#include <cstdlib>

#include <string>
#include <exception>

#include <spdlog/spdlog.h>

int main(int argc, char ** argv)
{
    try {
        std::string ip = "127.0.0.1";
        int port = 55123;

        if (argc == 2) {
            ip = argv[1];
        }

        if (argc == 3) {
            port = std::stoi(argv[2]);
        }

        average::Application app(ip, port);
        return app.run();
    }
    catch (std::exception &e) {
        spdlog::error("Exception: {}", e.what());
    }

    return EXIT_FAILURE;
}
