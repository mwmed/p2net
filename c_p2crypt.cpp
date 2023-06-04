#include "c_p2crypt.h"
#include <openssl/evp.h>
#include <openssl/pem.h>
#include "crc32.h"
#pragma warning(disable : 4996)

bool p2net::c_p2crypt::generate_key()
{
    rsa = RSA_new();
    if (!rsa)
        return false;

    BIGNUM* exponent = BN_new();
    if (!exponent)
    {
        RSA_free(rsa);
        return false;
    }

    BN_set_word(exponent, 65537);
    RSA_generate_key_ex(rsa, 1024, exponent, NULL);


    return true;
}

void p2net::c_p2crypt::generate_aes_rc4key()
{
    this->AES_KEY = new std::int8_t[16];
    RAND_bytes((unsigned char*)AES_KEY, 16);

    this->RC4_KEY = new std::int8_t[16];
    RAND_bytes((unsigned char*)RC4_KEY, 16);
}

p2net::core::data::c_array p2net::c_p2crypt::get_rsa_public_key()
{
    std::vector<uint8_t> derPublicKey;
    {
        derPublicKey.resize(140);
        uint8_t* keyPtr = derPublicKey.data();
        int keyLength;
        if (!(keyLength = i2d_RSAPublicKey(rsa, &keyPtr)))
        {
            return core::data::c_array();
        }
        derPublicKey.resize(keyLength);
    }


    return core::data::c_array((int8_t*)derPublicKey.data(), derPublicKey.size());
}

p2net::core::data::c_array p2net::c_p2crypt::encrypt(p2net::core::data::c_array ToEncrypt, bool& ret)
{
	ret = false;
	std::uint32_t padding = 16 - (ToEncrypt.get_buffer_length() + 1 + 4 + 2) % 16;
	core::data::c_array EncryptStruct;
	core::data::c_array  HashStruct;
	HashStruct.write(current_encrypt_counter);
	HashStruct.write(ToEncrypt.get_buffer(), ToEncrypt.get_buffer_length());
	EncryptStruct.write((std::int8_t)padding);
	EncryptStruct.write(Util::ComputeHash(HashStruct));
	EncryptStruct.write(HashStruct.get_buffer(), HashStruct.get_buffer_length());

	for (int i = 0; i < padding; i++)
		EncryptStruct.write<int8_t>((std::int8_t)0);


	++current_encrypt_counter;
	size_t es = 0;
	EVP_CIPHER_CTX* ctx;
	ctx = EVP_CIPHER_CTX_new();
	if (ctx == NULL)
	{


		return core::data::c_array();

	}
	if (EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, (unsigned char*)AES_KEY, NULL) == 0)
	{
		printf("Encrypt init failed\n");
		return core::data::c_array();
	}
	EVP_CIPHER_CTX_set_padding(ctx, false);
	BYTE* out = new BYTE[EncryptStruct.get_buffer_length()];
	int outSize2 = 0;
	if (EVP_EncryptUpdate(ctx, out, &outSize2, (unsigned char*)EncryptStruct.get_bytes(), EncryptStruct.get_buffer_length()) == 0)
	{
		printf("Encrypt update failed\n");

		return core::data::c_array();
	}
	es += outSize2;
	outSize2 = 0;
	if (EVP_EncryptFinal_ex(ctx, (unsigned char*)EncryptStruct.get_bytes() + es, &outSize2) == 0)
	{
		printf("Encrypt final failed\n");

		return core::data::c_array();
	}
	es += outSize2;
	EVP_CIPHER_CTX_free(ctx);

	ret = true;
	core::data::c_array  encrypted((int8_t*)out, es);
	delete[] out;
	return encrypted;
}

p2net::core::data::c_array p2net::c_p2crypt::decrypt(p2net::core::data::c_array ToDecrypt, bool& ret)
{
	core::data::c_array decrypted;
	EVP_CIPHER_CTX* ctx;
	ctx = EVP_CIPHER_CTX_new();
	ret = false;
	if (ctx == NULL)
	{

		return decrypted;
	}
	if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), 0, (unsigned char*)AES_KEY, 0))
	{

		return decrypted;
	}
	EVP_CIPHER_CTX_set_padding(ctx, false);

	std::int8_t* arr = new std::int8_t[ToDecrypt.get_buffer_length()];
	int ds = 0;
	size_t ds2 = 0;

	if (!EVP_DecryptUpdate(ctx, (unsigned char*)arr, &ds, (unsigned char*)ToDecrypt.get_buffer(), ToDecrypt.get_buffer_length()))
	{

		return decrypted;
	}
	ds2 += ds;
	ds = 0;
	if (!EVP_DecryptFinal_ex(ctx, (unsigned char*)(arr + ds2), &ds))
	{
		return decrypted;
	}
	ds2 += ds;
	EVP_CIPHER_CTX_free(ctx);
	ret = true;
	decrypted = core::data::c_array((int8_t*)arr, ds2);
	delete[] arr;
	return decrypted;
}

p2net::core::data::c_array p2net::c_p2crypt::rsadecrypt(p2net::core::data::c_array in, bool& result)
{
	unsigned char* aes_key_decrypted = new unsigned char[16];
	int ret = RSA_private_decrypt(in.get_buffer_length(), (unsigned char*)in.get_buffer(), aes_key_decrypted, rsa, RSA_PKCS1_OAEP_PADDING);
	result = ret != -1;
	return core::data::c_array((int8_t*)aes_key_decrypted, 16);
}

p2net::core::data::c_array p2net::c_p2crypt::rsaencrypt(p2net::core::data::c_array ToEncrypt)
{
	size_t outsize = 0;
	BYTE* ret = new BYTE[RSA_size(rsa)];
	outsize = RSA_public_encrypt(ToEncrypt.get_buffer_length(), (const unsigned char*)ToEncrypt.get_buffer(), (unsigned char*)ret, rsa, RSA_PKCS1_OAEP_PADDING);
	core::data::c_array encrypted((int8_t*)ret, outsize);
	delete[] ret;
	return encrypted;
}

p2net::core::data::c_array p2net::c_p2crypt::encrypt_key(p2net::core::data::c_array ToEncrypt, bool& ret)
{
	size_t es = 0;
	EVP_CIPHER_CTX* ctx;
	ctx = EVP_CIPHER_CTX_new();
	if (ctx == NULL)
	{

		return core::data::c_array();

	}
	if (EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, (unsigned char*)AES_KEY, NULL) == 0)
	{
		return core::data::c_array();
	}
	EVP_CIPHER_CTX_set_padding(ctx, false);
	BYTE* out = new BYTE[ToEncrypt.get_buffer_length()];
	int outSize2 = 0;
	if (EVP_EncryptUpdate(ctx, out, &outSize2, (unsigned char*)ToEncrypt.get_buffer(), ToEncrypt.get_buffer_length()) == 0)
	{
		return core::data::c_array();
	}
	es += outSize2;
	outSize2 = 0;
	if (EVP_EncryptFinal_ex(ctx, (unsigned char*)ToEncrypt.get_buffer() + es, &outSize2) == 0)
	{
		return core::data::c_array();
	}
	es += outSize2;
	EVP_CIPHER_CTX_free(ctx);

	ret = true;
	core::data::c_array encrypted((int8_t*)out, es);
	delete[] out;
	return encrypted;
}
