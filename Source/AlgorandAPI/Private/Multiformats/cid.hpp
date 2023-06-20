/**
 * Content IDentifier: CID
 *
 * @file cid.hpp
 * @author Matthew Knight
 * @date 2019-10-16
 */

#pragma once

#include "multibase.hpp"
#include "multihash.hpp"
#include "varint.hpp"

namespace Multiformats {
    class Cid {
        Varint version;
        Varint content_type;
        Multihash content_address;

      public:
        /** @brief Construct from multibase encoded string */
        Cid(std::string const& encoded);

        /** @brief Construct from CID binary */
        Cid(std::vector<std::uint8_t> const& buf);

        /** @brief Construct from parts */
        Cid(Varint const& version, Varint const& content_type,
            Multihash const& content_address);

        /** @brief encode to multibase */
        std::string to_string(Multibase::Protocol const& protocol);

        /** @brief to human readable format */
        std::string human_readable(Multibase::Protocol const& protocol);
    };
}; // namespace Multiformats
