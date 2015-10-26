#pragma once

#include <boost/asio.hpp>
#include <unordered_map>
#include <regex>

namespace sweet {
namespace marble {

class Request {
public:
	std::string method, path, http_version;
	std::istream content;
	std::unordered_map<std::string, std::string> header;
	std::smatch path_match;

	Request();
	
	boost::asio::streambuf content_buffer;
};

}
}
