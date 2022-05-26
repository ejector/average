#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include <boost/asio.hpp>

namespace average {

class Client
{
public:
    using io_context = boost::asio::io_context;
    using tcp_socket = boost::asio::ip::tcp::socket;

    Client(io_context &context);
    void connect(const std::string &ip, int port);
    void write(int random);
    double read();

protected:
    io_context &_context;
    tcp_socket _socket;
};

} // average

#endif // CLIENT_H
