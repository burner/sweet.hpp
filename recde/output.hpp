#pragma once

#include <ostream>

struct Output {
	std::ostream& prsS;
	std::ostream& prsH;
	std::ostream& astS;
	std::ostream& astH;
	std::ostream& errS;
	std::ostream& errH;
	std::ostream& visH;
	std::ostream& outH;
	std::ostream& outS;
	std::ostream& dotH;
	std::ostream& dotS;
	std::ostream& mulH;
	std::ostream& mulS;

	std::string errHfn;

	explicit Output(std::ostream&,std::ostream&,std::ostream&,
		std::ostream&,std::ostream&,std::ostream&,
		std::ostream&,std::ostream&,std::ostream&,
		std::ostream&,std::ostream&,std::ostream&,std::ostream&);
};
