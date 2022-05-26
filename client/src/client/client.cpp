#include "client.h"

#include <boost/asio.hpp>

using namespace boost::asio;

namespace average {

Client::Client(io_context &context)
    : _context(context)
    , _socket(context)
{
}

void Client::connect(const std::string &ip, int port)
{
    _socket.connect(ip::tcp::endpoint(ip::address::from_string(ip), port));
}

void Client::write(int data)
{
    boost::asio::write(_socket, boost::asio::buffer(&data, sizeof(data)));
}

double Client::read()
{
    double data;
    boost::asio::read(_socket, boost::asio::buffer(&data, sizeof(data)));
    return data;
}

}
