#include "server_http.hpp"

namespace sweet {
namespace marble {

long safeSizetToLong(const size_t value) {
	if(value > std::numeric_limits<long>::max()) {
		throw std::range_error("failed to convert size_t to long");
	}
	
	return static_cast<long>(value);
}

}
}
