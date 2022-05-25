#include <iostream>
#include <exception>
#include <random>
#include <vector>
#include <memory>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "client/client.h"

namespace average {

class Randomizer
{
public:
    Randomizer(int min, int max)
        : _generator(_random_device())
        , _distrib(min, max)
    {

    }
    int random()
    {
        return _distrib(_generator);
    }
protected:
    std::random_device _random_device;
    std::mt19937 _generator;
    std::uniform_int_distribution<> _distrib;
};

class Application
{
public:
    void init_logger()
    {
        auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logfile.txt");
        std::vector<spdlog::sink_ptr> sinks = {stdout_sink, file_sink};
        auto logger = std::make_shared<spdlog::logger>("logger", std::begin(sinks), std::end(sinks));
        spdlog::register_logger(logger);
    }
    int run()
    {
        try {

            init_logger();

            boost::asio::io_context io_context;

            boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);

            bool _exit = false;
            auto exit_handler = [&_exit](boost::system::error_code, size_t) {
                _exit = true;
            };

            signals.async_wait(exit_handler);

            Client client(io_context);
            client.connect("127.0.0.0", 55123);

            Randomizer randomizer(0, 1023);

            while (!_exit) {
                client.write(randomizer.random());
                spdlog::info("average: {}", client.read());
            }

        } catch (std::exception & e) {
            spdlog::error("Exception: {}", e.what());
        }

        return 0;
    }
};

} // namespace average

int main()
{
    average::Application app;
    return app.run();
}
