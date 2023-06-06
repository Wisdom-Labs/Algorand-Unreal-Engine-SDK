#pragma once

#include <cstring>
#include <vector>
#include <string>
using namespace std;

namespace algorand {
namespace vertices {

typedef std::vector<unsigned char> bytes;

class VERTICES_API Address {
public:
	Address() {}                    // Constructs the ZERO address
	Address& operator=(const Address& other);
	Address(std::string b32form);
	Address(bytes public_key);
	std::string as_string;
	bytes public_key;
	bool is_zero() const;
private:
	Address(std::string s, bytes with_csum);
	Address(bytes public_key, bytes with_csum);
};

}
}