// Multiaddr -- self-describing network addresses
//
// Author: Matthew Knight
// File Name: multiaddr.hpp
// Date: 2019-09-12

#include <sstream>

#include "multiaddr.hpp"

#include "multicodec.hpp"
#include "varint.hpp"

#include <regex>
#include <string>
#include <vector>

#include <cstdint>

namespace {
    using namespace Multiformats;

    std::uint64_t const ip4{4};
    std::uint64_t const ip6{41};

    std::uint64_t const tcp{6};
    std::uint64_t const udp{273};
    std::uint64_t const dccp{33};
    std::uint64_t const sctp{132};

    struct ProtocolInfo {
        Varint code;
        int byte_length;
    };

    std::vector<ProtocolInfo> const protocol_info{
        {4, 4},    {6, 2},    {273, 2},  {33, 2},   {41, 16},  {42, -1},
        {53, -1},  {54, -1},  {55, -1},  {56, -1},  {132, 2},  {301, 0},
        {302, 0},  {400, -1}, {421, -1}, {421, -1}, {444, 12}, {445, 37},
        {446, -1}, {447, -1}, {460, 0},  {480, 0},  {443, 0},  {477, 0},
        {478, 0},  {479, 0},  {277, 0},  {275, 0},  {276, 0},  {290, 0},
        {777, -1},
    };

    template <typename Iterator>
    std::vector<std::uint8_t> ip4_to_binary(Iterator begin, Iterator end) {
        std::regex const pattern{"^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$"};
        if (!std::regex_match(begin, end, pattern))
            throw std::invalid_argument("invalid ip4 address");

        std::vector<std::uint8_t> ret;

        for (auto it = begin; it != end;) {
            auto period = std::find(it, end, '.');
            std::string tmp{it, period};
            ret.push_back(std::strtoul(tmp.c_str(), nullptr, 10));

            if (period == end)
                it = end;
            else
                it = std::next(period);
        }

        return ret;
    }

    template <typename Iterator>
    std::vector<std::uint8_t> ip6_to_binary(Iterator begin, Iterator end) {
        std::regex const pattern{"^(?:[A-Fa-f0-9]{1,4}:){7}[A-Fa-f0-9]{1,4}$"};
        if (!std::regex_match(begin, end, pattern))
            throw std::invalid_argument("invalid ip4 address");

        std::vector<std::uint8_t> ret;

        for (auto it = begin; it != end;) {
            auto colon = std::find(it, end, ':');

            std::string tmp{it, colon};
            std::uint16_t value = std::strtoul(tmp.c_str(), nullptr, 16);

            ret.push_back(value >> 8);
            ret.push_back(value & 0xff);

            if (colon == end)
                it = end;
            else
                it = std::next(colon);
        }

        return ret;
    }

    template <typename Iterator>
    std::vector<std::uint8_t> port_to_binary(Iterator begin, Iterator end) {
        std::string tmp{begin, end};
        std::uint16_t port{
            static_cast<std::uint16_t>(std::strtoul(tmp.c_str(), nullptr, 10))};
        return {static_cast<std::uint8_t>(port >> 8),
                static_cast<std::uint8_t>(port & 0xff)};
    }

    std::string ip4_to_string(std::vector<std::uint8_t> const& buf) {
        std::string ret;
        for (auto it = buf.cbegin(); it != buf.cend(); ++it) {
            ret += std::to_string(*it);
            if (it != std::prev(buf.cend()))
                ret += '.';
        }

        return ret;
    }

    std::string ip6_to_string(std::vector<std::uint8_t> const& buf) {
        std::stringstream ss;
        ss << std::hex;
        for (auto it = buf.cbegin(); it != buf.cend(); it += 2) {
            int value = (*it << 8) | *std::next(it);
            ss << value;
            if (it != std::prev(buf.cend(), 2))
                ss << ':';
        }

        return ss.str();
    }

    std::string port_to_string(std::vector<std::uint8_t> const& buf) {
        if (buf.size() != 2)
            throw std::invalid_argument("buffer is incorrect size");

        int value = (buf[0] << 8) | buf[1];
        return std::to_string(value);
    }

    template <typename Iterator>
    std::vector<std::uint8_t> to_binary(Varint const& code, Iterator begin,
                                        Iterator end) {
        std::uint64_t value = code;
        switch (value) {
        case ip4:
            return ip4_to_binary(begin, end);
        case ip6:
            return ip6_to_binary(begin, end);
        case tcp:
            [[fallthrough]];
        case udp:
            [[fallthrough]];
        case dccp:
            [[fallthrough]];
        case sctp:
            return port_to_binary(begin, end);
        }

        // default is to assume string stored in buf
        return {begin, end};
    }

    std::string to_string(Varint const& code,
                          std::vector<std::uint8_t> const& buf) {
        std::uint64_t value = code;
        switch (value) {
        case ip4:
            return ip4_to_string(buf);
        case ip6:
            return ip6_to_string(buf);
        case tcp:
            [[fallthrough]];
        case udp:
            [[fallthrough]];
        case dccp:
            [[fallthrough]];
        case sctp:
            return port_to_string(buf);
        }

        // default is to assume buf is a string
        return {buf.cbegin(), buf.cend()};
    }
} // namespace

namespace Multiformats {
    Multiaddr::Multiaddr(std::string const& address) {
        if (address.empty() || address[0] != '/')
            throw std::invalid_argument("invalid multiaddr");

        auto begin = address.cbegin();

        for (auto end = begin; begin != address.cend(); begin = end) {
            end = std::find(std::next(begin), address.cend(), '/');
            std::string protocol_str{std::next(begin), end};
            Protocol protocol{Multicodec::table.at(protocol_str)};

            auto info_it = std::find_if(
                protocol_info.cbegin(), protocol_info.cend(),
                [&](auto const& info) { return info.code == protocol.code; });

            if (info_it == protocol_info.cend())
                throw std::invalid_argument("unsupported multiaddr protocol");

            // try to get value for protocol
            if (info_it->byte_length != 0) {
                begin = end;
                if (protocol_str == "unix")
                    end = address.cend();
                else
                    end = std::find(std::next(begin), address.cend(), '/');

                protocol.value =
                    ::to_binary(protocol.code, std::next(begin), end);
                if (info_it->byte_length >= 0 &&
                    protocol.value.size() != info_it->byte_length)
                    throw std::runtime_error("incorrect buffer size");
            }

            addr.push_back(protocol);
        }
    }

    Multiaddr::Multiaddr(std::vector<std::uint8_t> const& raw) {
        auto it = raw.cbegin();
        while (it != raw.cend()) {
            auto [code, next] = make_varint(it, raw.cend());
            Protocol protocol{code};
            auto info_it =
                std::find_if(protocol_info.cbegin(), protocol_info.cend(),
                             [code = code](auto const& protocol) {
                                 return code == protocol.code;
                             });
            if (info_it == protocol_info.cend())
                throw std::runtime_error("unsupported protocol");

            std::size_t size;
            if (info_it->byte_length < 0) {
                auto [varint, value_it] = make_varint(next, raw.cend());
                it = value_it;
                size = varint;
            } else {
                it = next;
                size = info_it->byte_length;
            }

            std::copy_n(it, size, std::back_inserter(protocol.value));
            it += size;

            addr.push_back(protocol);
        }
    }

    std::string Multiaddr::Protocol::to_string() const {
        return ::to_string(code, value);
    }

    std::string Multiaddr::to_string() const {
        std::string ret;
        for (auto const& address : addr) {
            auto it = std::find_if(
                Multicodec::table.cbegin(), Multicodec::table.cend(),
                [&](auto const& elem) { return address.code == elem.second; });

            if (it == Multicodec::table.cend())
                throw std::runtime_error(
                    "unsupported protocol within multiaddr");

            auto info_it = std::find_if(
                protocol_info.cbegin(), protocol_info.cend(),
                [&](auto const& info) { return address.code == info.code; });

            if (info_it == protocol_info.cend())
                throw std::runtime_error(
                    "unsupported protocol within multiaddr");

            ret += '/';
            ret += it->first;

            if (info_it->byte_length != 0) {
                ret += '/';
                ret += address.to_string();
            }
        }

        return ret;
    }

    std::vector<std::uint8_t> Multiaddr::to_binary() const {
        std::vector<std::uint8_t> ret;
        for (auto const& address : addr) {
            auto it = std::find_if(protocol_info.cbegin(), protocol_info.cend(),
                                   [&](auto const& protocol) {
                                       return address.code == protocol.code;
                                   });

            if (it == protocol_info.cend())
                throw std::runtime_error(
                    "unsupported protocol within multiaddr");

            std::copy(address.code.begin(), address.code.end(),
                      std::back_inserter(ret));

            if (it->byte_length != 0) {
                if (it->byte_length < 0) {
                    Varint len{address.value.size()};
                    std::copy(len.begin(), len.end(), std::back_inserter(ret));
                }

                std::copy(address.value.cbegin(), address.value.cend(),
                          std::back_inserter(ret));
            }
        }

        return ret;
    }

    std::size_t Multiaddr::size() const { return addr.size(); }

    Multiaddr::ConstIterator Multiaddr::begin() const { return addr.cbegin(); }

    Multiaddr::ConstIterator Multiaddr::end() const { return addr.cend(); }

    Multiaddr::ConstReference Multiaddr::front() const { return addr.front(); }

    Multiaddr::ConstReference Multiaddr::back() const { return addr.back(); }
}; // namespace Multiformats
