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
	const std::string& getStore() const;

	/** Get resource. */
	const std::string& getResource() const;
private:
	/** Store. */
	std::string store;

	/** Resource. */
	std::string resource;
};

inline void YggStream::setStore(const std::string& store){
	this->store = store;
	std::transform(
		store.begin(),
		store.end(),
		this->store.begin(),
		::tolower
	);
}

inline void YggStream::setResource(const std::string& resource){
	this->resource = resource;
}

inline const std::string& YggStream::getStore() const{
	return store;
}

inline const std::string& YggStream::getResource() const{
	return resource;
}

};	//End of namespace Ygg

#endif
