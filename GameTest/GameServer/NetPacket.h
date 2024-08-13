#pragma once

#include <iostream>
#include <vector>

class NetPacket  {
public:
	enum class NetMessages {
		UDP_CONNECTION_SUCCESS,
		CONNECTION_UDP_MESSAGE,
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
		ENEMY_COLLISION,
		QUIT_GAME,
		GAME_STARTED,
		ENEMY_COLLISION_W_DAMAGE_AREA,
		GAME_END
	};

private:
	NetMessages m_messageType;
	std::vector<uint8_t> m_Data;
	size_t m_dataSize;

public:
	static NetPacket deserialize(const std::vector<uint8_t>& serializedData);

public:
	NetPacket() = default;

	NetPacket(NetMessages type, const uint8_t* data_, size_t dataSize_);

	NetMessages getMsgType() const;

	const std::vector<uint8_t>& getData() const;
	
	size_t getDataSize();

	std::vector<uint8_t> serialize() const;
};