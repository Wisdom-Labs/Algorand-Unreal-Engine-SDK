// Multibase -- self-describing base encodings
//
// Author: Matthew Knight
// File Name: multibase.cpp
// Date: 2019-09-12

#include "multibase.hpp"

#include <openssl/evp.h>

#include <algorithm>
#include <iomanip>
#include <array>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

#include <limits>
#include <cctype>
#include <cstring>

#include <iostream>

namespace {
    using namespace Multiformats::Multibase;

    template <typename T>
    constexpr T ceil_division(T numerator, T denominator) {
        return (numerator + denominator - 1) / denominator;
    }

    /**
     * Count consecutive values in a sequence, return count and end iterator of
     * zeros.
     */
    template <typename Iterator, typename Value>
    std::tuple<std::size_t, Iterator>
    count_consecutive(Iterator begin, Iterator end, Value value) {

        Iterator ret = begin;
        while (*ret == value && ret != end)
            ++ret;

        return {std::distance(begin, ret), ret};
    }

    Protocol get_protocol(char first) {
        switch (first) {
        case '\0':
            return Protocol::Identity;
        case '0':
            return Protocol::Base2;
        case '7':
            return Protocol::Base8;
        case '9':
            return Protocol::Base10;
        case 'f':
            return Protocol::Base16;
        case 'F':
            return Protocol::Base16Upper;
        case 'v':
            return Protocol::Base32Hex;
        case 'V':
            return Protocol::Base32HexUpper;
        case 't':
            return Protocol::Base32HexPad;
        case 'T':
            return Protocol::Base32HexPadUpper;
        case 'b':
            return Protocol::Base32;
        case 'B':
            return Protocol::Base32Upper;
        case 'c':
            return Protocol::Base32Pad;
        case 'C':
            return Protocol::Base32PadUpper;
        case 'h':
            return Protocol::Base32Z;
        case 'Z':
            return Protocol::Base58Flickr;
        case 'z':
        case '1':
        case 'Q':
            return Protocol::Base58Btc;
        case 'm':
            return Protocol::Base64;
        case 'M':
            return Protocol::Base64Pad;
        case 'u':
            return Protocol::Base64Url;
        case 'U':
            return Protocol::Base64UrlPad;
        }

        throw std::runtime_error("invalid protocol");
    }

    const std::array patterns{std::regex{"^.*$"},
                              std::regex{"^0[0-1]*$"},
                              std::regex{"^7[0-7]*$"},
                              std::regex{"^9[0-9]*$"},
                              std::regex{"^f[0-9a-f]*"},
                              std::regex{"^F[0-9A-F]*$"},
                              std::regex{"^v[0-9a-v]*$"},
                              std::regex{"^V[0-9A-V]*$"},
                              std::regex{"^t[0-9a-v=]*$"},
                              std::regex{"^T[0-9A-V=]*$"},
                              std::regex{"^b[2-7a-z]*$"},
                              std::regex{"^B[2-7A-Z]*$"},
                              std::regex{"^c[2-7a-z=]*$"},
                              std::regex{"^C[2-7A-Z=]*$"},
                              std::regex{"^h[13-7a-km-uw-z]*$"},
                              std::regex{"^Z[1-9A-HJ-Za-km-z]*$"},
                              std::regex{"^(z|1|Q)[1-9A-HJ-Za-km-z]*$"},
                              std::regex{"^m[0-9a-zA-Z+/]*$"},
                              std::regex{"^M[0-9a-zA-Z+/=]*$"},
                              std::regex{"^u[0-9a-zA-Z_-]*$"},
                              std::regex{"^U[0-9a-zA-Z_=-]*$"}
    };

    Protocol validate(std::string const& str) {
        if (str.empty())
            throw std::runtime_error("empty string");

        auto protocol = get_protocol(str.front());
        if (patterns.size() < static_cast<std::uint32_t>(protocol))
            throw std::runtime_error("unknown protocol");

        std::regex pattern = patterns[static_cast<std::uint32_t>(protocol)];

        if (!std::regex_match(str, pattern))
            throw std::runtime_error("invalid characters for protocol");

        return protocol;
    }

    /**
     * Encoders and Decoders
     *
     * Decoders only check the prefix to make sure it is correct, if they come
     * accross errenous coding, incorrect sizing, it will throw.
     *
     * The setup here is that each implementation of the encoder and decoder is
     * a template function specialization, so that we can easily generate a
     * table later
     */
    template <Protocol protocol>
    void encode(std::vector<std::uint8_t> const& input, std::string& output);

    template <Protocol protocol>
    void decode(std::string const& input, std::vector<std::uint8_t>& output);

    template <Protocol protocol>
    void encode_upper(std::vector<std::uint8_t> const& input,
                      std::string& output) {
        encode<protocol>(input, output);
        std::transform(output.begin(), output.end(), output.begin(),
                       [](auto elem) { return std::toupper(elem); });
    }

    template <Protocol protocol>
    void decode_upper(std::string const& input,
                      std::vector<std::uint8_t>& output) {
        std::string clone = input;
        std::transform(clone.begin(), clone.end(), clone.begin(),
                       [](auto elem) { return std::tolower(elem); });
        decode<protocol>(clone, output);
    }

    // Identity
    template <>
    void encode<Protocol::Identity>(std::vector<std::uint8_t> const& input,
                                    std::string& output) {
        output = std::string{input.cbegin(), input.cend()};
    }

    template <>
    void decode<Protocol::Identity>(std::string const& input,
                                    std::vector<std::uint8_t>& output) {
        output = std::vector<std::uint8_t>{input.cbegin(), input.cend()};
    }

    // Base2
    template <>
    void encode<Protocol::Base2>(std::vector<std::uint8_t> const& input,
                                 std::string& output) {
        output.reserve((8 * input.size()) + 1);
        auto inserter = std::back_inserter(output);
        inserter = '0';

        for (auto it = input.cbegin(); it != input.cend(); ++it)
            for (auto bit = 8; bit > 0; bit--)
                inserter = *it & (1 << (bit - 1)) ? '1' : '0';
    }

    template <>
    void decode<Protocol::Base2>(std::string const& input,
                                 std::vector<std::uint8_t>& output) {
        if ((input.size() - 1) % 8 != 0)
            throw std::runtime_error("Base2 encoding does not align to 8 bits");

        output.reserve((input.size() - 1) / 8);

        auto inserter = std::back_inserter(output);
        for (auto it = std::next(input.cbegin(), 1); it != input.cend();) {
            std::uint8_t value{0};
            for (auto bit = 8; bit > 0 && it != input.cend(); bit--) {
                if (*it == '1')
                    value |= 1 << (bit - 1);
                else if (*it != '0')
                    throw std::runtime_error("invalid character type");

                it++;
            }

            inserter = value;
        }
    }

    // Base8
    template <>
    void encode<Protocol::Base8>(std::vector<std::uint8_t> const& input,
                                 std::string& output) {
        std::array const characters{'0', '1', '2', '3', '4', '5', '6', '7'};
        std::uint8_t const mask{0x7};

        output.reserve(input.size());

        auto it = input.crbegin();
        auto end = input.crend();
        auto inserter = back_inserter(output);

        // find end of leading zero bytes
        while (end != input.crbegin() && *std::prev(end) == '0')
            --end;

        std::uint8_t overflow;
        std::uint8_t offset{0};
        for (; it != end; ++it) {
            if (offset > 0) {
                auto value =
                    ((*it & (mask >> (3 - offset))) << (3 - offset)) | overflow;
                inserter = characters[value];
            }

            for (; offset < 8; offset += 3) {
                auto value = (*it & (mask << offset)) >> offset;
                if (offset > 5)
                    overflow = value;
                else
                    inserter = characters[value];
            }

            offset &= mask;
        }

        if (((offset - 3) & mask) > 5 && overflow != 0)
            inserter = characters[overflow];

        fill_n(inserter, std::distance(end, std::prev(input.crend())), '0');
        inserter = '7';
        std::reverse(output.begin(), output.end());
    }

    template <>
    void decode<Protocol::Base8>(std::string const& input,
                                 std::vector<std::uint8_t>& output) {

        std::uint8_t const mask{0x7};
        std::size_t leading_zeros{0};
        auto convert = [](auto num) { return num - 48; };

        if (input.empty())
            throw std::runtime_error("input is empty");

        if (input.size() > 1) {
            auto it = std::next(input.cbegin());
            for (; *it == '0' && it != input.cend(); ++it)
                leading_zeros++;

            std::size_t ms_bits{3};
            while ((convert(*it) & (1 << ms_bits)) == 0 && ms_bits > 0)
                ms_bits--;

            ++it;

            std::size_t bits = ms_bits + (3 * distance(it, input.cend()));
            std::fill_n(std::back_inserter(output), leading_zeros, 0);
            std::fill_n(std::back_inserter(output), bits / 8 + 1, 0);
        } else {
            return;
        }

        auto it = input.crbegin();
        auto out = output.rbegin();
        auto inserter = std::back_inserter(output);

        std::uint8_t overflow;
        std::uint8_t offset{0};
        for (; it != std::prev(input.crend(), leading_zeros + 1) &&
               out != output.rend();
             ++it) {
            auto value = convert(*it);

            if (offset > 0 && offset < 3)
                *out |= overflow;

            *out |= (value & mask) << offset;

            if (offset > 4)
                ++out;

            if (offset > 5)
                overflow = value >> (8 - offset);

            offset = (offset + 3) & mask;
        }
    }

    // Base16
    template <>
    void encode<Protocol::Base16>(std::vector<std::uint8_t> const& input,
                                  std::string& output) {
        std::array const characters{'0', '1', '2', '3', '4', '5', '6', '7',
                                    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

        output.reserve(2 * input.size());
        auto inserter = std::back_inserter(output);
        inserter = 'f';

        for (auto elem : input) {
            inserter = characters[(0xf0 & elem) >> 4];
            inserter = characters[0x0f & elem];
        }
    }

    template <>
    void decode<Protocol::Base16>(std::string const& input,
                                  std::vector<std::uint8_t>& output) {
        if (input.size() % 2 != 1)
            throw std::runtime_error("incorrect alignment for Base16");

        output.reserve(input.size() / 2);

        auto it = std::next(input.cbegin());
        auto inserter = std::back_inserter(output);
        auto convert = [](std::uint8_t num) {
            std::uint8_t value;
            if (num > 47 && num < 58)
                value = num - 48;
            else if (num > 96 && num < 123)
                value = num - 87;
            else
                throw std::runtime_error("invalid character");

            return value;
        };

        while (it != input.cend()) {
            auto ms = *it;
            ++it;
            auto ls = *it;
            ++it;

            inserter = (convert(ms) << 4) | convert(ls);
        }
    }

    // Base10
    template <>
    void encode<Protocol::Base10>(std::vector<std::uint8_t> const& input,
                                  std::string& output) {
        std::uint8_t const mask{0x7};

        if (input.empty()) {
            output.push_back('9');
            return;
        }

        std::uint32_t overflow{0};
        auto [leading_zeros, it] =
            count_consecutive(input.cbegin(), input.cend(), 0);
        std::uint8_t bit{7};
        while ((*it & (1 << bit)) == 0 && bit)
            bit--;

        std::vector<std::uint32_t> buf{0};

        // double dabble algorithm
        for (; it != input.cend(); ++it) {
            for (; bit < 8; bit--) {
                // add 3
                for (auto& elem : buf) {
                    for (int i = 0; i < 32; i += 4) {
                        std::uint32_t msk = 0xf;
                        std::uint32_t min = 4;
                        std::uint32_t offset = 3;

                        if ((elem & (msk << i)) > (min << i))
                            elem += (offset << i);
                    }
                }

                // check to see if a new word is required
                if (buf.back() & (1 << 31))
                    buf.push_back(0);

                // shift
                bool b_overflow = (*it & (1 << bit)) != 0;
                for (auto& auto_elem : buf) {
                    bool tmp = static_cast<bool>(auto_elem & (1 << 31));
                    auto_elem = (auto_elem << 1) || b_overflow;     // bug fixing
                    b_overflow = tmp;
                }
            }

            bit &= mask;
        }

        // formatting output
        std::stringstream ss;

        // print leading zeros
        ss << '9';
        for (auto i = 0; i < leading_zeros; i++)
            ss << '0';

        auto buf_it = buf.crbegin();
        ss << std::hex << *(buf_it++) << std::setfill('0') << std::setw(2);
        for (; buf_it != buf.crend(); ++buf_it)
            ss << *buf_it;

        output = ss.str();
    }

    template <>
    void decode<Protocol::Base10>(std::string const& input,
                                  std::vector<std::uint8_t>& output) {
        if (input.size() < 2)
            return;

        auto [leading_zeros, begin] =
            count_consecutive(std::next(input.cbegin()), input.cend(), '0');

        std::vector<std::uint64_t> buf;
        auto const bits =
            std::numeric_limits<decltype(buf)::value_type>::digits;
        auto const str_width = bits / 4;

        auto it = input.cend();
        while (std::distance(begin, it) > str_width) {
            it -= str_width;
            std::string tmp{it, std::next(it, str_width)};
            buf.push_back(std::stoull(tmp, 0, 16));
        }

        if (std::distance(begin, it) > 0) {
            std::string tmp{begin, it};
            buf.push_back(std::stoull(tmp, 0, 16));
        }

        std::uint8_t std_bit{};
        // reverse the double dabble algorithm
        for (auto auto_bit = 0; std::any_of(buf.cbegin(), buf.cend(),
                                       [](auto& elem) { return elem != 0; });
             auto_bit = (auto_bit + 1) & 0x7) {

            if (auto_bit == 0)
                output.push_back(0);

            // transfer lsb
            output.back() |= (buf.front() & 0x1) << auto_bit;

            // shift everything
            std::uint64_t carry{};
            for (auto auto_it = buf.rbegin(); auto_it != buf.rend(); ++auto_it) {
                std::uint64_t tmp = *auto_it << (bits - 1);
                *auto_it = (*auto_it >> 1) | carry;
                carry = tmp;
            }

            // subtract 3 from each digit if greater than 7
            std::uint64_t const mask{0xf};
            for (auto& elem : buf)
                for (auto i = 0; i < bits; i += 4)
                    if (((elem & (mask << i)) >> i) > 7)
                        elem -= (3ull << i);
        }

        // add in "leading zeros"
        std::fill_n(std::back_inserter(output), leading_zeros, 0);
        std::reverse(output.begin(), output.end());
    }

    // Base32 variables / functions
    std::array const base32_lookup{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                                   'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                                   'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                                   'y', 'z', '2', '3', '4', '5', '6', '7'};

    std::array const base32_hex_lookup{'0', '1', '2', '3', '4', '5', '6', '7',
                                       '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
                                       'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                       'o', 'p', 'q', 'r', 's', 't', 'u', 'v'};

    std::array const base32_z_lookup{'y', 'b', 'n', 'd', 'r', 'f', 'g', '8',
                                     'e', 'j', 'k', 'm', 'c', 'p', 'q', 'x',
                                     'o', 't', '1', 'u', 'w', 'i', 's', 'z',
                                     'a', '3', '4', '5', 'h', '7', '6', '9'};

    void base32_encode(std::array<char, 32> const& lookup, bool padding,
                       std::vector<std::uint8_t> const& input,
                       std::string& output) {
        std::uint8_t const mask{0x1f};
        std::size_t size{padding ? ((((input.size() - 1) / 5) + 1) * 8 + 1)
                                 : ((((input.size() * 8) + 4) / 5) + 1)};
        output.reserve(size);
        auto inserter = std::back_inserter(output);
        inserter = 'b';

        std::uint8_t offset{0};
        std::uint8_t overflow{0};

        auto it = input.cbegin();
        for (; it != input.cend(); ++it) {
            // grab overflow
            if (offset > 0) {
                auto shift = 8 - offset;
                auto value = (*it & (mask << shift)) >> shift;
                inserter = lookup[overflow + value];
            }

            for (; offset < 8; offset += 5) {
                if (offset > 3) {
                    auto shift = offset - 3;
                    auto value = (*it & (mask >> shift)) << shift;
                    if (it == std::prev(input.cend()))
                        inserter = lookup[value];
                    else
                        overflow = value;
                } else {
                    auto shift = 3 - offset;
                    auto value = (*it & (mask << shift)) >> shift;
                    inserter = lookup[value];
                }
            }

            offset &= 0x7;
        }

        if (padding) {
            auto mod = (output.size() - 1) % 8;
            std::size_t pads = mod == 0 ? 0 : 8 - mod;
            std::fill_n(inserter, pads, '=');
        }
    }

    void base32_decode(std::array<char, 32> const& lookup, bool padding,
                       std::string const& input,
                       std::vector<std::uint8_t>& output) {

        std::size_t padding_count{};
        if (padding) {
            for (auto it = input.crbegin(); *it == '=' && it != input.crend();
                 ++it)
                ++padding_count;
        }

        std::size_t size{(input.size() - padding_count - 1) * 5 / 8};
        std::fill_n(std::back_inserter(output), size, 0);

        std::uint8_t offset{3};
        std::uint8_t overflow{};
        auto end =
            padding ? std::prev(input.cend(), padding_count) : input.cend();
        auto out = output.begin();

        for (auto it = std::next(input.cbegin()); it != end; ++it) {
            std::uint8_t value =
                std::distance(lookup.cbegin(),
                              std::find(lookup.cbegin(), lookup.cend(), *it));
            *out |= value << offset;

            auto carry_back = value >> (8 - offset);
            if (offset > 3 && out != output.begin())
                *std::prev(out) |= carry_back;

            if (offset < 5)
                ++out;

            offset = (offset - 5) & 0x7;
        }
    }

    // Base32Hex
    template <>
    void encode<Protocol::Base32Hex>(std::vector<std::uint8_t> const& input,
                                     std::string& output) {
        base32_encode(base32_hex_lookup, false, input, output);
        output.front() = 'v';
    }

    template <>
    void decode<Protocol::Base32Hex>(std::string const& input,
                                     std::vector<std::uint8_t>& output) {
        base32_decode(base32_hex_lookup, false, input, output);
    }

    // Base32HexPad
    template <>
    void encode<Protocol::Base32HexPad>(std::vector<std::uint8_t> const& input,
                                        std::string& output) {
        base32_encode(base32_hex_lookup, true, input, output);
        output.front() = 't';
    }

    template <>
    void decode<Protocol::Base32HexPad>(std::string const& input,
                                        std::vector<std::uint8_t>& output) {
        base32_decode(base32_hex_lookup, true, input, output);
    }

    // Base32
    template <>
    void encode<Protocol::Base32>(std::vector<std::uint8_t> const& input,
                                  std::string& output) {
        base32_encode(base32_lookup, false, input, output);
    }

    template <>
    void decode<Protocol::Base32>(std::string const& input,
                                  std::vector<std::uint8_t>& output) {
        base32_decode(base32_lookup, false, input, output);
    }

    // Base32Pad
    template <>
    void encode<Protocol::Base32Pad>(std::vector<std::uint8_t> const& input,
                                     std::string& output) {
        base32_encode(base32_lookup, true, input, output);
        output.front() = 'c';
    }

    template <>
    void decode<Protocol::Base32Pad>(std::string const& input,
                                     std::vector<std::uint8_t>& output) {
        base32_decode(base32_lookup, true, input, output);
    }

    // Base32Z
    template <>
    void encode<Protocol::Base32Z>(std::vector<std::uint8_t> const& input,
                                   std::string& output) {
        base32_encode(base32_z_lookup, false, input, output);
        output.front() = 'h';
    }

    template <>
    void decode<Protocol::Base32Z>(std::string const& input,
                                   std::vector<std::uint8_t>& output) {
        base32_decode(base32_z_lookup, false, input, output);
    }

    // Base58 Stuff goes here
    template <typename InputIt, typename OutputIt>
    auto switch_case(InputIt begin, InputIt end, OutputIt out) {
        return std::transform(begin, end, out, [](auto& elem) {
            if (std::islower(elem))
                return static_cast<char>(std::toupper(elem));
            else if (std::isupper(elem))
                return static_cast<char>(std::tolower(elem));
            else
                return elem;
        });
    }

    std::array const base58_btc_lookup{
        '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C',
        'D', 'E', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q',
        'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c',
        'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'm', 'n', 'o', 'p',
        'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    std::array const base58_flickr_lookup{
        '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c',
        'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'm', 'n', 'o', 'p',
        'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B',
        'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    void base58_encode(std::array<char, 58> const& lookup,
                       std::vector<std::uint8_t> const& input,
                       std::string& output) {

        auto [leading_zeros, it] =
            count_consecutive(input.cbegin(), input.cend(), 0);

        std::vector<std::uint8_t> buf;
        std::size_t size = (std::distance(it, input.cend()) * 138 / 100) + 1;
        std::fill_n(std::back_inserter(buf), size, 0);

        std::uint32_t carry{};
        for (; it != input.cend(); ++it) {
            carry = *it;
            for (auto it_buf = buf.rbegin(); it_buf != buf.rend(); ++it_buf) {
                carry += 256 * (*it_buf);
                *it_buf = carry % 58;
                carry = carry / 58;
            }
        }

        // erase any leading zeros
        auto zero_end = buf.begin();
        while (*zero_end == 0 && zero_end != buf.end())
            ++zero_end;

        if (buf.front() == 0)
            buf.erase(buf.begin(), zero_end);

        output.reserve(leading_zeros + buf.size());
        auto inserter = std::back_inserter(output);
        inserter = ' ';
        std::fill_n(inserter, leading_zeros, '1');
        std::cout << output << std::endl;
        std::transform(buf.cbegin(), buf.cend(), inserter,
                       [&](auto& elem) { return lookup[elem]; });
    }

    void base58_decode(std::array<char, 58> const& lookup,
                       std::string const& input,
                       std::vector<std::uint8_t>& output) {

        if (input.size() < 2)
            return;

        auto [leading_zeros, it] =
            count_consecutive(std::next(input.cbegin()), input.cend(), '1');

        output.push_back(0);

        for (; it != input.cend(); ++it) {
            std::uint32_t carry =
                std::distance(lookup.cbegin(),
                              std::find(lookup.cbegin(), lookup.cend(), *it));

            for (auto i = 0; i < output.size(); ++i) {
                carry += output[i] * 58;
                output[i] = carry;
                carry = carry >> 8;
                if (i == (output.size() - 1) && carry)
                    output.push_back(0);
            }
        }

        std::fill_n(std::back_inserter(output), leading_zeros, 0);
        std::reverse(output.begin(), output.end());
    }

    template <>
    void encode<Protocol::Base58Btc>(std::vector<std::uint8_t> const& input,
                                     std::string& output) {
        base58_encode(base58_btc_lookup, input, output);
        output.front() = 'z';
    }

    template <>
    void decode<Protocol::Base58Btc>(std::string const& input,
                                     std::vector<std::uint8_t>& output) {
        base58_decode(base58_btc_lookup, input, output);
    }
    template <>
    void encode<Protocol::Base58Flickr>(std::vector<std::uint8_t> const& input,
                                        std::string& output) {
        base58_encode(base58_flickr_lookup, input, output);
        output.front() = 'Z';
    }

    template <>
    void decode<Protocol::Base58Flickr>(std::string const& input,
                                        std::vector<std::uint8_t>& output) {
        base58_decode(base58_flickr_lookup, input, output);
    }

    // Base64Pad
    std::string add_padding(std::string const& input) {
        std::string tmp{input};
        std::fill_n(std::back_inserter(tmp), 4 - ((input.size() - 1) % 4), '=');
        return tmp;
    }

    auto to_url = [](auto& elem) {
        switch (elem) {
        case '+':
            return '-';
        case '/':
            return '_';
        default:
            return elem;
        }
    };

    auto from_url = [](auto& elem) {
        switch (elem) {
        case '-':
            return '+';
        case '_':
            return '/';
        default:
            return elem;
        }
    };

    template <>
    void encode<Protocol::Base64Pad>(std::vector<std::uint8_t> const& input,
                                     std::string& output) {
        if (input.empty()) {
            output = "M";
            return;
        }

        std::vector<std::uint8_t> buf;
        std::fill_n(std::back_inserter(buf),
                    (((input.size() - 1) / 3) + 1) * 4 + 2, 0);
        buf[0] = 'M';
        EVP_EncodeBlock(buf.data() + 1, input.data(), input.size());
        output = std::string{buf.begin(), std::prev(buf.end())};
    }

    template <>
    void decode<Protocol::Base64Pad>(std::string const& input,
                                     std::vector<std::uint8_t>& output) {
        auto [padding_count, it] =
            count_consecutive(input.crbegin(), input.crend(), '=');
        std::fill_n(std::back_inserter(output),
                    ((input.size() - padding_count - 1) * 3) / 4, 0);
        if (EVP_DecodeBlock(
                output.data(),
                reinterpret_cast<unsigned const char*>(input.c_str() + 1),
                input.size() - 1) == -1)
            throw std::runtime_error("decoding error");
    }

    // Base64
    template <>
    void encode<Protocol::Base64>(std::vector<std::uint8_t> const& input,
                                  std::string& output) {
        if (input.empty()) {
            output = "m";
            return;
        }

        encode<Protocol::Base64Pad>(input, output);
        output[0] = 'm';
        while (output.back() == '=')
            output.pop_back();
    }

    template <>
    void decode<Protocol::Base64>(std::string const& input,
                                  std::vector<std::uint8_t>& output) {
        decode<Protocol::Base64Pad>(add_padding(input), output);
    }
    // Base64UrlPad
    template <>
    void encode<Protocol::Base64UrlPad>(std::vector<std::uint8_t> const& input,
                                        std::string& output) {
        if (input.empty()) {
            output = "U";
            return;
        }

        encode<Protocol::Base64Pad>(input, output);
        output.front() = 'U';
        std::transform(output.begin(), output.end(), output.begin(), to_url);
    }

    template <>
    void decode<Protocol::Base64UrlPad>(std::string const& input,
                                        std::vector<std::uint8_t>& output) {
        std::string tmp;
        std::transform(input.cbegin(), input.cend(), std::back_inserter(tmp),
                       from_url);
        decode<Protocol::Base64Pad>(tmp, output);
    }

    // Base64Url
    template <>
    void encode<Protocol::Base64Url>(std::vector<std::uint8_t> const& input,
                                     std::string& output) {
        if (input.empty()) {
            output = "u";
            return;
        }

        encode<Protocol::Base64UrlPad>(input, output);
        output[0] = 'u';
        while (output.back() == '=')
            output.pop_back();
    }

    template <>
    void decode<Protocol::Base64Url>(std::string const& input,
                                     std::vector<std::uint8_t>& output) {
        std::string tmp;
        std::transform(input.cbegin(), input.cend(), std::back_inserter(tmp),
                       from_url);
        decode<Protocol::Base64Pad>(add_padding(tmp), output);
    }

    struct Coder {
        using Encoder = void (*)(std::vector<std::uint8_t> const&,
                                 std::string&);
        using Decoder = void (*)(std::string const&,
                                 std::vector<std::uint8_t>&);

        Encoder encoder;
        Decoder decoder;
    };

    template <Protocol protocol>
    constexpr auto make_coder_entry() {
        return std::make_pair(protocol,
                              Coder{encode<protocol>, decode<protocol>});
    }

    template <Protocol upper, Protocol lower>
    constexpr auto make_upper_coder_entry() {
        return std::make_pair(upper,
                              Coder{[](std::vector<std::uint8_t> const& input,
                                       std::string& output) {
                                        encode_upper<lower>(input, output);
                                    },
                                    [](std::string const& input,
                                       std::vector<std::uint8_t>& output) {
                                        decode_upper<lower>(input, output);
                                    }});
    }

    std::unordered_map<Protocol, Coder> const coders{
        make_coder_entry<Protocol::Base2>(),
        make_coder_entry<Protocol::Base8>(),
        make_coder_entry<Protocol::Base10>(),
        make_coder_entry<Protocol::Base16>(),
        make_upper_coder_entry<Protocol::Base16Upper, Protocol::Base16>(),
        make_coder_entry<Protocol::Base32Hex>(),
        make_upper_coder_entry<Protocol::Base32HexUpper, Protocol::Base32Hex>(),
        make_coder_entry<Protocol::Base32HexPad>(),
        make_upper_coder_entry<Protocol::Base32HexPadUpper,
                               Protocol::Base32HexPad>(),
        make_coder_entry<Protocol::Base32>(),
        make_upper_coder_entry<Protocol::Base32Upper, Protocol::Base32>(),
        make_coder_entry<Protocol::Base32Pad>(),
        make_upper_coder_entry<Protocol::Base32PadUpper, Protocol::Base32Pad>(),
        make_coder_entry<Protocol::Base58Btc>(),
        make_coder_entry<Protocol::Base58Flickr>(),
        make_coder_entry<Protocol::Base32Z>(),
        make_coder_entry<Protocol::Base64>(),
        make_coder_entry<Protocol::Base64Pad>(),
        make_coder_entry<Protocol::Base64Url>(),
        make_coder_entry<Protocol::Base64UrlPad>()};

    auto find_coder(Protocol protocol) {
        auto it = coders.find(protocol);
        if (it == coders.end())
            throw std::runtime_error("unsupported protocol");
        return it->second;
    }
} // namespace

namespace Multiformats::Multibase {
    /** @param protocol The protocol you want to print
     *  @return The string representation */
    std::string to_string(Protocol protocol) {
        switch (protocol) {
        case Protocol::Identity:
            return "identity";
        case Protocol::Base2:
            return "base2";
        case Protocol::Base8:
            return "base8";
        case Protocol::Base10:
            return "base10";
        case Protocol::Base16:
            return "base16";
        case Protocol::Base16Upper:
            return "base16upper";
        case Protocol::Base32Hex:
            return "base32hex";
        case Protocol::Base32HexUpper:
            return "base32hexupper";
        case Protocol::Base32HexPad:
            return "base32hexpad";
        case Protocol::Base32HexPadUpper:
            return "base32hexpadupper";
        case Protocol::Base32:
            return "base32";
        case Protocol::Base32Upper:
            return "base32upper";
        case Protocol::Base32Pad:
            return "base32pad";
        case Protocol::Base32PadUpper:
            return "base32padupper";
        case Protocol::Base32Z:
            return "base32z";
        case Protocol::Base58Flickr:
            return "base58flickr";
        case Protocol::Base58Btc:
            return "base58btc";
        case Protocol::Base64:
            return "base64";
        case Protocol::Base64Pad:
            return "base64pad";
        case Protocol::Base64Url:
            return "base64url";
        case Protocol::Base64UrlPad:
            return "base64urlpad";
        }

        // if not found, throw
        throw std::runtime_error("unknown protocol to_string");
    }

    std::vector<std::uint8_t> decode(std::string const& str) {
        auto protocol = validate(str);
        std::vector<std::uint8_t> ret;
        find_coder(protocol).decoder(str, ret);

        return ret;
    }

    std::string encode(Protocol protocol,
                       std::vector<std::uint8_t> const& buf) {
        std::string ret;
        find_coder(protocol).encoder(buf, ret);

        return ret;
    }
} // namespace Multiformats::Multibase
