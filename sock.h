/*
 * Gracefully copied from "Unix System Programming using C++ By Terence Chan".
 * A very beautiful book. 
*/

#ifndef SOCK_H
#define SOCK_H
#endif

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/sysinfo.h>

const int BACKLOG_NUM = 5;

class sock {
    private:
    int sid;        // socket descriptor
    int domain;     // socket domain
    int socktype;   // socket type
    int rc;         // member function return status code

    /* Build an Internet socket name based on a hostname and a port number */
    int constr_name(struct sockaddr_in &addr, const char* hostnm, int port) {
        addr.sin_family = domain;
        if(!hostnm) {
            addr.sin_addr.s_addr = INADDR_ANY;
        } else {
            struct hostent *hp = gethostbyname(hostnm);
            if(hp == 0) {
                perror("gethostname");
                return -1;
            }
            memcpy((char*)&addr.sin_addr, (char *)hp->h_addr, hp->h_length);
        }
        addr.sin_port = port;
        return sizeof(addr);
    }
    /* Build a UNIX domain socket name based on a pathname */
    int constr_name(struct sockaddr& addr, const char* Pathnm) {
        addr.sa_family= domain;
        strcpy(addr.sa_data, Pathnm);
        return sizeof(addr.sa_family) + strlen(Pathnm) + 1;
    }

    /* Convert an IP Address to a character string host name */

    char *ip2name(const struct in_addr in) {
        u_long laddr;
        if((int)(laddr = inet_addr(inet_ntoa(in))) == -1) {
            return 0;
        }
        struct hostent *hp = gethostbyaddr((char *)&laddr, sizeof(laddr), AF_INET);
        if(!hp) {
            return 0;
        }
        for(char **p = hp->h_addr_list; *p != 0; p++) {
            if(hp->h_name) return hp->h_name;
        }
        return 0;
    }
    public:
    /* sock object constructor function */
    sock(int dom, int type, int protocol=0) : domain(dom), socktype(type)
    {
        if((sid = socket(dom, type, protocol)) < 0) {
            perror("socket");
        }
    }

    /* sock object destructor function */
    ~sock() {
        shutdown(sid, SHUT_RDWR);
        close(sid);
    }

    int fd() {
        return sid;
    }

    int good() {
        return (sid >= 0);
    }

    /* assign a UNIX or an Internet name to a socket */
    int bind(const char* name, int port=-1) {
        if(port == -1) {
            struct sockaddr addr;
            int len = constr_name(addr, name);
            if((rc=::bind(sid, &addr, len)) < 0) {
                perror("bind");
            }
        } else {
            struct sockaddr_in addr;
            int len = constr_name(addr, name, port);
            if((rc=::bind(sid, (struct sockaddr *)&addr, len)) < 0 || (rc=getsockname(sid, (struct sockaddr*)&addr, &len)) < 0) {
                perror("bind or getsockname");
            }
            else {
                std::cout << "Socket port:" << ntohs(addr.sin_port) << std::endl;
            }
        }
        /* setup connection backlog threshold for a STREAM socket */
        if(rc != -1 && socktype != SOCK_DGRAM && (rc = listen(sid, BACKLOG_NUM)) < 0) {
            perror("listen");
        }
        return rc;
    }
};