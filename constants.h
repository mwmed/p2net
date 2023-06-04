#pragma once



#define DEBUG_LOGS 1

enum class socket_type {
	none,
	server,
	client,
	proxy

};

enum class packet_state {
	none,
	waiting_header,
	waiting_body,
	completed
};

enum class send_type {
	none,
	start = 42,
	normal,
	compressed,
	encrypted
};