#include "MethodsPieces.h"

int main(int argc, char** argv) {
    MethodsPieces mp;
    char buffer[1024];

    /*----------- CREATES SOCKET UDP ------------*/
    Socket* socketUDP;
    socketUDP = new Socket('d');
    struct sockaddr_in other;
    socketUDP->Bind(PIECES_UDP_PORT);
    int enableBroadcast = 1;
    setsockopt(socketUDP->getIDSocket(), SOL_SOCKET, SO_BROADCAST, &enableBroadcast, sizeof(enableBroadcast));
    memset( &other, 0, sizeof( other ) );
    printf("Pieces: Socket UDP bind a 127.0.0.1:PIECES_UDP_PORT\n");

    /*----------- RECEIVES UDP MESSAGES ------------*/
    printf("Pieces (127.0.0.1): Esperando a recibir mensajes en el puerto PIECES_UDP_PORT...\n");
    int n = socketUDP->recvFrom((void*)buffer, sizeof(buffer), (void*)&other);
    buffer[n] = '\0'; 
    printf("Pieces (127.0.0.1): Mensaje BROADCAST recibido: %s\n", buffer);

    /*----------- HANDLES UDP MESSAGES ------------*/
    // Se tiene que utilizar la info recibida del broadcast, ahora lo hacemos sin utilizarlo
    mp.handlePresent();
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