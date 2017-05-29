#ifndef COM_DAFER45_YGGDRASIL_IN_FILE_BUFFER
#define COM_DAFER45_YGGDRASIL_IN_FILE_BUFFER

#include <fstream>
#include <vector>

namespace Ygg{

class InFileBuffer : public std::filebuf{
public:
	/** Constructor. */
	InFileBuffer(const std::string& url);

	/** Destructor. */
	~InFileBuffer();
private:
};

};	//End of namespace Ygg

#endif
