#include "client.h"

using namespace boost::asio;

namespace average {

Client::Client(io_context &context)
    : _socket(context)
{
}

void Client::connect(const std::string &ip, int port)
{
    _socket.connect(ip::tcp::endpoint(ip::address::from_string(ip), port));
}

void Client::write_random(int value)
{
    boost::asio::write(_socket, boost::asio::buffer(&value, sizeof(value)));
}

double Client::read_average()
{
    double data;
    boost::asio::read(_socket, boost::asio::buffer(&data, sizeof(data)));
    return data;
}

}
