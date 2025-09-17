#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include <boost/locale.hpp>

#include "beast_wss_file_client.h"

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;

using tcp = boost::asio::ip::tcp;
using std::ofstream;
using std::string;
using std::vector;

const char* DOWNLOAD_DIR = "./download";

WssFileClient::WssFileClient(const char* ip, uint16_t port, const char* file_name)
    : net_context(1), ssl_context(ssl::context::tls_client), resolver(net_context), ws(net_context, ssl_context),
      host(ip), port(port), file_name(file_name), file_size(0), received_size(0)
{
    ssl_context.set_verify_mode(ssl::verify_none);  // 自签证书，禁用验证
    ssl_context.set_options(ssl::context::default_workarounds | ssl::context::no_sslv2 | ssl::context::no_sslv3 | ssl::context::no_tlsv1 | ssl::context::no_tlsv1_1 | ssl::context::single_dh_use);
}

int WssFileClient::download_file()
{
    beast::error_code ec;

    const auto results = resolver.resolve(host, std::to_string(port));
    net::connect(ws.next_layer().next_layer(), results, ec);
    if (ec)
    {
        std::cerr << "connect error: " << boost::locale::conv::between(ec.message(), "UTF-8", "GBK") << std::endl;
        return -1;
    }

    SSL_set_tlsext_host_name(ws.next_layer().native_handle(), host.c_str());  // 设置SNI

    ws.next_layer().handshake(ssl::stream_base::client, ec);
    if (ec)
    {
        std::cerr << "handshake error: " << boost::locale::conv::between(ec.message(), "UTF-8", "GBK") << std::endl;
        return -1;
    }

    ws.handshake(host, "/", ec);
    if (ec)
    {
        std::cerr << "handshake error: " << boost::locale::conv::between(ec.message(), "UTF-8", "GBK") << std::endl;
        return -1;
    }

    string request = "FILE: " + file_name;
    ws.write(net::buffer(request));

    ws.read(net_buffer);
    string response = beast::buffers_to_string(net_buffer.data());
    net_buffer.consume(net_buffer.size());

    std::regex valid_response("FILE: (.*) SIZE: (\\d+)");
    if (!std::regex_match(response, valid_response))
    {
        std::cerr << "response error: " << response << std::endl;
        return -1;
    }

    string file_name_received = std::regex_replace(response, valid_response, "$1");
    if (file_name_received != file_name)
    {
        std::cerr << "file name error: " << file_name_received << std::endl;
        ws.close(websocket::close_code::normal);
        return -1;
    }
    file_size = std::stoul(std::regex_replace(response, valid_response, "$2"));

    std::filesystem::path file_path(DOWNLOAD_DIR);
    file_path.append(file_name);

    file.open(file_path.string(), std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "open file error: " << file_path << std::endl;
        return -1;
    }

    ws.binary(true);
    while (received_size < file_size)
    {
        ws.read(net_buffer);
        file.write(static_cast<const char*>(net_buffer.data().data()), net_buffer.size());
        received_size += net_buffer.size();
        net_buffer.consume(net_buffer.size());
    }

    ws.binary(false);
    ws.read(net_buffer);
    response = beast::buffers_to_string(net_buffer.data());
    net_buffer.consume(net_buffer.size());
    if (response != "FILE END")
    {
        std::cerr << "response error: " << response << std::endl;
        std::cout << "Raw response length: " << response.length() << std::endl;
        for (char c : response)
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (static_cast<int>(c) & 0xFF) << " ";
        }
        return -1;
    }

    ws.close(websocket::close_code::normal);
    file.close();

    std::cout << "download success: " << file_path << std::endl;

    return 0;
}
