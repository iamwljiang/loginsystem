#ifndef LS_DATA_PROTOCOL_H_
#define LS_DATA_PROTOCOL_H_

/*
packet struct:
2个字节标识包的类型 + 2个字节标识包的长度 + 后面数据协议格式可以自定义
*/

/*
AC表示 authentication center
SAC表示 session auth and client
*/

//TYPE|LEN|USERNAME|SECRET{des("username:loginserver ip",half(hash(md5(password))))}|SIGNATRUE{md5(SECRET+half(hash(md5(password))))}
#define CLIENT_KEY_APPLY_TGT	0x01
//TYPE|LEN|SECRET(des("username:start_time:sac_key",auth_key) )
#define CLIENT_AC_TGT           0x02
//TYPE|LEN|SECRET(des("username",sac_key) )
#define CLIENT_SAC_APPLY_TICKET	0x04

#define is_client_type(t) bool check(short t){ if(t == CLIENT_KEY_APPLY_TGT || \
t == CLIENT_SAC_APPLY_TICKET || t == CLIENT_AC_TGT) return true;\else return false;}

//#define SERVER_

#define AC_TRY_MAX_VERIFY		20		

#define AC_REPLY_TGT			0x01
#define AC_REPLY_SAC            0x02
#define AC_REPLY_TICKET			0x04
#define AC_REPLY_SSC 			0x08
#define AC_REPLY_ERROR			0x10

#define is_ac_type(t) bool check(short t){ if(t == AC_REPLY_ERROR || \
t == AC_REPLY_TICKET || t == AC_REPLY_TGT || t == AC_REPLY_SAC || t == AC_REPLY_SSC ) return true; \ else return false;}

#define is_avalid_len(t) bool check(short t){ return (t > 0 && t < 1024*4);}
#define is_avalid_type(t) bool check(short t){ return (t > 0 && t < 65535);}



#endif //LS_DATA_PROTOCOL_H_