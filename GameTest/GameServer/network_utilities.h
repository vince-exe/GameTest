#pragma once

#include <iostream>
#include <boost/asio.hpp>

#include "NetPacket.h"

using boost::asio::ip::tcp;

namespace NetUtils {        
    NetPacket read_(tcp::socket& socket);

    bool write_(tcp::socket& socket, const NetPacket& packet);
}

