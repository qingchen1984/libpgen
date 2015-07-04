#include <stdio.h>
#include "pgen.h"
#include "address.h"
#include "packet.h"
#include "mytypes.h"
#include "netutil.h"


pgen_ardrone::pgen_ardrone(){
	CLEAR();
}


void pgen_ardrone::CLEAR(){
	pgen_udp::CLEAR();

	ARDRONE.pcmd.seq   = 0;
	ARDRONE.pcmd.flag  = 0;
	ARDRONE.pcmd.roll  = 0;
	ARDRONE.pcmd.pitch = 0;
	ARDRONE.pcmd.gaz   = 0;
	ARDRONE.pcmd.yaw.x = 0;
	ARDRONE.pcmd.yaw.y = 0;
	ARDRONE.pcmd.yaw.z = 0;
	ARDRONE.ref.seq        = 0;
	ARDRONE.ref.command    = 0;
}



void pgen_ardrone::SEND(const char *ifname){
	WRAP();		
	int sock;
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = IP.dst._addr;

	if((sock=initRawSocket(ifname, 3)) < 0)
		exit(-1);
	if(sendRawPacket(sock, data, len, 3, (struct sockaddr*)&addr) < 0)
		exit(PGEN_ERROR);

	close(sock);
}



void pgen_ardrone::CAST(const bit8* packet, int len){
	pgen_udp::CAST(packet, len);
	
	char buf[256];
	packet += sizeof(struct MYETH)+sizeof(struct MYIP)+sizeof(struct MYUDP);
	len -= sizeof(struct MYETH)+sizeof(struct MYIP)+sizeof(struct MYUDP);
	

	
	for(int i=0; i<len; i++){
		if(packet[i] == 0x0d)	buf[i] = '.';
		else					buf[i] = packet[i];
	}buf[len] = '\0';



	sscanf(buf, "AT*PCMD_MAG=%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld.AT*REF=%ld,%ld.",
			&ARDRONE.pcmd.seq, &ARDRONE.pcmd.flag, &ARDRONE.pcmd.roll,
			&ARDRONE.pcmd.pitch, &ARDRONE.pcmd.gaz, &ARDRONE.pcmd.yaw.x,
			&ARDRONE.pcmd.yaw.y, &ARDRONE.pcmd.yaw.z,
			&ARDRONE.ref.seq, &ARDRONE.ref.command );

}




void pgen_ardrone::WRAP(){
	pgen_udp::WRAP();

	char cmd1[64];
	char cmd2[64];
	char *p1;
	char spliter = 0x0d;

	sprintf(cmd1, "AT*PCMD_MAG=%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld", 
			ARDRONE.pcmd.seq, ARDRONE.pcmd.flag, 
			ARDRONE.pcmd.roll, ARDRONE.pcmd.pitch, 
			ARDRONE.pcmd.gaz, ARDRONE.pcmd.yaw.x, 
			ARDRONE.pcmd.yaw.y, ARDRONE.pcmd.yaw.z);
	
	sprintf(cmd2, "AT*REF=%ld,%ld", 
			ARDRONE.ref.seq, ARDRONE.ref.command);

	p1 = cmd;
	memcpy(p1, cmd1, strlen(cmd1));
	p1 += strlen(cmd1);
	memcpy(p1, &spliter, 1);
	p1 += 1;
	memcpy(p1, cmd2, strlen(cmd2));
	p1 += strlen(cmd2);
	memcpy(p1, &spliter, 1);
	p1 += 1;
	clen = p1 - cmd;

	udp.len = htons(ntohs(udp.len) + clen);

	u_char* p = data;
	memcpy(p, &ip, sizeof ip);
	p += sizeof(struct iphdr);
	memcpy(p, &udp, sizeof udp);
	p += sizeof(struct MYUDP);
	memcpy(p, cmd, clen);
	p += clen;
	len = p-data;
}


void pgen_ardrone::INFO(){
	WRAP();
	pgen_udp::INFO();
	
	printf(" * AR Drone packet\n");
	printf("    - PCMD MAG\n");
	printf("         Sequence Num : %ld \n", ARDRONE.pcmd.seq);
	printf("         Flag         : %ld \n", ARDRONE.pcmd.flag);
	printf("         Roll         : %ld \n", ARDRONE.pcmd.roll);
	printf("         Pitch        : %ld \n", ARDRONE.pcmd.pitch);
	printf("         Gaz          : %ld \n", ARDRONE.pcmd.gaz);
	printf("         Yaw(x,y,z)   : (%ld,%ld,%ld)  \n", 
			ARDRONE.pcmd.yaw.x, ARDRONE.pcmd.yaw.y,
			ARDRONE.pcmd.yaw.z);
	printf("    - REF\n");
	printf("         Sequence Num : %ld \n", ARDRONE.ref.seq);
	printf("         Command      : %ld \n", ARDRONE.ref.command);
	
}





void pgen_ardrone::SUMMARY(){
	WRAP();
	printf("AR Drone PCMD(seq=%ld flag=%ld roll=%ld pitch=%ld gaz=%ld yaw=(%ld,%ld,%ld)) ", 
			ARDRONE.pcmd.seq, ARDRONE.pcmd.flag, ARDRONE.pcmd.roll, 
			ARDRONE.pcmd.pitch, ARDRONE.pcmd.gaz,
			ARDRONE.pcmd.yaw.x, ARDRONE.pcmd.yaw.y,
			ARDRONE.pcmd.yaw.z);
	printf("REF(seq=%ld, cmd=%ld)\n", ARDRONE.ref.seq, ARDRONE.ref.command);
}


void pgen_ardrone::_printdata(){
	for(int i=0; i<clen; i++){
		if(cmd[i] == 0x0d)
			printf(".");
		else
			printf("%c", cmd[i]);
	}printf("\n");
}