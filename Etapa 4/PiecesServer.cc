#include "MethodsPieces.h"


int main(int argc, char** argv) {
    Socket* s1;
    s1 = new Socket('u');  // Crea un socket UDP: datagrama
    struct sockaddr other;
    MethodsPieces mp;
    char buffer[1024];
    
    // Realiza el bind en el puerto PIECES_UDP_PORT
    s1->Bind(PIECES_UDP_PORT);
    printf("Pieces Server: Bind a puerto PIECES_UDP_PORT\n");
    memset( &other, 0, sizeof( other ) );

    int n = s1->recvFrom((void*)buffer, sizeof(buffer), (void*)&other);
    buffer[n] = '\0'; 
    printf("Pieces: Broadcast message received: %s\n", buffer);



    // mp.handlePresent(1);
    // printf("handlePresent: Se termina el handlePresent\n");
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