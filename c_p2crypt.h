#pragma once
#include <openssl/rsa.h>
#include <openssl/rand.h>
#include "c_array.h"





namespace p2net {
	class c_p2crypt {
	public:
		inline c_p2crypt(){}
		


		bool generate_key();

		void generate_aes_rc4key();

		p2net::core::data::c_array get_rsa_public_key();

		p2net::core::data::c_array  encrypt(p2net::core::data::c_array ToEncrypt, bool& ret);
		p2net::core::data::c_array  decrypt(p2net::core::data::c_array  ToDecrypt, bool& ret);
		p2net::core::data::c_array  rsadecrypt(p2net::core::data::c_array  in, bool& result);
		p2net::core::data::c_array  rsaencrypt(p2net::core::data::c_array  ToEncrypt);
		p2net::core::data::c_array  encrypt_key(p2net::core::data::c_array  ToEncrypt, bool& ret);


		RSA* rsa;
		std::int8_t* AES_KEY = 0;
		std::int8_t* RC4_KEY = 0;
		std::mutex mtx;

	private:
		size_t rsa_key_bits = 1024;
		EVP_PKEY_CTX* keyCtx;

		std::int16_t current_encrypt_counter = 0;
		std::int16_t current_decrypt_counter = 0;
	};
}