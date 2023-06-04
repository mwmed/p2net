#pragma once
#include "client_network_settings.h"





namespace p2net {
	namespace client {
		class notify_serverconnectionhint_message {
		public:
			inline notify_serverconnectionhint_message(){}

			inline notify_serverconnectionhint_message(core::data::c_array key) {
				session_key.init(key.get_buffer(), key.get_buffer_length());
			}

			client_network_settings settings;

			core::data::c_array session_key;


			core::data::c_array write() {
				core::data::c_array obj;

				auto settings_msg = settings.write();

				obj.write(settings_msg.get_buffer(), settings_msg.get_buffer_length());

				obj.write_scalar(session_key.get_buffer_length());

				obj.write(session_key.get_buffer(), session_key.get_buffer_length());


				return obj;
			}
		};
	}
}