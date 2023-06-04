#pragma once
#include "c_array.h"









namespace p2net {
	namespace client {
		class notify_csencryptedsessionkey_message {
		public:
			inline notify_csencryptedsessionkey_message(){}

			inline notify_csencryptedsessionkey_message(core::data::c_array key) {
				aes_key.init(key.get_buffer(), key.get_buffer_length());
			}

			core::data::c_array aes_key;



			core::data::c_array write() {
				core::data::c_array arr;
				arr.write_scalar(aes_key.get_buffer_length());
				arr.write(aes_key.get_buffer(), aes_key.get_buffer_length());
				return arr;
			}


			bool read(core::data::c_array obj) {
				
			}
		};
	}
}