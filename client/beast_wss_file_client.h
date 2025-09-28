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
#include <spdlog/spdlog.h>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;

using tcp = boost::asio::ip::tcp;
using std::string;
using std::string_view;
using std::vector;
using std::shared_ptr;

namespace wss_file_client
{
    constexpr size_t MAX_LOG_SIZE = 5 * 1024 * 1024;
    constexpr size_t MAX_LOG_COUNT = 3;
}

class WssFileClient
{
private:
    bool connected;
    net::io_context net_context;
    ssl::context ssl_context;
    websocket::stream<beast::ssl_stream<beast::tcp_stream>> ws;
    string host;
    uint16_t port;
    static shared_ptr<spdlog::logger> logger;

public:
    WssFileClient(const char* host, uint16_t port, const char* cert_file);
    ~WssFileClient();

    int connect();
    int download_file(string_view file_name);
    int disconnect();
};

#endif /* _BEAST_WSS_FILE_CLIENT_H_INCLUDED_ */
