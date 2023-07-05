/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Pieces Server for PI. Project
 * 
 *   Gabriel Gonzalez Flores
 *   Sebastian Rodriguez Tencio
 *   Juan Aguilar Torres
 *
 **/

#include "MethodsPieces.h"

// void MethodsPieces::task( Socket * client ) {
//     char figure[ BUFSIZE ];
   
//     client->SSLAccept();
//     client->SSLRead( figure, BUFSIZE );
//     std::cout << "Server received: " << figure << std::endl;
// }

void MethodsPieces::handlePresent(){
    Socket* socket = new Socket('u');
    struct sockaddr_in other;
    memset(&other, 0, sizeof(other)); 
    other.sin_family = AF_INET;
    // Se tiene que sacar el puerto y la IP del DISCOVER
    other.sin_port = htons(INTERMEDIARY_UDP_PORT);
    inet_pton(AF_INET, "127.0.0.1", &other.sin_addr);
    printf("handlePresent: Se crea socket hacia 127.0.0.2:INTERMEDIARY_UDP_PORT\n");
    
    std::string message_string = std::to_string(LEGO_PRESENT) + SEPARATOR + "127.0.0.1" + ":" + std::to_string(PIECES_UDP_PORT);

    std::vector<std::string> figuresVector = getFigureNames("figures");
    for(int i = 0; i < figuresVector.size(); i++) {
        message_string += SEPARATOR + figuresVector[i];
    }
    const char* message = message_string.c_str();

    socket->sendTo((void *)message, strlen(message), (void *)&other);
    printf("handlePresent: Se envia mensaje PRESENT con info 127.0.0.1:PIECES_UDP_PORT:figuras...\n");
}

std::string MethodsPieces::handleResponse(const std::string& request){
    // Create a TCP socket
    Socket* socket = new Socket('s');
    return "";
}


std::string MethodsPieces::handleRelease(const std::string& figureName){
    return "";
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
