#include "Account.h"

std::pair<bytes, bytes>
Account::generate_keys(bytes seed) {
	assert(sodium_init() >= 0);
	unsigned char ed25519_pk[crypto_sign_ed25519_PUBLICKEYBYTES];
	unsigned char ed25519_sk[crypto_sign_ed25519_SECRETKEYBYTES];

	crypto_sign_ed25519_seed_keypair(ed25519_pk, ed25519_sk, seed.data());
	auto pub = bytes{ ed25519_pk, &ed25519_pk[sizeof(ed25519_pk)] };
	auto sec = bytes{ ed25519_sk, &ed25519_sk[sizeof(ed25519_sk)] };
	return std::make_pair(pub, sec);
}

std::pair<bytes, bytes>
Account::generate_keys() {
	assert(sodium_init() >= 0);
	unsigned char ed25519_pk[crypto_sign_ed25519_PUBLICKEYBYTES];
	unsigned char ed25519_sk[crypto_sign_ed25519_SECRETKEYBYTES];

	crypto_sign_ed25519_keypair(ed25519_pk, ed25519_sk);
	auto pub = bytes{ ed25519_pk, &ed25519_pk[sizeof(ed25519_pk)] };
	auto sec = bytes{ ed25519_sk, &ed25519_sk[sizeof(ed25519_sk)] };
	return std::make_pair(pub, sec);
}

bytes
Account::seed() const {
	unsigned char ed25519_seed[crypto_sign_ed25519_SEEDBYTES];
	crypto_sign_ed25519_sk_to_seed(ed25519_seed, secret_key.data());
	return bytes{ ed25519_seed, &ed25519_seed[sizeof(ed25519_seed)] };
}

Account::Account(std::string address)
	: address(Address(address)) {
}

Account::Account(Address address)
	: address(address) {
}

Account::Account(bytes public_key, bytes secret_key)
	: address(Address(public_key)), secret_key(secret_key) {
	assert(public_key.size() == crypto_sign_ed25519_PUBLICKEYBYTES);
	assert(secret_key.size() == crypto_sign_ed25519_SECRETKEYBYTES);
}

Account::Account(std::pair<bytes, bytes> key_pair) :
	Account(key_pair.first, key_pair.second) {
}

Account Account::from_mnemonic(std::string m) {
	auto seed = seed_from_mnemonic(m);
	auto keys = generate_keys(seed);
	return Account(keys.first, keys.second);
}

std::string Account::mnemonic() const {
	return mnemonic_from_seed(seed());
}