#pragma once

#include <ostream>

struct Output {
	std::ostream& prsS;
	std::ostream& prsH;
	std::ostream& astS;
	std::ostream& astH;
	std::ostream& errS;
	std::ostream& errH;

	explicit Output(std::ostream&,std::ostream&,std::ostream&,std::ostream&,std::ostream&,std::ostream&);
};
