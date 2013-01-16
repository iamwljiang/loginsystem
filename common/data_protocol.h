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
     
#define CLIENT_KEY_APPLY_TGT	0x01
#define CLIENT_AC_TGT           0x02        
#define CLIENT_SAC_APPLY_TICKET	0x04

#define is_client_type(t) bool check(short t){ if(t == CLIENT_KEY_APPLY_TGT || \
t == CLIENT_SAC_APPLY_TICKET || t == CLIENT_AC_TGT) return true;\else return false;}

//#define SERVER_

#define AC_REPLY_TGT			0x01
#define AC_REPLY_SAC            0x02
#define AC_REPLY_TICKET			0x04
#define AC_REPLY_ERROR			0x08

#define is_ac_type(t) bool check(short t){ if(t == AC_REPLY_ERROR || \
t == AC_REPLY_TICKET || t == AC_REPLY_TGT || t == AC_REPLY_SAC) return true; \ else return false;}


#endif //LS_DATA_PROTOCOL_H_