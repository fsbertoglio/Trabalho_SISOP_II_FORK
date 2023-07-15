#include "socketAPI.hpp"

SocketAPI::SocketAPI(int port, string sessionMode) {
    this->port = port;
    this->sessionMode = sessionMode;
    this->serverAddr = "";
    SocketAPI::createSocket();
}

SocketAPI::SocketAPI(int port, string serverAddr, string sessionMode) {
    this->port = port;
    this->sessionMode = sessionMode;
    this->serverAddr = serverAddr;
    SocketAPI::createSocket();
}

SocketAPI::~SocketAPI() {
    close(this->socketfd);
};

int SocketAPI::createSocket() {

    //Creates an endpoint for communication and return a file descriptor that refers to that endpoint.
    if (this->socketfd = socket(AF_INET, SOCK_STREAM, 0) < 0) {
        cerr << "SocketAPI>createSocket> error opening socket" << endl;
        return -1;
    }

    /*
    struct sockaddr_in {
        short            sin_family;   // e.g. AF_INET, AF_INET6
        unsigned short   sin_port;     // port number
        struct in_addr   sin_addr;     // internet address
        char             sin_zero[8];  
    };
    */ 
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(this->port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(serverAddr.sin_zero), 8);
    
    //Associates a local address with a socket and catch errors.
    if(bind(this->socketfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "SocketAPI>createSocket> error binding socket" << endl;
        return -1;
    }
}

int SocketAPI::listenSocket(packet_struct* packet) {
    int packetSize = sizeof(packet_struct);
    char buffer[1024];
    bzero(buffer, 1024);

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(struct sockaddr_in); 

    int n = recvfrom(this->socketfd, buffer, packetSize, 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (n < 0) {
        cerr << "SocketAPI>receivePacket> error receiving packet" << endl;
        return -1;
    }
    memcpy(packet, buffer, packetSize);
    return n;
}

int SocketAPI::sendPacket(packet_struct* packet, string destIP, uint16_t destPort) {
    int packetSize = sizeof(packet_struct);
    char buffer[1024];
    bzero(buffer, 1024);
    memcpy(buffer, packet, packetSize);

    struct sockaddr_in destAddr;
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(destPort);
    destAddr.sin_addr.s_addr = inet_addr((char*)destIP.c_str());
    bzero(&(destAddr.sin_zero), 8);

    int n = sendto(this->socketfd, buffer, packetSize, 0, (struct sockaddr *)&destAddr, sizeof(destAddr));
    if (n < 0) {
        cerr << "SocketAPI>sendPacket> error sending packet" << endl;
        return -1;
    }
    return n;
}