/**
 * Unsigned variable-length integer
 *
 * @file varint.hpp
 * @author Matthew Knight
 * @date 2019-09-12
 */

#pragma once

#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <limits>

#include <cstdint>
namespace Multiformats {
    class Varint {
        static constexpr auto max_bits = 63;
        static constexpr unsigned long long max = 0xefffffffffffffff;

        std::vector<std::uint8_t> buf{};

      public:
        /** @brief Default constructor
         *
         *  The default value of this class is zero
         */
        Varint() : buf(1, 0) {}

        /** @brief Construct from an integral type
         *
         *  @param integral Some integral value
         *  @throw std::invalid_argument if the value of the integral is
         * negative or greater than 2^63 - 1 */
        template <typename Integral,
                  typename = std::enable_if_t<std::is_integral_v<Integral>>>
        Varint(Integral integral) {
            if constexpr (std::numeric_limits<Integral>::digits > max_bits)
                if (integral > max)
                    throw std::invalid_argument("integral value is too large");

            if (integral < 0)
                throw std::invalid_argument(
                    "Varint cannot represent negative numbers");

            do {
                std::uint8_t value = integral & 0x7f;
                integral = integral >> 7;

                if (integral)
                    value |= 0x80;

                buf.push_back(value);
            } while (integral);

            if (buf.size() > 1) {
                std::for_each(buf.begin(), std::prev(buf.end()),
                              [](auto& elem) {
                                  // set msb
                                  elem |= 0x80;
                              });
            }
        }

        /** @brief Construct from sequence
         *
         *  @param begin Iterator to beginning of sequence
         *  @param end Iterator to end of sequence
         *  @throw std::invalid_argument If the number is too large, or the
         * sequence cannot be parsed */
        template <typename Iterator,
                  typename = std::enable_if_t<
                      sizeof(typename Iterator::value_type) == 1 &&
                      std::is_integral_v<typename Iterator::value_type>>>
        Varint(Iterator begin, Iterator end) {
            if (std::all_of(begin, end, [](auto& elem) { return elem & 0x80; }))
                throw std::invalid_argument("parsing error");

            auto it = begin;
            while (*it & 0x80 && it != end)
                ++it;

            // increment past end byte
            ++it;

            if (std::distance(begin, it) > 9)
                throw std::invalid_argument("number is too large");

            std::copy(begin, it, std::back_inserter(buf));
        }

        /** @brief Get size of the varint */
        auto size() const { return buf.size(); }

        /** @brief Get iterator to beginning of underlying buffer */
        auto begin() const { return buf.cbegin(); }

        /** @brief Get iterator to end of underlying buffer */
        auto end() const { return buf.cend(); }

        /** @brief Conversion to 64-bit unsigned.
         *
         * The current maximum value of the varint is 2^63 - 1, so a 64-bit uint
         * will be able to represent any value */
        operator std::uint64_t() const {
            std::uint64_t ret{};
            for (auto i = 0; i < size(); ++i)
                ret |= (buf[i] & 0x7f) << (7 * i);

            return ret;
        }
    };

    /**
     * @brief Extract Varint from sequence.
     *
     * @param begin Iterator to beginning of Varint
     * @param end Iterator to end of container
     * @return Deserialized Varint and past-the-end Iterator
     *
     * @throw std::runtime_error If Varint size is too big, or error in parsing
     */
    template <typename Iterator>
    std::tuple<Varint, Iterator> make_varint(Iterator begin, Iterator end) {
        Varint ret{begin, end};
        return {ret, std::next(begin, ret.size())};
    }
} // namespace Multiformats
