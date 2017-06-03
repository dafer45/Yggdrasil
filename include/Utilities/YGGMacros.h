/* Copyright 2017 Kristofer Björnson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @package TBTKcalc
 *  @file TBTKMacros.h
 *  @brief Precompiler macros
 *
 *  @author Kristofer Björnson
 */

#ifndef COM_DAFER45_YGGDRASIL_MACRO
#define COM_DAFER45_YGGDRASIL_MACRO

#include <cstring>
#include <sstream>

#define YGGAssert(expression, onFail)	\
	if(!(expression)){	\
		onFail;	\
	}

#define YGGWhere std::string(__FILE__) + ", " + std::to_string(__LINE__)

/*#define YGGAssert(expression, function, exception, message, hint)	\
	if(!(expression)){	\
		throw exception(	\
			function,	\
			message,	\
			hint,	\
			std::string(__FILE__) + ", " + std::to_string(__LINE__)	\
		);	\
	}*/

/*#define YGGExit(function, message, hint)	\
	TBTK::Streams::err << "Error in " << function << "\n";	\
	TBTK::Streams::err << "\t" << message << "\n";	\
	std::stringstream hintStream;	\
	hintStream << hint;	\
	if(std::strcmp(hintStream.str().c_str(), "") != 0)	\
		TBTK::Streams::err << "\tHint: " << hint << "\n";	\
	TBTK::Streams::err << "\tWhere: " << __FILE__ << ", " << __LINE__ << "\n";	\
	if(TBTK::Streams::logIsOpen())	\
		TBTK::Streams::closeLog();	\
	exit(1);

#define YGGNotYetImplemented(function)	\
	TBTK::Streams::err << "Error in " << function << "\n";	\
	TBTK::Streams::err << "\tNot yet implemented.\n";	\
	TBTK::Streams::err << "\tWhere: " << __FILE__ << ", " << __LINE__ << "\n";	\
	if(TBTK::Streams::logIsOpen())	\
		TBTK::Streams::closeLog();	\
	exit(1);*/

#endif
