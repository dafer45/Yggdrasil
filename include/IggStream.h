#ifndef COM_DAFER45_YGGDRASIL_IGGSTREAM
#define COM_DAFER45_YGGDRASIL_IGGSTREAM

#include "YggStream.h"

#include <istream>

namespace Yggdrasil{

/** Input stream. */
class IggStream : public YggStream, public std::istream{
public:
	/** Constructor. */
	IggStream();

	/** Destructor. */
	~IggStream();

	/** Open. */
	void open();
private:
};

};	//End of namespace Yggdrasil

#endif
