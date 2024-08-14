#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace std {
    template <>
    struct hash<boost::uuids::uuid> {
        std::size_t operator()(const boost::uuids::uuid& uuid) const noexcept {
            return boost::uuids::hash_value(uuid);
        }
    };
}