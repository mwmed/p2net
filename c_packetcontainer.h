#pragma once
#include "c_packet.h"






namespace p2net {
	namespace core {
		class c_packetcontainer {
		public:
			inline c_packetcontainer(){}


			void on_data_received(std::uint64_t sock, data::c_array arr);
			std::map<std::uint64_t, std::deque<data::c_packet>*> get_packets() { return m_received_packets; }

			void new_client(uint64_t c) {
				m_received_packets[c] = new std::deque<data::c_packet>();
			}
		private:
			std::map<std::uint64_t, std::deque<data::c_packet>*> m_received_packets;

			std::map<std::uint64_t, data::c_array> m_received_data;

			
		};
	}
}