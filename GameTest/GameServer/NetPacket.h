#pragma once

#include <iostream>
#include <vector>

enum NetMessages {
	SERVER_DOWN,
	IDLE,
	STANDARD_MESSAGE,
	SERVER_FULL,
	NICK_EXITS,
	CLIENT_ACCEPTED,
	MATCHMAKING_REQUEST,
	WAIT_FOR_MATCH,
	UNDO_MATCHMAKING,
	MATCH_FOUND,
	PLAYER_POSITION,
	QUIT_GAME
};

class NetPacket  {
public:
	NetPacket() = default;

	NetPacket(NetMessages type, const uint8_t* data, size_t dataSize);

	NetMessages getMsgType() const;

	const std::vector<uint8_t>& getData() const;
	
	size_t getDataSize();

	std::string getStr();

	std::vector<uint8_t> serialize() const;

	static NetPacket deserialize(const std::vector<uint8_t>& serializedData);

private:
	NetMessages messageType;
	std::vector<uint8_t> data;
	size_t dataSize;
};

