#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <exception>

#include <vector>
#include <memory>

#include <boost/asio.hpp>
#include <boost/interprocess/detail/os_thread_functions.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "randomizer.h"
#include "client/client.h"

namespace average {

class Application
{
public:
    Application(std::string &ip, int port)
        : _ip(ip)
        , _port(port)
    {

    }
    void init_logger()
    {
        auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

        std::string file_name = fmt::format("client_{}.log", boost::interprocess::ipcdetail::get_current_process_id());

        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(file_name);
        std::vector<spdlog::sink_ptr> sinks = {stdout_sink, file_sink};
        auto logger = std::make_shared<spdlog::logger>("logger", std::begin(sinks), std::end(sinks));
        spdlog::set_default_logger(logger);
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
            client.connect(_ip, _port);

            Randomizer randomizer(0, 1023);

            while (!_exit) {
                client.write_random(randomizer.random());
                spdlog::info("average: {}", client.read_average());
            }

        } catch (std::exception & e) {
            spdlog::error("Exception: {}", e.what());
        }

        return 0;
    }
protected:
    std::string _ip;
    int _port = 0;
};

} // namespace average

#endif // APPLICATION_H
