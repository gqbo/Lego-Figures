#include "MethodsPieces.h"


int main(int argc, char** argv) {
    MethodsPieces mp;
    char buffer[1024];

    /*----------- CREATES SOCKET UDP ------------*/
    Socket* socketUDP;
    socketUDP = new Socket('d');
    struct sockaddr_in other;
    socketUDP->Bind(PIECES_UDP_PORT);
    memset( &other, 0, sizeof( other ) );
    printf("Pieces Server: Socket UDP bind a puerto PIECES_UDP_PORT\n");

    /*----------- RECEIVES UDP MESSAGES ------------*/
    int n = socketUDP->recvFrom((void*)buffer, sizeof(buffer), (void*)&other);
    buffer[n] = '\0'; 
    printf("Pieces: Mensaje BROADCAST recibido: %s\n", buffer);

    /*----------- HANDLES UDP MESSAGES ------------*/
    std::string presentMessage = mp.handlePresent(buffer);
    printf("Pieces Server: Crea el mensaje UDP PRESENT\n");

    /*----------- SENDS PRESENT MESSAGE ------------*/
    memset(&other, 0, sizeof(other)); 
    other.sin_family = AF_INET;
    other.sin_port = htons(INTERMEDIARY_UDP_PORT);
    inet_pton(AF_INET, "127.0.0.2", &other.sin_addr);
    const char* message = presentMessage.c_str();
    socketUDP->sendTo((void *)message, strlen(message), (void *)&other);
    printf("Pieces Server: Envia mensaje a 127.0.0.2:INTERMEDIARY_UDP_PORT\n");
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