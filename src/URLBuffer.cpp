#include "URLBuffer.h"

#include <iostream>

#include <curl/curl.h>

using namespace std;

namespace Yggdrasil{

URLBuffer::URLBuffer(const std::string& url){
	this->url = url;
	buffer.push_back('H');
	buffer.push_back('e');
	buffer.push_back('l');
	buffer.push_back('l');
	buffer.push_back('o');
	buffer.push_back(' ');
	buffer.push_back('w');
	buffer.push_back('o');
	buffer.push_back('r');
	buffer.push_back('l');
	buffer.push_back('d');
	buffer.push_back('!');
	buffer.push_back('\n');

	CURL *easyHandle = curl_easy_init();
	curl_easy_setopt(easyHandle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(easyHandle, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(easyHandle, CURLOPT_WRITEDATA, this);

	CURLcode curlCode = curl_easy_perform(easyHandle);
	switch(curlCode){
	case CURLE_OK:
		break;
	case CURLE_FILE_COULDNT_READ_FILE:
		cout << "Error in Resource::read()";
		cout << "Unable to open file.",
		exit(1);
        default:
		cout << "Error in Resource::read()\n";
		cout << "Failed to read with CURL error code " << curlCode << ".";
		exit(1);
	}

	char *start = &buffer.front();
	char *end = &buffer.front() + buffer.size();
	setg(start, start, end);
}

URLBuffer::~URLBuffer(){
}

size_t URLBuffer::writeCallback(
	char *data,
	size_t size,
	size_t nmemb,
	void *userData
){
	for(size_t n = 0; n < size*nmemb; n++)
		((URLBuffer*)userData)->buffer.push_back(data[n]);

	return size*nmemb;
}

};	//End of namespace Yggdrasil
