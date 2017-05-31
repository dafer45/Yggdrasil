#ifndef COM_DAFER45_YGGDRASIL_IN_STRING_BUFFER
#define COM_DAFER45_YGGDRASIL_IN_STRING_BUFFER

#include <streambuf>
#include <string>

namespace Ygg{

class InStringBuffer : public std::streambuf{
public:
	/** Constructor. */
	InStringBuffer(const std::string& str);

	/** Destructor. */
	~InStringBuffer();
private:
	/** Buffer. */
	char* buffer;
};

};	//End of namespace Ygg

#endif
