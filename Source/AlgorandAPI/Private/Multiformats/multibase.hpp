/**
 * Self-describing base encodings
 *
 * @file multibase.hpp
 * @author Matthew Knight
 * @date 2019-10-11
 */

#pragma once

#include <string>
#include <vector>

#include <cstdint>

namespace Multiformats::Multibase {
    /** @brief Base encodings */
    enum class Protocol {
        Identity,
        Base2,
        Base8,
        Base10,
        Base16,
        Base16Upper,
        Base32Hex,
        Base32HexUpper,
        Base32HexPad,
        Base32HexPadUpper,
        Base32,
        Base32Upper,
        Base32Pad,
        Base32PadUpper,
        Base32Z,
        Base58Flickr,
        Base58Btc,
        Base64,
        Base64Pad,
        Base64Url,
        Base64UrlPad
    };

    /** @brief Serialize protocol to string */
    std::string to_string(Protocol protocol);

    /** @brief Decode encoded string to binary */
    std::vector<std::uint8_t> decode(std::string const& str);

    /** @brief Encode raw binary to encoded string */
    std::string encode(Protocol protocol, std::vector<std::uint8_t> const& buf);

} // namespace Multiformats::Multibase
