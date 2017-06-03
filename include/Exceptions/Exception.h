#ifndef COM_DAFER45_YGGDRASIL_EXCEPTION
#define COM_DAFER45_YGGDRASIL_EXCEPTION

#include <exception>
#include <string>

namespace Ygg{

class Exception : public std::exception{
public:
	/** Constructor. */
	Exception(
		const std::string& function,
		const std::string& where,
		const std::string& message,
		const std::string& hint
	);

	/** Destructor. */
	virtual ~Exception();

	/** Overrider std::exception::what(). */
	virtual const char* what() const noexcept;

	/** Print exception to standard error output. */
	virtual void print() const;
private:
	/** Function throwing the exception. */
	std::string function;

	/** File name and line where the exception happened. */
	std::string where;

	/** Message describing what happened. */
	std::string message;

	/** Hint for how to resolve the problem. */
	std::string hint;
};

};	//End of namespace Ygg

#endif
