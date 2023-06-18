#include "Account.h"

#include <mutex>

std::mutex sodiumMutex;

// Declare a mutex object
std::pair<bytes, bytes>
Account::generate_keys(bytes seed) {
	try
	{
		// Lock the mutex
		sodiumMutex.lock();
		
		assert(sodium_init() >= 0);
		unsigned char ed25519_pk[crypto_sign_ed25519_PUBLICKEYBYTES];
		unsigned char ed25519_sk[crypto_sign_ed25519_SECRETKEYBYTES];
		
		crypto_sign_ed25519_seed_keypair(ed25519_pk, ed25519_sk, seed.data());
		
		auto pub = bytes{ ed25519_pk, &ed25519_pk[sizeof(ed25519_pk)] };
		auto sec = bytes{ ed25519_sk, &ed25519_sk[sizeof(ed25519_sk)] };

		// Unlock the mutex
		sodiumMutex.unlock();
		return std::make_pair(pub, sec);
		
	}
	catch (std::exception &e)
	{
		UE_LOG(LogTemp, Display, TEXT("Didn't generate accounts with valid pub and sec %s"), e.what());
		bytes pub, sec;  
		pub.clear();  sec.clear();
		return std::make_pair(pub, sec);
	}
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
	: address(algorand::vertices::Address(address)) {
}

Account::Account(algorand::vertices::Address address)
	: address(address) {
}

Account::Account(bytes public_key, bytes secret_key)
	: address(algorand::vertices::Address(public_key)), secret_key(secret_key) {
	assert(public_key.size() == crypto_sign_ed25519_PUBLICKEYBYTES);
	assert(secret_key.size() == crypto_sign_ed25519_SECRETKEYBYTES);
}

Account::Account(std::pair<bytes, bytes> key_pair) :
	Account(key_pair.first, key_pair.second) {
}

Account& Account::operator=(const Account& other)
{
	if (this != &other) // check for self-assignment
	{
		secret_key.assign(other.secret_key.begin(), other.secret_key.end());
		address = other.address;
	}
	return *this;
}

Account Account::from_mnemonic(std::string m) {
	auto seed = seed_from_mnemonic(m);
	auto keys = generate_keys(seed);
	return Account(keys.first, keys.second);
}

Account Account::initialize_new() {
	auto keys = generate_keys();
	return Account(keys.first, keys.second);
}

std::string Account::mnemonic() const {
	return mnemonic_from_seed(seed());
}