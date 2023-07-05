#include "MethodsIntermediate.h"


int main(int argc, char** argv) {

    MethodsIntermediate mi;
    char buffer[1024];

    /*----------- CREATES SOCKET UDP ------------*/
    Socket * socketUDP;
    socketUDP = new Socket( 'd' );
    struct sockaddr other;
    socketUDP->Bind( INTERMEDIARY_UDP_PORT);
    memset( &other, 0, sizeof( other ) );
    printf("Intermediate Server: Socket UDP bind a 127.0.0.1:INTERMEDIARY_UDP_PORT\n");

    /*-------- HANDLES UDP BROADCAST MESSAGE ------------*/
    mi.handleDiscover();

    /*-------- RECEIVES PRESENT MESSAGE ------------*/
    printf("Intermediate Server (127.0.0.1): Esperando a recibir mensajes en el puerto INTERMEDIARY_UDP_PORT...\n");
    int n = socketUDP->recvFrom((void*)buffer, sizeof(buffer), (void*)&other);
    buffer[n] = '\0'; 
    printf("Intermediate Server (127.0.0.1): Mensaje recibido: %s\n", buffer);
}
