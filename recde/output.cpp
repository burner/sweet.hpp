#include <output.hpp>

Output::Output(std::ostream& ps,std::ostream& ph,std::ostream& as,
		std::ostream& ah, std::ostream& es,std::ostream& eh, const std::string& ehfn) :
	prsS(ps), prsH(ph), astS(as), astH(ah), errS(es), errH(eh), errHfn(ehfn)
{
}
