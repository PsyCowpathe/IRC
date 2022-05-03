#include "server.hpp"


int main()
{
    int i;
    Serv test;
    test.SetSockfd(socket(AF_INET, SOCK_STREAM, 0));
    int u = setsockopt(test.GetSockfd(), 0, SO_KEEPALIVE, test._optVal, test._optLen);
    bind(test.GetSockfd(), )
}