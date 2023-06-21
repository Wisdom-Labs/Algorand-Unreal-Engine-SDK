/**
 * Self-describing hashes
 *
 * @file multihash.hpp
 * @author Matthew Knight
 * @date 2019-10-11
 */

#pragma once

#include "varint.hpp"

#include <vector>

#include <cstdint>

namespace Multiformats {
    /** @brief Object for containing specific hash */
    class Multihash {
        std::vector<std::uint8_t> buf;

        using ConstIterator = typename decltype(buf)::const_iterator;

      public:
        Multihash() = default;

        /** @brief Hash plaintext using explicit fuction code */
        Multihash(std::vector<std::uint8_t> const& plaintext,
                  Varint const& protocol);

        /** @brief Hash plaintext using string name */
        Multihash(std::vector<std::uint8_t> const& plaintext,
                  std::string const& protocol);

        /** @brief Construct multihash from sequence */
        template <typename Iterator>
        Multihash(Iterator begin, Iterator end) {
            std::copy(begin, end, std::back_inserter(buf));
        }

        /** @brief Extract function code from multihash */
        Varint func_code() const;

        /** @brief Extract digest length from multihash */
        Varint len() const;

        /** @brief Get size of entire multihash */
        std::size_t size() const;

        /** @brief Const iterator to begining of multihash */
        ConstIterator begin() const;

        /** @brief Const iterator to beginning of digest withing multihash */
        ConstIterator digest() const;

        /** @brief Const iterator to end of multihash */
        ConstIterator end() const;
    };
} // namespace Multiformats
