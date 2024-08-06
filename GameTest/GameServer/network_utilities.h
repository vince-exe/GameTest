#pragma once

#include <iostream>
#include <boost/asio.hpp>

#include "NetPacket.h"
#include "udp_utilities.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

namespace NetUtils {
    namespace Tcp {
        NetPacket read_(tcp::socket& socket);

        void write_(tcp::socket& socket, const NetPacket& packet);
    }
    namespace Udp {
        NetPacket read_(udp::socket& socket, udp::endpoint& endpoint);

        void write_(udp::socket& socket, const udp::endpoint& endpoint, const NetPacket& packet);
    }
}