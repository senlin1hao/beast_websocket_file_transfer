#ifndef _BEAST_WSS_FILE_SERVER_H_INCLUDED_
#define _BEAST_WSS_FILE_SERVER_H_INCLUDED_

#include <string>
#include <vector>
#include <fstream>
#include <cstdint>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>

#include "ssl_teardown.h"

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;

using tcp = boost::asio::ip::tcp;
using std::ifstream;
using std::string;
using std::thread;
using std::vector;

class WssFileServerSession : public std::enable_shared_from_this<WssFileServerSession>
{
private:
    websocket::stream<ssl::stream<tcp::socket>> ws;
    beast::flat_buffer net_buffer;
    string file_name;
    ifstream file;
    vector<char> file_buffer;

    void on_ssl_handshake();
    void on_websocket_accept();
    void on_read_request();
    bool is_save_path(const string& path);
    void send_file();
    void send_next_block(size_t file_size, size_t sent_size);
    void send_file_end();
    void session_close();

public:
    WssFileServerSession(tcp::socket&& socket, ssl::context& ctx);

    void run();
};

class WssFileServer
{
private:
    bool running;
    size_t thread_num;
    net::io_context net_context;
    tcp::endpoint endpoint;
    ssl::context ssl_context;
    tcp::acceptor acceptor;

    void run();

public:
    WssFileServer(const char* ip, uint16_t port, size_t thread_num, const char* cert_file, const char* cert_key_file);

    void start();
};

#endif /* _BEAST_WSS_FILE_SERVER_H_INCLUDED_ */
