//Copyright 2022 by Winter Solider

#include <Crawler.h>

int loading_html() {
  try
  {
    auto const host = "www.boost.org";
    auto const port = "443";
    auto const target = "/doc/libs/1_69_0/libs/beast/example/http/client/"
        "sync-ssl/http_client_sync_ssl.cpp"; //
    int version = 11;

    boost::asio::io_context ioc;

    ssl::context ctx{ ssl::context::sslv23_client };

    load_root_certificates(ctx);

    boost::asio::ip::tcp::resolver resolver{ ioc };
    ssl::stream<boost::asio::ip::tcp::socket> stream{ ioc, ctx };

    if (!SSL_set_tlsext_host_name(stream.native_handle(), host))
    {
      boost::system::error_code ec{ static_cast<int>(::ERR_get_error()),
                                   boost::asio::error::get_ssl_category() };
      throw boost::system::system_error{ ec };
    }

    auto const results = resolver.resolve(host, port);

    boost::asio::connect(stream.next_layer(), results.begin(),
                         results.end());

    stream.handshake(ssl::stream_base::client);

    boost::beast::http::request<boost::beast::http::string_body> req{
        boost::beast::http::verb::get, target,
        version };
    req.set(boost::beast::http::field::host, host);
    req.set(boost::beast::http::field::user_agent,
            BOOST_BEAST_VERSION_STRING);

    boost::beast::http::write(stream, req);

    boost::beast::flat_buffer buffer;

    boost::beast::http::response<boost::beast::http::dynamic_body> res;

    boost::beast::http::read(stream, buffer, res);

    std::ofstream ofs{"out.txt"};
    ofs << res;
    ofs.close();


    boost::system::error_code ec;
    stream.shutdown(ec);
    if (ec == boost::asio::error::eof)
    {
      ec.assign(0, ec.category());
    }
    if (ec)
      throw boost::system::system_error{ ec };
  }
  catch (std::exception const& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int main(){
  loading_html();
}
