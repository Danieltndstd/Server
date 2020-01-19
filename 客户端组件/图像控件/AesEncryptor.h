#pragma once

#include "openssl/aes.h"


class CAesEncryptor
{
public:
	/*
	   @param: 
				in_src:  �����������
				out_des: ����Ѽ�����
	 */
	static	bool aes_encry(unsigned char* in_src, unsigned char* out_des);
	
	/*
	   @param: 
				in_src:  �����������
				out_des: ����ѽ�����
	 */
	static  bool aes_decry(unsigned char* in_src, unsigned char* out_des);

private:
	static AES_KEY					m_aes_key;
	static const unsigned char		m_keyt[16];
};

