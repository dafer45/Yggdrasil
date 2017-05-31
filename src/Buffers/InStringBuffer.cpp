#include "InStringBuffer.h"

using namespace std;

namespace Ygg{

InStringBuffer::InStringBuffer(const string& str){
	buffer = new char[str.size()];
	for(unsigned int n = 0; n < str.size(); n++)
		buffer[n] = str[n];
	char* start = buffer;
	char* end = start + str.size();
	setg(start, start, end);
}

InStringBuffer::~InStringBuffer(){
	delete [] buffer;
}

};	//End of namespace Ygg
