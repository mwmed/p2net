#pragma once
#include "c_socket.h"
#include "c_packetcontainer.h"


namespace p2net {
	namespace core {

		class c_tcpclient {
		public:
			inline c_tcpclient(std::string ip, uint16_t p)
			{
				client_socket = new c_socket(ip, p);
			}

			bool connect(bool startthread = true);


		public:
			bool running() { return is_running; }
			void stop() { is_running = false; }

			c_socket* get_socket() { return client_socket; }

			void start_listeners();

		public:
			virtual void on_handle_packet(c_socket& client_socket, data::c_packet& message) = 0;

			virtual void on_disconnect() = 0;

		private:
			void packet_handler();
			void send_handler();
			void client_handler();

		private:

			c_packetcontainer container;
			c_socket* client_socket = nullptr;
			std::deque<data::c_array> packets_to_send;

			bool is_running = true;


			std::thread m_send_thread;
			std::thread m_packet_thread;

			std::thread m_client_thread;


			BYTE* read_buffer = 0;

		};
	}
}