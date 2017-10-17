#include "httpRequestHandler.h"
#include "socket.h"

//--------------AQUI VAN FUNCIONES PARA PROCESAR LOS PROTOCOLOS-------------------
char *ftp_protocol(){
	char *response;
	response = "\n\nConnected to 127.0.0.1\nUser ((127.0.0.1):(none)) : \nPlease specify the password: \nftp>\n";
	return response;
}

char *smtp_protocol(){
	char *response;
	response = "\n\n200 localhost My SMTP MAIL Service, Version:\n1.3 ready at Today\n";
	return response;
}

char *dns_protocol(){
	char *response = (char *)calloc(2000,sizeof(char));
	strcat(response, "\n\nDomain Name System (query)\n");
	strcat(response, "\t[Response In: 1852]\n");
	strcat(response, "\tTransaction ID: 0x241a\n");
	strcat(response, "\tFlags: 0x0100 (Standard query)\n");
	strcat(response, "\t\t0... .... .... .... = Response: Message is a query\n");
	strcat(response, "\t\t.000 0... .... .... = Opcode: Standard query (0)\n");
	strcat(response, "\t\t.... ..0. .... .... = Truncated: Message is not truncated\n");
	strcat(response, "\t\t.... ...1 .... .... = Recursion desired: Do query recursively\n");
	strcat(response, "\t\t.... .... .0.. .... = Z: reserved (0)\n");
	strcat(response, "\t\t.... .... ...0 .... = Non-authenticated data OK: Non-authenticated data is unacceptable\n");
	strcat(response, "\tQuestions: 1\n");
	strcat(response, "\tAnswer RRs: 0\n");
	strcat(response, "\tAuthority RRs: 0\n");
	strcat(response, "\tAdditional RRs: 0\n");
	strcat(response, "\tQueries\n");
	strcat(response, "\t\t127.0.0.1: type A, class IN\n");
	strcat(response, "\t\t\tName: localHost\n");
	strcat(response, "\t\t\tType: A (Host address)\n");
	strcat(response, "\t\t\tClass: IN (0x0001)\n");
	return response;
}

char *telnet_protocol(){
	char *response;
	response = "\n\nTrying 127.0.0.1...\nConnected to localhost.\nEscape character is '^]'.\n";

	return response;
}

char *ssh_protocol(){
	char *response;
	response = "\n\n################################################\n#           WELCOME TO THE SSH SERVER          #\n#        THIS IS A SECURE CONNECTION           #\n#        ONLY FOR AUTORIZED PERSONAL           #\n#          IF NOT PLEASE EXIT NOW              #\n################################################\n";
	return response;
}

char *http_protocol(int socket, char *currentPath){
	char *response;
	char requestMsg[5000];
	recv(socket , requestMsg, 5000 , 0);
	printf("---------------------------------\n%s---------------------------------\n", requestMsg);
	response = request(requestMsg, currentPath);
	return response;
}

char *snmp_protocol(){
	char *response = (char*)calloc(200,sizeof(char));
	strcat(response, "Version: 1\r\n");
	strcat(response, "Community: public \r\n");
	strcat(response, "PDU type: GET\r\n");
	strcat(response, "Request Id: 0\r\n");
	strcat(response, "Error Status: NO ERROR\r\n");
	strcat(response, "Error Index: 0\r\n");

	return response;
}
//--------------AQUI VAN FUNCIONES PARA PROCESAR LOS PROTOCOLOS-------------------


char *accept_request(int socket, int port, char *currentPath){//Se discrimina el protocolo por el puerto y se le devuelve su respuesta
	char * response;
	switch(port){
		case 2020:
		case 2021:
			response = ftp_protocol();//listo
			break;
		case 2025:
			response = smtp_protocol();//listo
			break;
		case 2053:
			response = dns_protocol();//listo
			break;
		case 2023:
			response = telnet_protocol();//listo
			break;
		case 2022:
			response = ssh_protocol();//listo
			break;
		case 8080:
			response = http_protocol(socket, currentPath);//listo
			break;
		case 2161:
			response = snmp_protocol();//listo
			break;
		default:
			response = http_protocol(socket, currentPath);//listo
			break;
	}
	return response;
}