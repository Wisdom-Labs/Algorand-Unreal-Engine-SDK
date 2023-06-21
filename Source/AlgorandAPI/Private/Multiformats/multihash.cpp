// Multihash -- self-describing hashes
//
// Author: Matthew Knight
// File Name: multihash.cpp
// Date: 2019-10-10

#include <iterator>
#include "multihash.hpp"
#include "multicodec.hpp"
#include "varint.hpp"

#include "openssl/evp.h"

#include <array>

namespace {
    using namespace Multiformats;
    std::uint64_t const blake2b_512{0xb240};
    std::uint64_t const blake2s_256{0xb260};
    std::uint64_t const md4{0xd4};
    std::uint64_t const md5{0xd5};
    std::uint64_t const sha1{0x11};
    std::uint64_t const sha2_256{0x12};
    std::uint64_t const sha2_512{0x13};
    std::uint64_t const sha3_224{0x17};
    std::uint64_t const sha3_256{0x16};
    std::uint64_t const sha3_384{0x15};
    std::uint64_t const sha3_512{0x14};
    std::uint64_t const shake_128{0x18};
    std::uint64_t const shake_256{0x19};

    class OpenSSLHasher {
        EVP_MD_CTX* ctx;

      public:
        OpenSSLHasher(EVP_MD const* md)
            : ctx(EVP_MD_CTX_new()) {

            EVP_DigestInit(ctx, md);
            if (ctx == nullptr)
                throw std::invalid_argument(
                    "OpenSSL doesn't support this hash");
        }

        std::vector<std::uint8_t>
        hash(std::vector<std::uint8_t> const& plaintext) {
            int size = EVP_MD_CTX_size(ctx);
            std::vector<std::uint8_t> digest;
            std::fill_n(std::back_inserter(digest), size, 0);
            unsigned digest_len{};

            EVP_DigestUpdate(ctx, plaintext.data(), plaintext.size());
            EVP_DigestFinal(ctx, digest.data(), &digest_len);

            if (digest_len != size)
                throw std::runtime_error("digest size does not match");

            return digest;
        }

        ~OpenSSLHasher() { EVP_MD_CTX_free(ctx); }
    };

    template <typename Hasher, typename... Args>
    auto hash_impl(std::vector<std::uint8_t> const& plaintext, Args... args) {
        Hasher hasher(args...);
        return hasher.hash(plaintext);
    }

    auto hash(Varint const& protocol,
              std::vector<std::uint8_t> const& plaintext) {
        switch (static_cast<std::uint64_t>(protocol)) {
        case sha1:
            return hash_impl<OpenSSLHasher>(plaintext, EVP_sha1());
        case blake2b_512:
            return hash_impl<OpenSSLHasher>(plaintext, EVP_blake2b512());
        case blake2s_256:
            return hash_impl<OpenSSLHasher>(plaintext, EVP_blake2s256());
        case md4:
            return hash_impl<OpenSSLHasher>(plaintext, EVP_md4());
        case md5:
            return hash_impl<OpenSSLHasher>(plaintext, EVP_md5());
        case sha2_256:
            return hash_impl<OpenSSLHasher>(plaintext, EVP_sha256());
        case sha2_512:
            return hash_impl<OpenSSLHasher>(plaintext, EVP_sha512());
        case sha3_224:
            return hash_impl<OpenSSLHasher>(plaintext, EVP_sha3_224());
        case sha3_256:
            return hash_impl<OpenSSLHasher>(plaintext, EVP_sha3_256());
        case sha3_384:
            return hash_impl<OpenSSLHasher>(plaintext, EVP_sha3_384());
        case sha3_512:
            return hash_impl<OpenSSLHasher>(plaintext, EVP_sha3_512());
        case shake_128:
            return hash_impl<OpenSSLHasher>(plaintext, EVP_shake128());
        case shake_256:
            return hash_impl<OpenSSLHasher>(plaintext, EVP_shake256());
        }

        throw std::invalid_argument("unsupported hash function");
    }
} // namespace

namespace Multiformats {
    /**
     * @param plaintext binary to hash
     * @param protocol function code of hash
     * @throw std::invalid_argument if function code is not supported */
    Multihash::Multihash(std::vector<std::uint8_t> const& plaintext,
                         Varint const& protocol) {
//        std::vector<std::uint8_t> digest = hash(protocol, plaintext);
        std::vector<std::uint8_t> digest = plaintext;
        Varint len{digest.size()};

        buf.reserve(protocol.size() + len.size() + digest.size());

        // bundle it all up
        std::copy(protocol.begin(), protocol.end(), std::back_inserter(buf));
        std::copy(len.begin(), len.end(), std::back_inserter(buf));
        std::copy(digest.cbegin(), digest.cend(), std::back_inserter(buf));
    }

    /**
     * @param plaintext binary to hash
     * @param protocol name of hash function
     * @throw std::out_of_range if protocol isn't in multicodec table */
    Multihash::Multihash(std::vector<std::uint8_t> const& plaintext,
                         std::string const& protocol)
        : Multihash(plaintext, Multicodec::table.at(protocol)) {}

    Varint Multihash::func_code() const { return {buf.cbegin(), buf.cend()}; }

    Varint Multihash::len() const {
        auto [fn, len_it] = make_varint(buf.cbegin(), buf.cend());
        auto [len, digest_it] = make_varint(len_it, buf.cend());
        return len;
    }

    std::size_t Multihash::size() const { return buf.size(); }

    Multihash::ConstIterator Multihash::begin() const { return buf.cbegin(); }

    Multihash::ConstIterator Multihash::digest() const {
        auto [fn, len_it] = make_varint(buf.cbegin(), buf.cend());
        auto [len, digest_it] = make_varint(len_it, buf.cend());
        return digest_it;
    }

    Multihash::ConstIterator Multihash::end() const { return buf.cend(); }
} // namespace Multiformats
