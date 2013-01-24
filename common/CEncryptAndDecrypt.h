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

	char* Md5FromString(const char* buffer,int len, char* dist);

	char* Md5FromString(const char* buffer1,int len1,const char* buffer2,int len2,char* dist);

	char* Md5FromFile(const char* filename, char* dist);
};

#endif //LS_ENANDDECRYPTION_H_
