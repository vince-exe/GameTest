#pragma once

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace NetUtils {
    const std::string MSG_DELIMETER = "\n";
        
    std::string read(tcp::socket& socket, const std::string& until);
  
    void send(tcp::socket& socket, const std::string& message, const std::string& delimeter);
}

namespace NetMessages {
    const std::string SERVER_FULL = "[!Server_Full!]";

    const std::string NICK_ALREADY_EXIST = "[!Nick_Exist!]";

    const std::string CLIENT_ACCEPTED = "[!Client_Accept!]";

    const std::string MATCHMAKING_REQUEST = "[!Match_Request!]";

    const std::string WAIT_FOR_MATCH = "[!Wait_Match!]";

    const std::string UNDO_MATCHMAKING = "[!Undo_Match!]";
}