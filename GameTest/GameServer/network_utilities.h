#pragma once

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace NetUtils {
    const std::string MSG_DELIMETER = "\n";
        
    std::string read(tcp::socket& socket, const std::string& until);
  
    void send(tcp::socket& socket, const std::string& message, const std::string& delimeter);
}