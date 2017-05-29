#include "InFileBuffer.h"

#include <iostream>

using namespace std;

namespace Ygg{

InFileBuffer::InFileBuffer(const std::string& filename) {
	open(filename, ios_base::in);
}

InFileBuffer::~InFileBuffer(){
	close();
}

};	//End of namespace Ygg
