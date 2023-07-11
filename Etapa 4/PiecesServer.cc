#include "MethodsPieces.h"

void taskUDP( Socket * client, MethodsPieces & mp ) {
    char buffer[BUFSIZE];
    struct sockaddr other;
    char code_number;
    memset( &other, 0, sizeof( other ) );
    mp.sendPresentBroadcast();
    /*----------- RECEIVES UDP MESSAGES ------------*/
    printf("Pieces (LOCAL): Socket UDP esperando a recibir mensajes en el puerto %d...\n", PIECES_UDP_PORT);
    for ( ; ; ) {
        client->recvFrom((void*)buffer, sizeof(buffer), (void*)&other);
        code_number = buffer[0];
        // switch
        switch (code_number) {
        case '0' /* HANDLES DISCOVER MESSAGE AND SENDS PRESENT MESSAGE */:
        // Se tiene que utilizar la info recibida del broadcast, ahora lo hacemos sin utilizarlo
            printf("Pieces (LOCAL): Socket UDP mensaje DISCOVER recibido: %s\n", buffer);
            mp.sendPresent(buffer);
            break;
        
        default:
            break;
        }
    } 
}

void taskTCP( Socket * client, MethodsPieces & mp ) {
    char buffer[BUFSIZE];
    char code_number;
    client->SSLAccept();
    client->SSLRead( buffer, BUFSIZE );	// Read a string from client, data will be limited by BUFSIZE bytes
    printf("Pieces (LOCAL): Socket TCP mensaje REQUEST recibido: %s\n", buffer);
    code_number = buffer[0];
    std::string response_string;

    switch (code_number) {
        case '2' /* HANDLES REQUEST MESSAGE AND SENDS RESPONSE MESSAGE */:
        // Se tiene que utilizar la info recibida del broadcast, ahora lo hacemos sin utilizarlo
            {
            response_string = mp.sendResponse(buffer);
            char* response = new char[response_string.length() + 1];
            strcpy(response, response_string.c_str());
            client->SSLWrite( response, strlen(response) );		// Write it back to client, this is the mirror function
            printf("Pieces (LOCAL): Socket TCP envía RESPONSE con el HTML\n");
            break;
            }
        
        default:
            break;
    }
   //TODO
    client->Close();		// Close socket in parent

}

int main(int argc, char** argv) {
    MethodsPieces mp;
    std::thread * workerUDP;
    std::thread * workerTCP;
    char buffer[1024];

    /*----------- CREATES SOCKET UDP ------------*/
    /*-------- BINDS TO PIECES_UDP_PORT ---------*/
    Socket* socketUDP;
    socketUDP = new Socket('d');
    struct sockaddr_in other;
    socketUDP->Bind(PIECES_UDP_PORT);
    printf("Pieces (LOCAL): Socket UDP bind a %d\n", PIECES_UDP_PORT);

    /*-------- UDP THREAD TO RECEIVE MESSAGES ------------*/
    workerUDP = new std::thread( taskUDP, socketUDP, std::ref(mp) );



    /*----------- CREATES SOCKET TCP ------------*/
    /*-------- BINDS TO PIECES_TCP_PORT ---------*/
    Socket * socketTCP, * client;
    socketTCP = new Socket('s');
    socketTCP->Bind( PIECES_TCP_PORT );
    socketTCP->Listen( 5 );
    socketTCP->SSLInitServer( "cert/ci0123.pem", "cert/ci0123.pem" );
    printf("Pieces (LOCAL): Socket TCP bind a %d\n", PIECES_TCP_PORT);
    
    /*-------- TCP THREAD TO RECEIVE MESSAGES ------------*/
    for( ; ; ) {
        printf("Pieces (LOCAL): Socket TCP esperando a recibir mensajes en el puerto %d\n", PIECES_TCP_PORT);
      client = socketTCP->Accept();
      client->SSLCreate( socketTCP );
      workerTCP = new std::thread( taskTCP, client, std::ref(mp) );
    }

    workerUDP->join();
    workerTCP->join();
}