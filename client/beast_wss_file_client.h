#ifndef _BEAST_WSS_FILE_CLIENT_H_INCLUDED_
#define _BEAST_WSS_FILE_CLIENT_H_INCLUDED_

#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/beast.hpp>

#include "ssl_teardown.h"

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;

using tcp = boost::asio::ip::tcp;
using std::string;
using std::vector;
using std::ofstream;

class WssFileClient
{
private:
    net::io_context net_context;
    ssl::context ssl_context;
    tcp::resolver resolver;
    websocket::stream<ssl::stream<tcp::socket>> ws;
    beast::flat_buffer net_buffer;
    string host;
    uint16_t port;
    string file_name;
    vector<char> file_buffer;
    size_t file_size;
    size_t received_size;
    ofstream file;

public:
    WssFileClient(const char* ip, uint16_t port, const char* file_name);

    int download_file();
};

#endif /* _BEAST_WSS_FILE_CLIENT_H_INCLUDED_ */
