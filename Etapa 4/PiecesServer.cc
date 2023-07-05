#include "MethodsPieces.h"

void task( Socket * client ) {
    MethodsPieces mp;
    char buffer[1024];
    struct sockaddr other;
    char code_number;
    memset( &other, 0, sizeof( other ) );
    mp.sendPresent();
    /*----------- RECEIVES UDP MESSAGES ------------*/
    printf("Pieces (LOCAL): Esperando a recibir mensajes en el puerto %d...\n", PIECES_UDP_PORT);
    for ( ; ; ) {
        client->recvFrom((void*)buffer, sizeof(buffer), (void*)&other);
        code_number = buffer[0];
        // switch
        switch (code_number) {
        case '0' /* HANDLES DISCOVER MESSAGE AND SENDS PRESENT MESSAGE */:
        // Se tiene que utilizar la info recibida del broadcast, ahora lo hacemos sin utilizarlo
            printf("Pieces (LOCAL): Mensaje BROADCAST recibido: %s\n", buffer);
            mp.sendPresent(buffer);
            break;
        
        default:
            break;
        }
    }
   
}

int main(int argc, char** argv) {
    MethodsPieces mp;
    std::thread * workerUDP;
    char buffer[1024];

    /*----------- CREATES SOCKET UDP ------------*/
    /*-------- BINDS TO PIECES_UDP_PORT ---------*/
    Socket* socketUDP;
    socketUDP = new Socket('d');
    struct sockaddr_in other;
    socketUDP->Bind(PIECES_UDP_PORT);
    printf("Pieces (LOCAL): Socket UDP bind a %d\n", PIECES_UDP_PORT);

    workerUDP = new std::thread( task, socketUDP ); // CREA SOLO 1 HILO

    // Espera a que el hilo termine
    workerUDP->join();

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

    // /*----------- RECEIVES UDP MESSAGES ------------*/
    // printf("Pieces (127.0.0.1): Esperando a recibir mensajes en el puerto PIECES_UDP_PORT...\n");
    // int n = socketUDP->recvFrom((void*)buffer, sizeof(buffer), (void*)&other);
    // buffer[n] = '\0'; 
    // printf("Pieces (127.0.0.1): Mensaje BROADCAST recibido: %s\n", buffer);