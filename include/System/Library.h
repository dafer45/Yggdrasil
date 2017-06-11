#ifndef COM_DAFER45_YGGDRASIL_LIBRARY
#define COM_DAFER45_YGGDRASIL_LIBRARY

#include "LibraryEntry.h"

#include <istream>
#include <string>

#include <json.hpp>

namespace Ygg{

class Library{
public:
	/** Constructor. */
	Library();

	/** Destructor. */
	~Library();

	/** Load library. */
	void load();

	/** Load library from file. */
	void load(const LibraryEntry& libraryEntry);

	/** Write library. */
	void save() const;

	/** Add library entry. */
	void addLibraryEntry(const LibraryEntry& libraryEntry);

	/** Update library entry. */
	void updateLibraryEntry(const LibraryEntry& libraryEntry);

	/** Remove library entry. */
	void removeLibraryEntry(const LibraryEntry& libraryEntry);

	/** Link library. */
	void link(const Library& library);

	/** Public key. */
	void setPublicKey(const std::string& publicKey);

	/** Sign library. */
	void sign(const std::string& privateKey);

	/** Verify signature. */
	void verify(const std::string& publicKey) const;

	/** Return a JSON representation of the library. */
	const nlohmann::json& getJSON() const;

	/** Get library entry. */
	LibraryEntry getLibraryEntry(const std::string& key) const;
private:
	/** Actuall data. */
	nlohmann::json library;

	/** LibraryEntry storing the location of the library if it has been
	 *  loaded from an external resource. */
	LibraryEntry fileInfo;

	/** Read JSON. */
	nlohmann::json readJSON(const std::string& filename) const;

	/** Write JSON. */
	void writeJSON(const std::string& filename, const nlohmann::json& j) const;
};

inline const nlohmann::json& Library::getJSON() const{
	return library;
}

};	//End of namespace Ygg

#endif
