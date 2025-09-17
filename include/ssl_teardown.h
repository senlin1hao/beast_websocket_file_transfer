#ifndef _SSL_TEARDOWN_H_INCLUDED_
#define _SSL_TEARDOWN_H_INCLUDED_

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>

namespace ssl = boost::asio::ssl;
namespace beast = boost::beast;
using tcp = boost::asio::ip::tcp;

namespace boost::beast::websocket
{
    template <>
    inline void teardown(
        role_type,
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& stream,
        boost::beast::error_code& ec)
    {
        stream.shutdown(ec);
        auto& sock = stream.next_layer();
        if (sock.is_open())
        {
            sock.close(ec);
        }
    }

    template <typename TeardownHandler>
    inline void async_teardown(
        role_type,
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& stream,
        TeardownHandler&& handler)
    {
        stream.async_shutdown([&stream, handler = std::forward<TeardownHandler>(handler)](boost::beast::error_code ec) mutable {
            auto& sock = stream.next_layer();
            if (sock.is_open())
            {
                boost::beast::error_code ignore_ec;
                sock.close(ignore_ec);
            }
            handler(ec);
        });
    }
}  // namespace boost::beast::websocket

#endif /*  _SSL_TEARDOWN_H_INCLUDED_ */
