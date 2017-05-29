#ifndef COM_DAFER45_YGGDRASIL_URL_BUFFER
#define COM_DAFER45_YGGDRASIL_URL_BUFFER

#include <streambuf>
#include <vector>

namespace Ygg{

class URLBuffer : public std::streambuf{
public:
	/** Constructor. */
	URLBuffer(const std::string& url);

	/** Destructor. */
	~URLBuffer();
private:
	/** URL. */
	std::string url;

	/** Buffer. */
	std::vector<char> buffer;

	/** Callback. */
	static size_t writeCallback(
		char *data,
		size_t size,
		size_t nmemb,
		void *userData
	);
};

};	//End of namespace Ygg

#endif
