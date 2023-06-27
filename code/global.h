#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


using namespace std;

#define PORTA_DESCOBERTA 4000 //subservico Descoberta
#define PORTA_MONITORAMENTO 4001 //subservico Monitoramento
#define PORTA_GERENCIA 4002 //subservico Gerencia
#define GLOBAL_BROADCAST_ADD "255.255.255.255"
//NOTE TO SELF: INTERFACE NAO TEM PORTA

// Definição da estrutura do nó da lista encadeada
struct participante {
    std::string hostname;
    std::string ip_address;
    std::string mac_address;
    std::string status;
    participante* next;
};


//Variaveis compartilhadas
string localStatus; //pode ser bool
string sessionMode; //ADMIN ou CLIENTE
string modo; //MANDATO OU ELEICAO
string gerenteHostname; //nome do gerente atual setado na descoberta no modo eleição (ou primeira vez)

participante* tabelaParticipantes = nullptr; //inicio da lista