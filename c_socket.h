#pragma once
#include "includes.h"

#include "constants.h"




namespace p2net {
	namespace core {
		class c_socket {
		public:
			inline c_socket(){}

			inline c_socket(std::string ip, uint16_t p) : target_server_ip(ip), port(p), sock_type(socket_type::client) {}

			inline c_socket(uint16_t p) : target_server_ip("127.0.0.1"), port(p), sock_type(socket_type::server) {}

			inline c_socket(SOCKET csock, socket_type stype) : sock(csock), sock_type(stype) {}

			bool connect_to_server();


			bool server_begin_listening();


			void send_bytes(int8_t* bytes, size_t size);

			SOCKET get_socket() { return sock; }

			void close_socket() { closesocket(sock); sock = 0; };


			socket_type get_socket_type() { return sock_type; }



		private:
			std::string target_server_ip = "";

			uint16_t port = 0;


			socket_type sock_type = socket_type::none;

			SOCKET sock = 0;
		};
	}
}
