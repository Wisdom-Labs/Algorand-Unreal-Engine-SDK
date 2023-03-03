#include "Address.h"
#include "base.h"
#include "include/utils/sha512_256.h"

#include <cassert>

bool
Address::is_zero() const {
    return public_key == bytes{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                               0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
}

Address::Address(std::string address) :
    Address(address, b32_decode(address)) {
}

Address::Address(std::string address, bytes with_checksum) :
    as_string(address),
    public_key(bytes{ with_checksum.begin(), with_checksum.begin() + 32 }) {
    assert(as_string.size() == 58);
    assert(public_key.size() == 32);
}

Address& Address::operator=(const Address& other)
{
    if (this != &other) // check for self-assignment
    {
        as_string.assign(other.as_string.begin(), other.as_string.end());
        public_key.assign(other.public_key.begin(), other.public_key.end());
    }
    return *this;
}

static bytes
checksummed(bytes public_key) {
    bytes copy(public_key);
    unsigned char result[32] = { 0 };
    
    ret_code_t err_code = VTC_SUCCESS;
    err_code = sha512_256((const unsigned char*)public_key.data(), public_key.size(), result, 32);
    auto hash = bytes(result, result + sizeof(result));
    copy.insert(copy.end(), hash.end() - 4, hash.end());
    return copy;

}
Address::Address(bytes public_key) : Address(public_key, checksummed(public_key)) { }

Address::Address(bytes public_key, bytes checksummed) :
    as_string(b32_encode(checksummed)),
    public_key(public_key) {
    assert(as_string.size() == 58);
    assert(public_key.size() == 32);
}
