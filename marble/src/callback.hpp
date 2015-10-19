#pragma once

#include "callbacktype.hpp"
#include "function.hpp"

namespace sweet {
namespace marble {

	struct Callback {
		CallbackType type;
		Function functions[2];

		Callback();
		Callback(CallbackType ct, Function func);
	};
}
}
