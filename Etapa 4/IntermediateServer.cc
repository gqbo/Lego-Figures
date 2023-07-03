#include "MethodsIntermediate.h"


int main(int argc, char** argv) {
    Socket * s1;
    s1 = new Socket( 'd' );	// Creates an UDP socket: datagram
    struct sockaddr other;
    MethodsIntermediate mi;
    char buffer[1024];
    s1->Bind( INTERMEDIARY_UDP_PORT );
    printf("Intermediate Server: Bind a puerto INTERMEDIARY_UDP_PORT\n");
    memset( &other, 0, sizeof( other ) );

    mi.handleDiscover();
    printf("Intermediate Server: Se termina el handleDiscover.\n");
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