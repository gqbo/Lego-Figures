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

void MethodsPieces::task( Socket * client ) {
    char figure[ BUFSIZE ];
   
    client->SSLAccept();
    client->SSLRead( figure, BUFSIZE );
    std::cout << "Server received: " << figure << std::endl;
}

std::string MethodsPieces::handlePresent(bool isResponse){
    // Create a UDP socket
    Socket* socket = new Socket('u');
    struct sockaddr_in other;
    char buffer[1024]; 
    
    memset(&other, 0, sizeof(other)); 
    other.sin_family = AF_INET;
    other.sin_port = htons(INTERMEDIARY_UDP_PORT);

    // Set IP for the a specific server
    inet_pton(AF_INET, "127.0.0.1", &other.sin_addr);
    other.sin_addr.s_addr = INADDR_ANY;

    socket->recvFrom((void *)buffer, 1024, (void *)&other);
    printf("Intermediate message received: %s\n", buffer); 
    
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

std::string MethodsPieces::handleResponse(const std::string& request){
    // Create a TCP socket
    Socket* socket = new Socket('s');
}


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