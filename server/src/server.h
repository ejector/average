#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include <set>
#include <future>

#include <spdlog/spdlog.h>
#include <boost/asio.hpp>

#include "randomstorage.h"

namespace average {

class Session : public std::enable_shared_from_this<Session>
{
public:
    using RandomStorageSetInt = RandomStorage<std::set<int>>;
    using tcp_socket = boost::asio::ip::tcp::socket;
    Session(RandomStorageSetInt& random_storage, tcp_socket socket)
        : _random_storage(random_storage)
        , _socket(std::move(socket))
    {
    }

    void start()
    {
        read();
    }

private:
    void read()
    {
        auto self(shared_from_this());
        _socket.async_read_some(boost::asio::buffer(&_random, sizeof(_random)),
                                [this, self](boost::system::error_code ec, std::size_t)
        {
            if (ec) {
                spdlog::error("Read error: {}", ec.message());
                return;
            }

            _async = std::async(std::launch::async, [this, self, random = _random]() {
                _random_storage.insert(_random);
                _average = _random_storage.average();
                boost::asio::post([this, self, average = _average](){
                    write(average);
                });
            });

        });
    }

    void write(double average)
    {
        auto self(shared_from_this());
        _average = average;
        boost::asio::async_write(_socket, boost::asio::buffer(&_average, sizeof(_average)),
                                 [this, self](boost::system::error_code ec, std::size_t)
        {
            if (ec) {
                spdlog::error("Write error: {}", ec.message());
                return;
            }
            read();
        });
    }
private:
    RandomStorageSetInt& _random_storage;
    tcp_socket _socket;
    std::future<void> _async;
    int _random = 0;
    double _average = 0.0;
};

class Server
{
public:
    using RandomStorageSetInt = RandomStorage<std::set<int>>;
    using tcp_endpoint = boost::asio::ip::tcp::endpoint;
    using tcp_socket =  boost::asio::ip::tcp::socket;
    using tcp_acceptot = boost::asio::ip::tcp::acceptor;
    Server(RandomStorageSetInt& random_storage, boost::asio::io_context& io_context, int port)
        : _random_storage(random_storage)
        , _acceptor(io_context, tcp_endpoint(boost::asio::ip::tcp::v4(), port))
    {
        accept();
    }

private:
    void accept()
    {
        _acceptor.async_accept(
                    [this](boost::system::error_code ec, tcp_socket socket)
        {
            if (!ec)
            {
                std::make_shared<Session>(_random_storage, std::move(socket))->start();
            }

            accept();
        });
    }
private:
    RandomStorageSetInt& _random_storage;
    tcp_acceptot  _acceptor;
};

} // namespace average

#endif // SERVER_H
