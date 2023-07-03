#include "MethodsPieces.h"


int main(int argc, char** argv) {
    Socket * s1;
    s1 = new Socket( 'd' );	// Creates an UDP socket: datagram

    struct sockaddr other;
    MethodsPieces mp;
    char buffer[1024];
    char *hello = (char *) "Hello from intermediate server"; 
    s1->Bind( INTERMEDIARY_UDP_PORT );
    memset( &other, 0, sizeof( other ) );

    mp.handlePresent(1);

    int n = s1->recvFrom( (void *) buffer, 1024, (void *) &other );	// Mensaje de los www servers
    buffer[n] = '\0'; 
    printf("Server: message received: %s\n", buffer);
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