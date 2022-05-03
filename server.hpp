#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h> 

struct sockaddr_in {
    short            sin_family;   // e.g: AF_INET
    unsigned short   sin_port;     // e.g: htons(3490)
    struct in_addr   sin_addr;     // détaillé ci-dessous
    char             sin_zero[8];  // '0' habituellement
};
struct in_addr {
    unsigned long s_addr;  // initialiser avec inet_aton()
};


class Serv
{
private:
    int sockfd;
public:
    const void *_optVal;
    socklen_t _optLen;
    int GetSockfd() const;
    void SetSockfd(int);
    Serv(/* args */);
    ~Serv();
};

Serv::Serv(/* args */)
{
}

Serv::~Serv()
{
}

void     Serv::SetSockfd(int i) 
{
    this->sockfd = i;
}


int     Serv::GetSockfd() const
{
    return(this->sockfd);
}