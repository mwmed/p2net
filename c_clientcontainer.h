#pragma once
#include "c_socket.h"






namespace p2net {
	namespace core {
		class c_clientcontainer {
		public:
			inline c_clientcontainer(){}


			void on_connected(c_socket sock) {
				auto sock_id = sock.get_socket();
#if DEBUG_LOGS
				printf("c_clientcontainer::on_connected | New client connected! [%d] adding to container.\n", sock_id);
#endif

				m_clients[sock_id] = sock;
			}

			void on_disconnected(c_socket sock) {
				auto sock_id = sock.get_socket();
#if DEBUG_LOGS
				printf("c_clientcontainer::on_connected | Client disconnected [%d] removing from container.\n", sock_id);
#endif
				if(m_clients.find(sock_id) != m_clients.end())
					m_clients.erase(sock_id);
			}
			void on_disconnected(std::uint64_t sock_id) {
#if DEBUG_LOGS
				printf("c_clientcontainer::on_connected | Client disconnected [%d] removing from container.\n", sock_id);
#endif
				if (m_clients.find(sock_id) != m_clients.end()) {
					auto m_sock = m_clients[sock_id];

					m_sock.close_socket();
					m_clients.erase(sock_id);
				}
			}

			c_socket get_client(std::uint64_t m_socket) {
				if (m_clients.find(m_socket) == m_clients.end()) {
#if DEBUG_LOGS
					printf("c_clientcontainer::get_client | Failed to remove client from container [%d]. (Client doesnt exist.) \n", m_socket);

#endif

					return c_socket(0);
				}

				return m_clients[m_socket];
			}

		private:
			std::map<std::uint64_t, c_socket> m_clients;
		};
	}
}