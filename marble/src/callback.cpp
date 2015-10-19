
#include "callback.hpp"

namespace sweet {
namespace marble {

Callback::Callback() : type(CallbackType::INVALID) {
}

Callback::Callback(CallbackType ct, Function func) : type(ct) {
	functions[static_cast<size_t>(ct)] = func;
}

}
}
