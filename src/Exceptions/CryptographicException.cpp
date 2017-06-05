#include "CryptographicException.h"

namespace Ygg{

CryptographicException::CryptographicException(
	const std::string& function,
	const std::string& where,
	const std::string& message,
	const std::string& hint
) : Exception(function, where, message, hint){
}

CryptographicException::~CryptographicException(){
}

};	//End of namespace Ygg
