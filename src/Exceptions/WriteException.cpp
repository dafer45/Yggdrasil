#include "WriteException.h"

namespace Ygg{

WriteException::WriteException(
	const std::string& function,
	const std::string& where,
	const std::string& message,
	const std::string& hint
) : Exception(function, where, message, hint){
}

WriteException::~WriteException(){
}

};	//End of namespace Ygg
