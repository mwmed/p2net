#pragma once
#include "c_socket.h"
#include "c_clientcontainer.h"
#include "c_packetcontainer.h"

namespace p2net {
	namespace core {

		class c_tcpserver {
		public:
			inline c_tcpserver() {

			}
			inline c_tcpserver(uint16_t p)
			{
				server_socket = new c_socket(p);
			}

			bool begin_listening(bool startthread = true);

		public:

			virtual void on_handle_packet(c_socket& client_socket, data::c_packet& message) = 0;

			virtual bool on_client_connected(c_socket& client_socket) = 0;

			virtual void on_client_connection_success(c_socket& client_socket) = 0;

			virtual void on_client_disconnected(std::uint64_t client_socket) = 0;

		public:
			bool running() { return is_running; }

			void stop() { is_running = false; }

			void send_to_client(c_socket client, data::c_array arr);

		
		private:
			void packet_handler();
			void send_handler();
			void client_handler();



		private:
			c_socket* server_socket = nullptr;


			c_clientcontainer container;
			c_packetcontainer packet_container;
			bool is_running = false;

			std::thread m_send_thread;
			std::thread m_packet_thread;

			std::thread m_client_thread;

			std::deque<std::pair<c_socket, data::c_array>> packets_to_send;

			BYTE* read_buffer = 0;

		};
	}
}