#include "MethodsIntermediate.h"

void task( Socket * client ) {
    MethodsIntermediate mi;
    char buffer[1024];
    struct sockaddr other;
    char code_number;
    memset( &other, 0, sizeof( other ) );
    /*--- SENDS BROADCAST DISCOVER TO PIECES_UDP_PORT ---*/
    mi.sendDiscover();
    printf("Intermediate Server (LOCAL): Esperando a recibir mensajes en el puerto %d...\n", INTERMEDIARY_UDP_PORT);
    for( ; ; ) {
        client->recvFrom((void*)buffer, sizeof(buffer), (void*)&other);
        code_number = buffer[0];

        switch (code_number) {
        case '1':
            printf("Intermediate Server (LOCAL): Mensaje present recibido: %s\n", buffer);

            mi.handlePresent(buffer);
            
            break;
        
        default:
            break;
        }
    }
   
}

int main(int argc, char** argv) {
    MethodsIntermediate mi;
    std::thread * workerUDP;
    char buffer[1024];

    /*----------- CREATES SOCKET UDP ------------*/
    /*----- BINDS TO INTERMEDIARY_UDP_PORT ------*/
    Socket * socketUDP;
    socketUDP = new Socket( 'd' );
    socketUDP->Bind( INTERMEDIARY_UDP_PORT);
    printf("Intermediate Server (LOCAL): Socket UDP bind a %d\n", INTERMEDIARY_UDP_PORT);

    /*-------- UDP THREAD TO RECEIVE MESSAGES ------------*/
    workerUDP = new std::thread( task, socketUDP );

    // Espera a que el hilo termine
    workerUDP->join();

}
