#pragma once

#include <memory>
#include <functional>
#include <ostream>

#include "request.hpp"

namespace sweet {
namespace marble {

typedef std::function<void(std::ostream&, std::shared_ptr<Request>)> 
	Function;
}
}
