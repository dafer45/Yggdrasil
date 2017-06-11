#ifndef COM_DAFER45_YGGDRASIL_CONFIGURATION
#define COM_DAFER45_YGGDRASIL_CONFIGURATION

#include "YGGMacros.h"

#include <iomanip>
#include <iostream>
#include <string>

#include <json.hpp>

namespace Ygg{

class Configuration{
public:
	/** Constructor. */
	Configuration();

	/** Configuration. */
	Configuration(const nlohmann::json& configuration);

	/** Destructor. */
	~Configuration();

	/** Load configuration. */
	void load();

	/** Set current library. */
	void setCurrentLibrary(const std::string& currentLibrary);

	/** Get current library. */
	const std::string& getCurrentLibrary() const;

	/** Get json. */
	nlohmann::json getJSON() const;

	/** Print. */
	void print() const;
private:
	/** Current library. */
	std::string currentLibrary;

	/** Ensure configuration file exists. */
	void ensureConfigurationFileExists();
};

inline void Configuration::setCurrentLibrary(
	const std::string& currentLibrary
){
	this->currentLibrary = currentLibrary;
}

inline const std::string& Configuration::getCurrentLibrary() const{
	return currentLibrary;
}

inline void Configuration::print() const{
	const int INDENT = 4;
	std::cout << std::left;
	std::cout << "Configuration:\n";
	std::cout << std::setw(INDENT) << "" << std::setw(20) << "Current library" << currentLibrary << "\n";
}

};	//End of namespace Ygg

#endif
