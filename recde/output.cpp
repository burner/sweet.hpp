#include <output.hpp>

Output::Output(std::ostream& ps,std::ostream& ph,std::ostream& as, 
		std::ostream& ah, std::ostream& es, std::ostream& eh, std::ostream& vh, 
		std::ostream& oh, std::ostream& os, std::ostream& dh, std::ostream& ds) :
	prsS(ps), prsH(ph), astS(as), astH(ah), errS(es), errH(eh), visH(vh), 
	outH(oh), outS(os), dotH(dh), dotS(ds)
{
}
