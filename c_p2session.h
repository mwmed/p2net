#pragma once
#include "c_p2crypt.h"
#include "c_socket.h"
#include "p2client.h"






namespace p2net {
	class c_p2session 
	{
	public:
		inline c_p2session()
		{

		}

		inline c_p2session(core::c_socket s)
		{
			sock = s;
		}

	public:

		void send_raw_bytes(core::data::c_array arr) {

			sock.send_bytes(arr.get_buffer(), arr.get_buffer_length());

		}

		void send_core(packet_core_opcode opcode, core::data::c_array arr) {
			auto packet = p2net::p2client::make_packet(opcode, arr);

			auto packet_bytes = packet.write();

			sock.send_bytes(packet_bytes.get_buffer(), packet_bytes.get_buffer_length());
		}


		core::data::c_array get_encrypted_struct(packet_core_opcode opcode, core::data::c_array arr, bool& res) {

			res = false;
			core::data::c_array encrypted_core;

			if (is_crypt_initialized()) {
				core::data::c_array core_struct;

				core_struct.write((uint8_t)opcode);

				core_struct.write(arr.get_buffer(), arr.get_buffer_length());


				auto encrypted_data = client_crypt->encrypt(core_struct, res);
				if (res) {

					encrypted_core.write((std::int8_t)1);
					encrypted_core.write_scalar(encrypted_data.get_buffer_length());
					encrypted_core.write(encrypted_data.get_buffer(), encrypted_data.get_buffer_length());
				}
			}
			return encrypted_core;
		}

		void send_encrypted(packet_core_opcode opcode, core::data::c_array arr) {

			bool res = false;
			auto encryped = get_encrypted_struct(opcode, arr, res);
			if(res)
				send_core(packet_core_opcode::encrypyted_reliable, encryped);

			/*
			if (is_crypt_initialized()) {


				core::data::c_array core_struct;

				core_struct.write((uint8_t)opcode);

				core_struct.write(arr.get_buffer(), arr.get_buffer_length());

				bool res = false;

				auto encrypted_data = client_crypt->encrypt(core_struct, res);
				if (res) {


					core::data::c_array encrypted_core;

					encrypted_core.write((std::int8_t)1);
					encrypted_core.write_scalar(encrypted_data.get_buffer_length());
					encrypted_core.write(encrypted_data.get_buffer(), encrypted_data.get_buffer_length());

					send_core(packet_core_opcode::encrypyted_reliable, encrypted_core);

				}

#if DEBUG_LOGS
				if (!res)
					printf("c_p2session::send_encrypted | Failed to encrypt data! \n");
#endif


			}

			*/
		}

		void send_rmi(std::uint16_t rmi_id, core::data::c_array message, send_type type = send_type::none)
		{
			core::data::c_array obj;

			obj.write(rmi_id);
			obj.write(message.get_buffer(), message.get_buffer_length());

			switch (type) {
			case send_type::normal:
				send_core(packet_core_opcode::rmi, obj);
				break;
			case send_type::encrypted:
				send_encrypted(packet_core_opcode::rmi, obj);
				break;
			case send_type::compressed:
				break;
			}
		}


	public:

		void set_socket(core::c_socket g_basesock) {
			sock = g_basesock;
		}
		bool is_crypt_initialized() {//
			return client_crypt != 0 && client_crypt->AES_KEY;
		}

		c_p2crypt* get_crypt() { return client_crypt; }


		void new_crypt() {
			client_crypt = new c_p2crypt();
		}

		void erase_crypt() {
			if (client_crypt) {
				if(client_crypt->AES_KEY)
					delete[] client_crypt->AES_KEY;
				if(client_crypt->RC4_KEY)
					delete[] client_crypt->RC4_KEY;

				delete client_crypt;
				client_crypt = nullptr;
			}
		}

		void disconnect() {
			erase_crypt();

			sock.close_socket();
		}
	private:
		core::c_socket sock;

		c_p2crypt* client_crypt = 0;
	};
}