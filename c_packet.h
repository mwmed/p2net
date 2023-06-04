#pragma once
#include "c_array.h"
#include "packet_constants.h"
#include "constants.h"


namespace p2net {
	namespace core {
		namespace data {
			class c_packet_info {
			public:
				packet_state state = packet_state::waiting_body;

				bool is_header_valid = false;

				bool is_body_valid = false;

				bool is_handled = false;
			};


			class c_header {
			public:
				uint16_t magic_id = 77;
				uint64_t body_length;
			};



			class c_body {
			public:
				packet_core_opcode opcode = packet_core_opcode::none;

				c_array message;
			};
			class c_packet {
			public:
				inline c_packet(){}

			//	inline c_packet(const data::c_packet& obj){}

				c_packet_info info;

				c_header header;
				c_body body;

				c_array write() {
					c_array arr;

					arr.write<uint16_t>(header.magic_id);
					arr.write<uint64_t>(header.body_length);

					arr.write((uint8_t)body.opcode);
					arr.write(body.message.get_buffer(), body.message.get_buffer_length());

					return arr;
				}

				void fix_size() {
					header.body_length = 1 + body.message.get_buffer_length();
				}
			};
		}
	}
}