#ifndef COM_DAFER45_YGGDRASIL_LIBRARY_ENTRY
#define COM_DAFER45_YGGDRASIL_LIBRARY_ENTRY

#include <string>

#include <json.hpp>

namespace Ygg{

class LibraryEntry{
public:
	/** Constructor. */
	LibraryEntry();

	/** Constructor. */
	LibraryEntry(const nlohmann::json& libraryEntry);

	/** Destructor. */
	~LibraryEntry();

	/** Set key. */
	void setKey(const std::string& key);

	/** Get key. */
	const std::string& getKey() const;

	/** Set store. */
	void setStore(const std::string& store);

	/** Get store. */
	const std::string& getStore() const;

	/** Set resource. */
	void setResource(const std::string& resource);

	/** Get resource. */
	const std::string& getResource() const;

	/** Set hash. */
	void setHash(const std::string& hash);

	/** Get hash. */
	const std::string& getHash() const;

	/** Set hash type. */
	void setHashType(const std::string& hashType);

	/** Get hash type. */
	const std::string& getHashType() const;

	/** Get value string. */
	nlohmann::json getJSONValue() const;
private:
	/** Key. */
	std::string key;

	/** Store. */
	std::string store;

	/** Resource. */
	std::string resource;

	/** Hash. */
	std::string hash;

	/** Hash type. */
	std::string hashType;
};

inline void LibraryEntry::setKey(const std::string& key){
	this->key = key;
}

inline const std::string& LibraryEntry::getKey() const{
	return key;
}

inline void LibraryEntry::setStore(const std::string& store){
	this->store = store;
}

inline const std::string& LibraryEntry::getStore() const{
	return store;
}

inline void LibraryEntry::setResource(const std::string& resource){
	this->resource = resource;
}

inline const std::string& LibraryEntry::getResource() const{
	return resource;
}

inline void LibraryEntry::setHash(const std::string& hash){
	this->hash = hash;
}

inline const std::string& LibraryEntry::getHash() const{
	return hash;
}

inline void LibraryEntry::setHashType(const std::string& hashType){
	this->hashType = hashType;
}

inline const std::string& LibraryEntry::getHashType() const{
	return hashType;
}

inline nlohmann::json LibraryEntry::getJSONValue() const{
	nlohmann::json libraryEntry;
	if(store.compare("") != 0)
		libraryEntry["store"] = store;
	if(resource.compare("") != 0)
		libraryEntry["resource"] = resource;
	if(hash.compare("") != 0)
		libraryEntry["hash"] = hash;
	if(hashType.compare("") != 0)
		libraryEntry["hashType"] = hashType;

	return libraryEntry;
}

};	//End of namespace Ygg

#endif
