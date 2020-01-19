#pragma once

#include "openssl/aes.h"


class CAesEncryptor
{
public:
	/*
	   @param: 
				in_src:  输入待加密流
				out_des: 输出已加密流
	 */
	static	bool aes_encry(unsigned char* in_src, unsigned char* out_des);
	
	/*
	   @param: 
				in_src:  输入待带密流
				out_des: 输出已解密流
	 */
	static  bool aes_decry(unsigned char* in_src, unsigned char* out_des);

private:
	static AES_KEY					m_aes_key;
	static const unsigned char		m_keyt[16];
};

