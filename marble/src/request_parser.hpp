#pragma once

#include <regex>
#include <memory>

#include "request.hpp"

namespace sweet {
namespace marble {

//class Request;

void parser_regex(std::shared_ptr<Request> request, std::istream& stream);
void parser_handwritten(std::shared_ptr<Request> request, std::istream& stream);

}
}
