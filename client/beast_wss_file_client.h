#ifndef _BEAST_WSS_FILE_CLIENT_H_INCLUDED_
#define _BEAST_WSS_FILE_CLIENT_H_INCLUDED_

#include <string>
#include <vector>
#include <cstdint>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;

using tcp = boost::asio::ip::tcp;
using std::string;
using std::string_view;
using std::vector;

class WssFileClient
{
private:
    bool connected;
    net::io_context net_context;
    ssl::context ssl_context;
    websocket::stream<beast::ssl_stream<beast::tcp_stream>> ws;
    string host;
    uint16_t port;

public:
    WssFileClient(const char* host, uint16_t port, const char* cert_file);
    ~WssFileClient();

    int connect();
    int download_file(string_view file_name);
    int disconnect();
};

#endif /* _BEAST_WSS_FILE_CLIENT_H_INCLUDED_ */
