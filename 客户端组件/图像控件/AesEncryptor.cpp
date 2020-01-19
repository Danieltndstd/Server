#include "StdAfx.h"
#include "AesEncryptor.h"

const unsigned char CAesEncryptor::m_keyt[16] = {'N','q','F','&','f','t','/','T', '2',
	'J','s','8','x','*','z'};

AES_KEY CAesEncryptor::m_aes_key;

bool CAesEncryptor::aes_encry(unsigned char* in_src, unsigned char* out_des)
{
	if (in_src==nullptr || out_des==nullptr) return false;

	int bSetSuc = AES_set_encrypt_key(m_keyt, 128, &m_aes_key);

	if (bSetSuc != 0) return false;

	AES_encrypt(in_src, out_des, &m_aes_key);

	return true;
}

bool CAesEncryptor::aes_decry(unsigned char* in_src, unsigned char* out_des)
{
	if (in_src==nullptr || out_des==nullptr) return false;

	int bSetSuc = AES_set_decrypt_key(m_keyt, 128, &m_aes_key);

	if (bSetSuc != 0) return false;

	AES_decrypt(in_src, out_des, &m_aes_key);

	return true;
}