#ifndef LS_ENANDDECRYPTION_H_
#define LS_ENANDDECRYPTION_H_

#include <iostream>
#include <string>

//openssl header file
#include <openssl/des.h>

class CEncryptAndDecrypt{
public:
	CEncryptAndDecrypt();
	
	~CEncryptAndDecrypt();

	char* DESEncrypt(const char* data,int len,const char* key);

	char* DESDecrpyt(const char* data,int len,const char* key);  

};

#endif //LS_ENANDDECRYPTION_H_
