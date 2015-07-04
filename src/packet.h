#ifndef PGEN_PACK_H
#define PGEN_PACK_H


#include "pgen.h"
#include "packconf.h"
#include "address.h"
#include "mytypes.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <iostream>


#define PGEN_PACKLEN 2000





class pgen_packet{
	protected:
		int 	packetType;
		int 	len;
		u_char 	data[PGEN_PACKLEN];
	public:
			
		pgen_packet();
		virtual void CLEAR();
		virtual void INFO()=0;	
		virtual void WRAP()=0;
		virtual void SEND(const char* ifname)=0;
		virtual void CAST(const bit8*, int)=0;
		void hex();
		void hexFull();
};





//#include <net/ethernet.h> 		/* for struct ether_header */
class pgen_eth : public pgen_packet {
	protected:
		struct MYETH eth; 
	public:
		static const int minLength = sizeof(struct MYETH);
		static const int macLength = sizeof(struct MYETH);
		struct{
			int type;
			macaddr src;
			macaddr dst;
		}ETH;
		
		pgen_eth();
		void CLEAR();
		void INFO();
		void WRAP();
		void SEND(const char* ifname);
		void CAST(const bit8*, int len);
};




//#include <netinet/if_ether.h>	/* for struct ether_arp 	*/
class pgen_arp : public pgen_eth {
	protected:
		struct MYARP arp;
	public:
		static const int minLength = sizeof(struct MYARP);
		static const int macLength = sizeof(struct MYARP);
		struct{
			int operation;
			macaddr	srcEth;
			macaddr	dstEth;
			ipaddr	srcIp;
			ipaddr	dstIp;
		}ARP;

		pgen_arp();
		void CLEAR();
		void INFO();
		void WRAP();
		void SEND(const char* ifname);
		void SUMMARY();
		void CAST(const bit8*, int len);
};



//#include <netinet/ip.h>			/* for struct iphdr			*/
class pgen_ip : public pgen_eth {
	protected:
		struct MYIP		ip;
	public:
		static const int minLength = sizeof(struct MYIP);
		static const int macLength = sizeof(struct MYIP);
		struct{
			int protocol;
			ipaddr src;
			ipaddr dst;
			int tos; 
			int id; 
			int ttl; 
		}IP;


		pgen_ip();
		void CLEAR();
		void INFO();
		void WRAP();
		void SEND(const char* ifname);
		void CAST(const bit8*, int len);
};




//#include <netinet/ip_icmp.h>	/* for struct icmp			*/
class pgen_icmp : public pgen_ip {
	protected:
		struct MYICMP icmp;
		u_char _data[100]; // no use yet
	public:
		static const int minLength = sizeof(struct MYICMP);
		static const int macLength = sizeof(struct MYICMP)+100;//??
		struct{	
			int option;
			int code;
			int id;
			int seq;
		}ICMP;
		
		pgen_icmp();
		void CLEAR();
		void INFO();
		void WRAP();
		void SEND(const char* ifname);
		void SUMMARY();
		void CAST(const bit8*, int len);

 };






//#include <netinet/tcp.h>		// for struct tcp		
class pgen_tcp : public pgen_ip {
	protected:
		struct MYTCP tcp;
		u_char _data[100]; // no use yet
	public:
		static const int minLength = sizeof(struct MYTCP);
		static const int macLength = sizeof(struct MYTCP)+100;//??
		struct{
			int src;
			int dst;
			struct{
				char fin;
				char syn;
				char rst;
				char psh;
				char ack;
				char urg;
			}flag ;
			int window;
			int seq;
			int ack;
		}TCP;

		pgen_tcp();
		void CLEAR();
		void INFO();
		void WRAP();
		void SEND(const char* ifname);
		void SUMMARY();
		void CAST(const bit8*, int len);
};





//#include <netinet/udp.h>		// for struct udp		
class pgen_udp : public pgen_ip {
	protected:
		struct MYUDP udp;
		u_char _data[100]; // no use yet
	public:
		static const int minLength = sizeof(struct MYUDP);
		static const int macLength = sizeof(struct MYUDP)+100;//??
		struct{
			int src;
			int dst;
		}UDP;

		pgen_udp();
		void CLEAR();
		void INFO();
		void WRAP();
		void SEND(const char* ifname);
		void CAST(const bit8*, int len);
};



class pgen_dns :public pgen_udp {
	protected:
		struct MYDNSHDR dns;
	public:
		//static const int minLength = sizeof(struct MYETH);
		//static const int macLength = sizeof(struct MYETH);
		struct{
			u_int16_t id;
			u_int16_t flags;
			u_int16_t qdcnt;
			u_int16_t ancnt;
			u_int16_t nscnt;
			u_int16_t arcnt;
			std::string name;
			u_int16_t type;
			u_int16_t cls;
		}DNS;

		pgen_dns();
		void CLEAR();
		void INFO();
		void WRAP();
		void SEND(const char* ifname);
};



class pgen_ardrone : public pgen_udp {
	protected:
		char cmd[256];
		int   clen;
	public:
		static const int minLength = 39;
		static const int macLength = 100; //???
		struct{
			struct{
				long seq;
				long flag;
				long roll;
				long pitch;
				long gaz;
				struct{
					long x;
					long y;
					long z;
				}yaw;
			}pcmd;
			struct{
				long seq;
				long command;
			}ref;
		}ARDRONE;

		pgen_ardrone();
		void CLEAR();
		void WRAP();
		void SEND(const char* ifname);
		void CAST(const bit8*, int);
		void INFO();
		void SUMMARY();
		void _printdata();
};





#endif