#include "pgen-packet.h"
#include "pgen-variable.h"


#include <map>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <net/ethernet.h> 
#include <netinet/ip.h>




pgen_ip::pgen_ip(){
	pgen_eth::clear();
	clear();	
}

void pgen_ip::clear(){
	ip_srcIp = 0;
	ip_dstIp = 0;
}


void pgen_ip::compile(const char* ifname){
	pgen_eth::compile(ifname);
	if(ip_srcIp.isEmpty()){
		ip_srcIp = pgen_getIP(ifname);
		printf(" - ip_srcIp is Empty. set %s's ip address(%s)\n", ifname, pgen_getIP(ifname));
	}

	packetType = PGEN_PACKETTYPE_IP;
	eth.ether_type = htons(ETHERTYPE_IP);
	memset(data, 0, sizeof data);
	memset(&ip, 0, sizeof ip);
	

	ip.ihl = sizeof(ip) / 4;
	ip.version = 4;
	ip.tos = 0; //no useing world now
	ip.tot_len = sizeof(ip);
	ip.id = random(); // ??????
	ip.frag_off = 0; // ?????
	ip.ttl = PGEN_DEFAULT_TTL;
	ip.protocol = IPPROTO_IP;
	ip.saddr = ip_srcIp._addr;
	ip.daddr = ip_dstIp._addr;
	ip.check = checksum(&ip, sizeof(ip));

	u_char* p = data;
	memcpy(p, &eth, sizeof eth);
	p += sizeof(eth);
	memcpy(p, &ip, sizeof ip);
	p += sizeof(ip);
	len = p - data;
}

void pgen_ip::info(){
	pgen_eth::info();
	
	std::map<int, const char*> _ipprot;
	_ipprot[0]  = "Not Set Empty IP Packet";
	_ipprot[1]  = "ICMP";
	_ipprot[2]  = "IGMP";
	_ipprot[4]  = "IPv4 on IP";
	_ipprot[6]  = "TCP";
	_ipprot[17] = "UDP";
	_ipprot[41] = "IPv6 on IP";

	printf(" * Internet Protocol version 4\n");
	printf("    - Version     :  %d \n", ip.version);
	printf("    - Source      :  %s \n", ip_srcIp.c_str());
	printf("    - Destination :  %s \n", ip_dstIp.c_str());
	printf("    - Protocol    :  %s (%u) \n", _ipprot[ip.protocol],  ip.protocol);
}
