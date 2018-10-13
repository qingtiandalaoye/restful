// restful.cpp : 定义控制台应用程序的入口点。
//
#include <iostream>
#include<iostream>
#include<string>
#include <curl/curl.h>    
#include "TestWebSocket.h"

using namespace std;

void testCurl() {

	CURL *curl = nullptr;
	CURLcode res;
	curl = curl_easy_init();
	if (curl != nullptr) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com");
		/* example.com is redirected, so we tell libcurl to follow redirection */
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}
		/* always cleanup */
		curl_easy_cleanup(curl);
	}
}





int main(int argc, char* argv[]) {
	//testCurl();

	TestWebSocket testws;
	testws.set_reader_thread();
	testws.login(1);
	
	//std::this_thread::sleep_for(std::chrono::milliseconds(10000000));

	char a;
	int b;
	float c;
	std::cin >> a >> b >> c;
	std::cout << a << " " << b << " " << c << " " << std::endl;

	system("pause");
	return 0;
}
