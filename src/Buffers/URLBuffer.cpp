#include "ReadException.h"
#include "URLBuffer.h"
#include "YGGMacros.h"

#include <iostream>

#include <curl/curl.h>

using namespace std;

namespace Ygg{

URLBuffer::URLBuffer(const std::string& url){
	this->url = url;

	CURL *easyHandle = curl_easy_init();
	curl_easy_setopt(easyHandle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(easyHandle, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(easyHandle, CURLOPT_WRITEDATA, this);

	CURLcode curlCode = curl_easy_perform(easyHandle);
	switch(curlCode){
	case CURLE_OK:
		break;
	case CURLE_FILE_COULDNT_READ_FILE:
		throw ReadException(
			"URLBuffer::URLBuffer()",
			YGGWhere,
			"Unable to open file.",
			""
		);
        default:
		throw ReadException(
			"URLBuffer::URLBuffer()",
			YGGWhere,
			"Failed to read with CURL error code " + to_string(curlCode) + ".",
			""
		);
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

};	//End of namespace Ygg
