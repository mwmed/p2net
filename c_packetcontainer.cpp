#include "pch.h"
#include "c_packetcontainer.h"


void p2net::core::c_packetcontainer::on_data_received(std::uint64_t sock, data::c_array arr)
{
	
	
	data::c_array& buffer = m_received_data[sock];

	buffer.write(arr.get_buffer(), arr.get_buffer_length());

	if (!m_received_packets[sock])
		m_received_packets[sock] = new std::deque<data::c_packet>();
	while (1) {
		if (m_received_packets[sock]->empty() || m_received_packets[sock]->back().info.state == packet_state::completed) {

			m_received_packets[sock]->emplace_back();

			//	printf("Adding Empty Packet\n");
		}


		data::c_packet& awaiting_packet = m_received_packets[sock]->back();


		if (!awaiting_packet.info.is_header_valid ) {
			if (buffer.bytes_left() >= 10) {
				buffer.read<uint16_t>(awaiting_packet.header.magic_id);
				buffer.read<uint64_t>(awaiting_packet.header.body_length);

				awaiting_packet.info.is_header_valid = true;
			}
			else break;
			//	printf("Header is valid now \n");
		}


		//printf("Bytes left [%d] body_length [%d] \n", buffer.bytes_left(), awaiting_packet.header.body_length);

		if (awaiting_packet.info.is_header_valid && !awaiting_packet.info.is_body_valid) {

			if (buffer.bytes_left() >= awaiting_packet.header.body_length) {
				uint8_t opcode = 0;
				buffer.read<uint8_t>(opcode);
				awaiting_packet.body.opcode = (packet_core_opcode)opcode;

				uint64_t message_size = awaiting_packet.header.body_length - 1;

				if (message_size) {

					int8_t* message_buffer = new int8_t[message_size];

					buffer.read(message_buffer, message_size);

					auto bmessage = data::c_array(message_buffer, message_size);
					awaiting_packet.body.message.init(message_buffer, message_size);

					delete[] message_buffer;

					message_buffer = nullptr;
				}


				awaiting_packet.info.state = packet_state::completed;

				awaiting_packet.info.is_body_valid = true;

				buffer.remove_bytes(awaiting_packet.header.body_length + 10);
			}
			else break;

		}

		if (buffer.bytes_left() < 10) break;
	}
	
	


}
