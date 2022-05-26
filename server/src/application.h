#ifndef APPLICATION_H
#define APPLICATION_H

#include <set>
#include <fstream>

#include <boost/asio.hpp>
#include <boost/interprocess/detail/os_thread_functions.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "randomstorage.h"
#include "server.h"

namespace average {

class Application
{
public:
    Application(int port, int save_period_sec)
        : _port(port)
        , _save_period_sec(save_period_sec)
        , _signals(_io_context, SIGINT, SIGTERM)
        , _save_timer(_io_context, save_period_sec)
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
    void init_exit_handler()
    {
        auto exit_handler = [&](boost::system::error_code, size_t) {
            _io_context.stop();
        };
        _signals.async_wait(exit_handler);
    }
    void start_save_data_timer()
    {
        _save_timer = boost::asio::deadline_timer(_io_context, boost::posix_time::seconds(_save_period_sec));
        _save_timer.async_wait([this](const boost::system::error_code& ec) {
            if (ec) {
                spdlog::error("Timer error: {}", ec.message());
                return;
            }
            static std::future<void> async;
            async = std::async(std::launch::async, [this, set = _random_storage.copy_data()]() {
                std::ofstream file("random_set.bin", std::ios::binary|std::ios::trunc);
                for (auto&& item: set) file << item;
                spdlog::info("Saved data to file");
                boost::asio::post([this](){
                    start_save_data_timer();
                });
            });
        });
    }
    int run()
    {
        try {

            init_logger();
            init_exit_handler();
            start_save_data_timer();

            Server server(_random_storage, _io_context, _port);

            _io_context.run();

        } catch (std::exception & e) {
            spdlog::error("Exception: {}", e.what());
        }

        return 0;
    }
protected:
    int _port = 0;
    int _save_period_sec = 0;
    boost::asio::io_context _io_context;
    boost::asio::signal_set _signals;
    boost::asio::deadline_timer _save_timer;
    RandomStorage<std::set<int>> _random_storage;
};

} // namespace average

#endif // APPLICATION_H
