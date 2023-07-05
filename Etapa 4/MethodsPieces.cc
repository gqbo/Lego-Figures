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
#include "MethodsCommon.h"


// void MethodsPieces::task( Socket * client ) {
//     char figure[ BUFSIZE ];
   
//     client->SSLAccept();
//     client->SSLRead( figure, BUFSIZE );
//     std::cout << "Server received: " << figure << std::endl;
// }

void MethodsPieces::sendPresent(){
    /*----------- CREATES SOCKET UDP ------------*/
    /*---------AND ENABLES BROADCATS CONFIG ------------*/
    Socket* socket = new Socket('u');
    struct sockaddr_in other;
    int enableBroadcast = 1;
    setsockopt(socket->getIDSocket(), SOL_SOCKET, SO_BROADCAST, &enableBroadcast, sizeof(enableBroadcast));
    memset(&other, 0, sizeof(other));
    other.sin_family = AF_INET; 
    other.sin_port = htons(INTERMEDIARY_UDP_PORT);
    /*----------- GETS IP ADDRESS OF THE PC ------------*/
    /*----------- AND SETS .255 AT THE END ------------*/
    std::string IPAddress = getIPAddress();
    std::string IPBroadcast;
    size_t thirdDot = IPAddress.find('.', IPAddress.find('.', IPAddress.find('.') + 1) + 1);

    if (thirdDot != std::string::npos) {
        IPBroadcast = IPAddress.substr(0, thirdDot + 1) + "255";
    }
    inet_pton(AF_INET, IPBroadcast.c_str(), &other.sin_addr);
    printf("sendPresent BROADCAST: Se crea socket hacia %s:%d\n", IPBroadcast.c_str(), INTERMEDIARY_UDP_PORT);
    
    std::string message_string = std::to_string(LEGO_PRESENT) + SEPARATOR + IPAddress + ":" + std::to_string(PIECES_UDP_PORT);

    // Almacena las figuras
    getFigureNames("figures");

    for(int i = 0; i < figureNames.size(); i++) {
        message_string += SEPARATOR + figureNames[i];
    }
    const char* message = message_string.c_str();

    socket->sendTo((void *)message, strlen(message), (void *)&other);
    printf("sendPresent BROADCAST: Se envia mensaje PRESENT con info %s:%d:", IPAddress.c_str(), PIECES_UDP_PORT);
    for(int i = 0; i < figureNames.size(); i++) {
        printf("%s", figureNames[i].c_str());
    }
    printf("\n");
}

void MethodsPieces::sendPresent(std::string buffer){
    Socket* socket = new Socket('u');
    struct sockaddr_in other;
    memset(&other, 0, sizeof(other)); 
    other.sin_family = AF_INET;
    // Se tiene que sacar el puerto y la IP del DISCOVER
    std::vector<std::string> discoverInfo = splitDiscover(buffer);
    other.sin_port = htons(std::stoi(discoverInfo[2]));
    inet_pton(AF_INET, discoverInfo[1].c_str(), &other.sin_addr);
    printf("sendPresent: Se crea socket hacia %s:%s\n", discoverInfo[1].c_str(), discoverInfo[2].c_str());
    
    std::string IPAddress = getIPAddress();
    std::string message_string = std::to_string(LEGO_PRESENT) + SEPARATOR + IPAddress + ":" + std::to_string(PIECES_UDP_PORT);

    for(int i = 0; i < figureNames.size(); i++) {
        message_string += SEPARATOR + figureNames[i];
    }
    const char* message = message_string.c_str();

    socket->sendTo((void *)message, strlen(message), (void *)&other);
    printf("sendPresent: Se envia mensaje PRESENT con info %s:%d:", IPAddress.c_str(), PIECES_UDP_PORT);
    for(int i = 0; i < figureNames.size(); i++) {
        printf("%s", figureNames[i].c_str());
    }
    printf("\n");
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
