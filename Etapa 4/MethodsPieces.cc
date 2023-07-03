/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Intermediate server with SSL for PI. Project
 *
 **/

#include "MethodsPieces.h"

void MethodsPieces::task( Socket * client ) {
    char figure[ BUFSIZE ];
   
    client->SSLAccept();
    client->SSLRead( figure, BUFSIZE );	// Read a string from client, data will be limited by BUFSIZE bytes
    std::cout << "Server received: " << figure << std::endl;
}

// LEGO PRESENT
std::string MethodsPieces::handlePresent(bool isResponse){
    // Create a UDP socket
    Socket* socket = new Socket('u');
    struct sockaddr_in other;
    char buffer[1024]; 
    // if(isResponse){
        memset(&other, 0, sizeof(other)); 
        other.sin_family = AF_INET;
        other.sin_port = htons(INTERMEDIARY_UDP_PORT);

        // Set IP for the a specific server
        inet_pton(AF_INET, "127.0.0.1", &other.sin_addr);
        other.sin_addr.s_addr = INADDR_ANY;

        socket->recvFrom((void *)buffer, 1024, (void *)&other);
        printf("Intermediate message received: %s\n", buffer); 
    
    // } else {
    //     int enableBroadcast = 1;
    //     setsockopt(socket->getIDSocket(), SOL_SOCKET, SO_BROADCAST, &enableBroadcast, sizeof(enableBroadcast));
    //     memset(&other, 0, sizeof(other)); 
    //     other.sin_family = AF_INET; 
    //     other.sin_port = htons(INTERMEDIARY_UDP_PORT);
    //     // Set IP for the BROADCAST
    //     // inet_pton(AF_INET, "172.16.123.255", &other.sin_addr);
    //     inet_pton(AF_INET, "127.0.0.1", &other.sin_addr);
    // }
    
    // // Message to Send: Code Separator IP::Port
    // /** TODO: Añadir IP de este servidor*/
    // std::string message_string = std::to_string(LEGO_DISCOVER) + SEPARATOR + "127.0.0.1" + ":" + std::to_string(INTERMEDIARY_UDP_PORT);

    // /** TODO: Especificar IP del server actual*/
    // std::string piecesIP;

    // std::string message = std::to_string(LEGO_PRESENT) + SEPARATOR + piecesIP + ":" + std::to_string(PIECES_UDP_PORT);

    // std::vector<std::string> figuresVector = getFigureNames("figures");
    // for(int i = 0; i < figuresVector.size(); i++) {
    //     message += SEPARATOR + figuresVector[i];
    // }
    
    // socket->sendTo(message.c_str(), message.length(), (void *) &other);

    // socket->Close();

    return "";
}

// LEGO RESPONSE
std::string MethodsPieces::handleResponse(const std::string& request){
    // Create a TCP socket
    Socket* socket = new Socket('s');
    
    
}

// LEGO RELEASE
std::string MethodsPieces::handleRelease(const std::string& figureName){

}
    
    
std::vector<std::string> MethodsPieces::getFigureNames(const std::string& folderPath) {
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            std::string figureName = filename.substr(0, filename.find_last_of('.'));
            this->figureNames.push_back(figureName);
        }
    }

    return figureNames;
}