#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include <boost/asio.hpp>

namespace average {

class Client final
{
public:
    using io_context = boost::asio::io_context;
    using tcp_socket = boost::asio::ip::tcp::socket;

    explicit Client(io_context &context);
    void connect(const std::string &ip, int port);
    void write_random(int value);
    double read_average();

protected:
    tcp_socket _socket;
};

} // average

#endif // CLIENT_H
