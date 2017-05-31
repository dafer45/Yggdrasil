#ifndef COM_DAFER45_YGGDRASIL_YGGSTREAM
#define COM_DAFER45_YGGDRASIL_YGGSTREAM

#include <algorithm>
#include <string>

namespace Ygg{

/** Base class for IggStream (input) and OggStream (output). */
class YggStream{
public:
	/** Constructor. */
	YggStream();

	/** Destructor. */
	~YggStream();

	/** Set source. */
	void setStore(const std::string& store);

	/** Set resource. */
	void setResource(const std::string& resource);
protected:
	/** Get store. */
	const std::string getStore() const;

	/** Get resource. */
	const std::string getResource() const;

	/** Find resource. */
	void locateResource();
private:
	/** Store. */
	std::string store;

	/** Resource. */
	std::string resource;

	/** Convert string to lower case. */
	std::string toLowerCase(const std::string& str) const;
};

inline void YggStream::setStore(const std::string& store){
	this->store = store;
}

inline void YggStream::setResource(const std::string& resource){
	this->resource = resource;
}

/*inline const std::string& YggStream::getStore() const{
	return store;
}

inline const std::string& YggStream::getResource() const{
	return resource;
}*/

inline std::string YggStream::toLowerCase(const std::string& str) const{
	std::string result = str;
	std::transform(
		str.begin(),
		str.end(),
		result.begin(),
		::tolower
	);

	return result;
}

};	//End of namespace Ygg

#endif
