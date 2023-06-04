#pragma once
#include "c_array.h"






namespace p2net {
	namespace client {
		class c_rmimessage {
		public:
			inline c_rmimessage(){}

			inline c_rmimessage(std::uint16_t rmi, std::uint64_t length, core::data::c_array msg) {
				rmi_id = rmi;

				message.init(msg.get_buffer(), msg.get_buffer_length());
			}


			std::uint16_t rmi_id = 0;
			core::data::c_array message;


			bool read(core::data::c_array& arr) {
				bool result = false;

				if (arr.read<uint16_t>(rmi_id)) {

					auto len = arr.bytes_left();
					auto bytes = new std::int8_t[len];

					result = arr.read(bytes, len);

					message.init(bytes, len);

					delete[] bytes;
					bytes = nullptr;
				}


				return result;
			}

			core::data::c_array write() {
				core::data::c_array obj;

				obj.write<std::uint16_t>(rmi_id);

				obj.write(obj.get_buffer(), obj.get_buffer_length());

				return obj;
			}

		};
	}
}