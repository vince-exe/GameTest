#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <cstring>

namespace GameSessionUtils {
	std::vector<uint8_t> convertCoordinatesToBytes(const std::vector<std::vector<std::pair<float, float>>>& data);
}