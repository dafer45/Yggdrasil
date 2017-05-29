#ifndef COM_DAFER45_YGGDRASIL_IGGSTREAM
#define COM_DAFER45_YGGDRASIL_IGGSTREAM

#include "YggStream.h"

#include <istream>

namespace Ygg{

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

};	//End of namespace Ygg

#endif
