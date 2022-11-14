#pragma once

#include "Address.h"
#include "mnemonic.h"
#include <cassert>
#include <cstring>

#include "../Libs/include/sodium.h"

using namespace std;

typedef std::vector<unsigned char> bytes;

class Account {
public:
	Account(std::string address);
	Account(Address address);
	Account(bytes public_key, bytes secret_key);
	Account(std::pair<bytes, bytes> key_pair);

	static Account from_mnemonic(std::string mnemonic);
	static std::pair<bytes, bytes> generate_keys();
	static std::pair<bytes, bytes> generate_keys(bytes seed);

	std::string mnemonic() const;
	bytes seed() const;
	bytes sign(std::string prefix, bytes msg) const;
	bytes sign(bytes msg) const;

	const bytes public_key() const { return address.public_key; }
	const Address address;
	const bytes secret_key;       // empty() if created from an address, not key
};