#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <assert.h>
#include <string>
#include <chrono>
#include <thread>
#include <libwebsockets.h>



class TestWebSocket

{

public:

public:
	TestWebSocket();
	void login(long timeout_nsec);



	void on_lws_data(struct lws* conn, const char* data, size_t len);
	void on_lws_connection_error(struct lws* conn);
	int lws_write_subscribe(struct lws* conn);


	void set_reader_thread();
	void loop();

	std::thread* ws_thread;
	struct lws_context *context = nullptr;
};