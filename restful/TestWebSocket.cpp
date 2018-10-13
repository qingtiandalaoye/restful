

#include "TestWebSocket.h"


static TestWebSocket* global_md = nullptr;

static int ws_service_cb(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{

	switch (reason)
	{
	case LWS_CALLBACK_CLIENT_ESTABLISHED:
	{
		std::cout << "3.1415926 LWS_CALLBACK_CLIENT_ESTABLISHED callback client established, reason = " << reason << std::endl;
		lws_callback_on_writable(wsi);
		break;
	}
	case LWS_CALLBACK_PROTOCOL_INIT:
	{
		std::cout << "3.1415926 LWS_CALLBACK_PROTOCOL_INIT init, reason = " << reason << std::endl;
		break;
	}
	case LWS_CALLBACK_CLIENT_RECEIVE:
	{
		std::cout << "3.1415926 LWS_CALLBACK_CLIENT_RECEIVE on data, reason = " << reason << std::endl;
		if (global_md)
		{
			global_md->on_lws_data(wsi, (const char*)in, len);
		}
		break;
	}
	case LWS_CALLBACK_CLIENT_CLOSED:
	{
		std::cout << "3.1415926 LWS_CALLBACK_CLIENT_CLOSED, reason = " << reason << std::endl;
		if (global_md) {
			std::cout << "3.1415926 LWS_CALLBACK_CLIENT_CLOSED 2,  (call on_lws_connection_error)  reason = " << reason << std::endl;
			global_md->on_lws_connection_error(wsi);
		}
		break;
	}
	case LWS_CALLBACK_CLIENT_RECEIVE_PONG:
	{
		std::cout << "3.1415926 LWS_CALLBACK_CLIENT_RECEIVE_PONG, reason = " << reason << std::endl;
		break;
	}
	case LWS_CALLBACK_CLIENT_WRITEABLE:
	{
		std::cout << "3.1415926 LWS_CALLBACK_CLIENT_WRITEABLE writeable, reason = " << reason << std::endl;
		if (global_md)
		{
			global_md->lws_write_subscribe(wsi);
		}
		break;
	}
	case LWS_CALLBACK_TIMER:
	{
		std::cout << "3.1415926 LWS_CALLBACK_TIMER, reason = " << reason << std::endl;
		break;
	}
	case LWS_CALLBACK_CLOSED:
	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
	{
		std::cout << "3.1415926 LWS_CALLBACK_CLOSED/LWS_CALLBACK_CLIENT_CONNECTION_ERROR writeable, reason = " << reason << std::endl;
		if (global_md)
		{
			global_md->on_lws_connection_error(wsi);
		}
		break;
	}
	default:
		break;
	}

	return 0;
}

static struct lws_protocols protocols[] =
{
	{
		"md-protocol",
		ws_service_cb,
		0,
		65536,
	},
	{ NULL, NULL, 0, 0 } /* terminator */
};


enum protocolList {
	PROTOCOL_TEST,

	PROTOCOL_LIST_COUNT
};

struct session_data {
	int fd;
};



TestWebSocket::TestWebSocket()
{

}


void TestWebSocket::login(long timeout_nsec)
{
	std::cout << "TestWebSocket::login:" << std::endl;
	global_md = this;

	if (context == NULL) {
		struct lws_context_creation_info info;
		memset(&info, 0, sizeof(info));

		info.port = CONTEXT_PORT_NO_LISTEN;
		info.protocols = protocols;
		info.iface = NULL;
		info.ssl_cert_filepath = NULL;
		info.ssl_private_key_filepath = NULL;
		info.extensions = NULL;
		info.gid = -1;
		info.uid = -1;
		info.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
		info.max_http_header_pool = 1024;
		info.fd_limit_per_thread = 1024;
		info.ws_ping_pong_interval = 10;
		info.ka_time = 10;
		info.ka_probes = 10;
		info.ka_interval = 10;

		context = lws_create_context(&info);
		std::cout << "TestWebSocket::login: context created." << std::endl;
	}

	if (context == NULL) {
		std::cout << "TestWebSocket::login: context is NULL. return" << std::endl;
		return;
	}

	int logs = LLL_NOTICE| LLL_ERR | LLL_DEBUG | LLL_WARN;
	lws_set_log_level(logs, NULL);

	struct lws_client_connect_info ccinfo = { 0 };

	//ws://121.40.165.18:8800
	//static std::string host = "121.40.165.18";
	//static std::string path = "/";
	//static int port = 8800;

	//ws://123.207.167.163:9010/ajaxchattest
	static std::string host = "123.207.167.163";
	static std::string path = "/ajaxchattest";
	static int port = 9010;

	ccinfo.context = context;
	ccinfo.address = host.c_str();
	ccinfo.port = port;
	ccinfo.path = path.c_str();
	ccinfo.host = host.c_str();
	ccinfo.origin = host.c_str();
	ccinfo.ietf_version_or_minus_one = -1;
	ccinfo.protocol = protocols[0].name;


	//ws://
	ccinfo.ssl_connection = LCCSCF_ALLOW_SELFSIGNED | LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK;
	//wss://
	//ccinfo.ssl_connection = LCCSCF_USE_SSL | LCCSCF_ALLOW_SELFSIGNED | LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK;

	struct lws *wsi = NULL;
	wsi = lws_client_connect_via_info(&ccinfo);
	std::cout << "TestWebSocket::login: Connecting to " << ccinfo.host << ":" << ccinfo.port << ":" << ccinfo.path << std::endl;

	if (wsi == NULL) {
		std::cout << "TestWebSocket::login: wsi create error." << std::endl;
		return;
	}
	std::cout << "TestWebSocket::login: wsi create success." << std::endl;


	//while (1)
	{
		//int n = lws_service(context, 1000);
		//std::cout << " 3.1415 loop() lws_service (n)" << n << std::endl;

	}
}




void TestWebSocket::on_lws_data(struct lws* conn, const char* data, size_t len)
{
	std::cout << "on_lws_data:" << data << std::endl;
}



void TestWebSocket::on_lws_connection_error(struct lws* conn)
{
	std::cout << "on_lws_connection_error" << conn << std::endl;
	login(1);
}


int TestWebSocket::lws_write_subscribe(struct lws* conn)
{
	std::cout << "lws_write_subscribe" << conn << std::endl;
	unsigned char msg[512];
	memset(&msg[LWS_PRE], 0, 512 - LWS_PRE);

	std::string jsonString = "hello";


	size_t length = jsonString.length();

	strncpy((char *)msg + LWS_PRE, jsonString.c_str(), length);
	int ret = lws_write(conn, &msg[LWS_PRE], length, LWS_WRITE_TEXT);


	lws_callback_on_writable(conn);


	return ret;
}




void TestWebSocket::set_reader_thread()
{
	ws_thread = new std::thread(&TestWebSocket::loop, this);
}



void TestWebSocket::loop()
{
	while (1)
	{
		int n = lws_service(context, 1000);
		//std::cout << " 3.1415 loop() lws_service (n)" << n << std::endl;

	}

}