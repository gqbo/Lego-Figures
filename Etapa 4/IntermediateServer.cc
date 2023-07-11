#include "MethodsIntermediate.h"

void taskUDP( Socket * client, MethodsIntermediate & mi) {
    char buffer[1024];
    struct sockaddr other;
    char code_number;
    memset( &other, 0, sizeof( other ) );
    /*--- SENDS BROADCAST DISCOVER TO PIECES_UDP_PORT ---*/
    mi.sendDiscover();
    printf("Intermediate Server (LOCAL): Socket UDP esperando a recibir mensajes en el puerto %d...\n", INTERMEDIARY_UDP_PORT);
    for( ; ; ) {
        client->recvFrom((void*)buffer, sizeof(buffer), (void*)&other);
        code_number = buffer[0];

        switch (code_number) {
        case '1':
            printf("Intermediate Server (LOCAL): Socket UDP mensaje PRESENT recibido: %s\n", buffer);

            mi.handlePresent(buffer);
            
            break;
        
        default:
            break;
        }
    }
}

void taskTCP( Socket * client, MethodsIntermediate & mi ) {
    char buffer[BUFSIZE];
    char code_number;
    std::string parse_html = "";
    std::string error_message = "";
    client->SSLAccept();
    client->SSLRead( buffer, BUFSIZE );	// Read a string from client, data will be limited by BUFSIZE bytes
    printf("Intermediate Server (LOCAL): Socket TCP recibe del cliente la figura: %s\n", buffer);
    
    std::string figure = buffer;
    if(mi.containsFigure(figure)) {
        std::string ip_piezas = mi.getMapEntry(figure);
        std::string request_string = mi.createRequest(figure);
        std::string html_string = mi.sendRequest(ip_piezas, request_string);
        parse_html = mi.parseResponse(html_string);
        char* html = new char[parse_html.length() + 1];
        strcpy(html, parse_html.c_str());
        client->SSLWrite( html, strlen(html) );		// Write it back to client, this is the mirror function
        printf("Intermediate Server (LOCAL): Socket TCP envia respuesta del HTML al cliente.\n");
    } else {
        std::string error_message = "La figura no existe o no se encontraron piezas de lego para esta figura.\n";
        char* error = new char[error_message.length() + 1];
        strcpy(error, error_message.c_str());
        client->SSLWrite( error, strlen(error) );		// Write it back to client, this is the mirror function
        printf("Intermediate Server (LOCAL): Socket TCP envia respuesta del HTML al cliente.\n");   
    }


    client->Close();		// Close socket in parent
}


int main(int argc, char** argv) {
    MethodsIntermediate mi;
    std::thread * workerUDP;
    std::thread * workerTCP;
    char buffer[1024];

    /*----------- CREATES SOCKET UDP ------------*/
    /*----- BINDS TO INTERMEDIARY_UDP_PORT ------*/
    Socket * socketUDP;
    socketUDP = new Socket( 'd' );
    socketUDP->Bind( INTERMEDIARY_UDP_PORT);
    printf("Intermediate Server (LOCAL): Socket UDP bind a %d\n", INTERMEDIARY_UDP_PORT);

    /*-------- UDP THREAD TO RECEIVE MESSAGES ------------*/
    workerUDP = new std::thread( taskUDP, socketUDP, std::ref(mi));

    /*----------- CREATES SOCKET TCP ------------*/
    /*-------- BINDS TO PIECES_TCP_PORT ---------*/
    Socket * socketTCP, * client;
    socketTCP = new Socket('s');
    socketTCP->Bind( INTERMEDIARY_TCP_PORT );
    socketTCP->Listen( 5 );
    socketTCP->SSLInitServer( "cert/ci0123.pem", "cert/ci0123.pem" );
    printf("Intermediate Server (LOCAL): Socket TCP bind con %d\n", INTERMEDIARY_TCP_PORT);
    
    /*-------- TCP THREAD TO RECEIVE MESSAGES ------------*/
    for( ; ; ) {
        printf("Intermediate Server (LOCAL): Socket TCP esperando a recibir mensajes en el puerto %d\n", INTERMEDIARY_TCP_PORT);
        client = socketTCP->Accept();
        client->SSLCreate( socketTCP );
        workerTCP = new std::thread( taskTCP, client, std::ref(mi) );
    }

    workerUDP->join();
    workerTCP->join();
}
