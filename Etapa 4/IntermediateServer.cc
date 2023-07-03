#include "MethodsIntermediate.h"


int main(int argc, char** argv) {

    MethodsIntermediate mi;
    char buffer[1024];

    /*----------- CREATES SOCKET UDP ------------*/
    Socket * socketUDP;
    socketUDP = new Socket( 'd' );	// Creates an UDP socket: datagram
    struct sockaddr other;
    socketUDP->Bind( INTERMEDIARY_UDP_PORT );
    memset( &other, 0, sizeof( other ) );
    printf("Intermediate Server: Socket UDP bind a puerto INTERMEDIARY_UDP_PORT\n");

    /*-------- HANDLES UDP BROADCAST MESSAGE ------------*/
    mi.handleDiscover();

    /*-------- RECEIVES PRESENT MESSAGE ------------*/
    int n = socketUDP->recvFrom((void*)buffer, sizeof(buffer), (void*)&other);
    buffer[n] = '\0'; 
    printf("Intermediate Server: Mensaje recibido: %s\n", buffer);
}



    // mi.addMapEntry("dalmata", "172.16.168.82");
    // mi.addMapEntry("dalmata", "172.16.168.83");
    // mi.addMapEntry("elefante", "172.16.168.82");

    // for (const auto& par : mi.getMap()) {
    //     const std::string& figura = par.first;
    //     const std::vector<std::string>& ips = par.second;

    //     std::cout << "Figura: " << figura << ", IPs: ";
    //     for (const std::string& ip : ips) {
    //         std::cout << ip << " ";
    //     }
    //     std::cout << std::endl;
    // }