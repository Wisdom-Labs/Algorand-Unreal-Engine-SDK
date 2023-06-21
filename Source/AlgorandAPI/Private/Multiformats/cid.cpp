/**
 * Content IDentifier: CID
 *
 * @file cid.cpp
 * @author Matthew Knight
 * @date 2019-10-16
 */

#include "cid.hpp"

#include "multibase.hpp"
#include "multicodec.hpp"

#include <algorithm>
#include <sstream>
#include <array>

#include <iostream>

namespace Multiformats {
    Cid::Cid(std::string const& encoded)
        : Cid(Multibase::decode(encoded)) {}

    Cid::Cid(std::vector<std::uint8_t> const& buf) {
        static constexpr std::array cidv0_preamble{0x12, 0x20};


        bool has_preamble = std::equal(cidv0_preamble.cbegin(),
                                       cidv0_preamble.cend(), buf.cbegin());
        // check for old, CIDv0
        if (has_preamble && buf.size() == 34) {
            version = 0;
            content_type = 0x70;
            content_address = Multihash(buf.cbegin(), buf.cend());
        } else {
            auto it = buf.cbegin();
            version = Varint{it, buf.cend()};
            it += version.size();

            content_type = Varint{it, buf.cend()};
            it += content_type.size();
            content_address = Multihash(it, buf.cend());
        }
    }

    Cid::Cid(Varint const& version, Varint const& content_type,
             Multihash const& content_address)
        : version(version)
        , content_type(content_type)
        , content_address(content_address) {}

    std::string Cid::to_string(Multibase::Protocol const& protocol) {
        std::vector<std::uint8_t> buf;
        if (version != 0) {
            std::copy(version.begin(), version.end(), std::back_inserter(buf));
            std::copy(content_type.begin(), content_type.end(),
                      std::back_inserter(buf));
        } else if (protocol != Multibase::Protocol::Base58Btc) {
            // CIDv0 must use base58btc
            throw std::invalid_argument("CIDv0 must use base58btc");
        }

        std::copy(content_address.begin(), content_address.end(),
                  std::back_inserter(buf));
        return Multibase::encode(protocol, buf);
    }

    std::string Cid::human_readable(Multibase::Protocol const& protocol) {
        static constexpr auto separator = " - ";
        static constexpr auto get_key = [](auto const& map,
                                           auto const& value) -> std::string {
            auto it = std::find_if(
                map.cbegin(), map.cend(),
                [&value](auto const& elem) { return value == elem.second; });
            return it == map.cend() ? "unknown" : it->first;
        };

        std::string codec = get_key(Multicodec::table, content_type);
        std::string func_str =
            get_key(Multicodec::table, content_address.func_code());
        std::uint64_t bits = content_address.len() * 8;
        std::vector<std::uint8_t> tmp{content_address.digest(),
                                      content_address.end()};
        std::string digest =
            Multibase::encode(Multibase::Protocol::Base16, tmp);

        std::stringstream ss;
        ss << Multibase::to_string(protocol) << " - cidv" << version << " - "
           << codec << " - " << func_str << "-" << bits << "-"
           << digest.substr(1);

        return ss.str();
    }
} // namespace Multiformats
