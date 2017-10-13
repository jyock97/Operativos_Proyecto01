#ifndef NET_CONTROLLER_H
#define NET_CONTROLLER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
void makeConnection();

void *netController();

void sedMessage(char *msg);

#endif
