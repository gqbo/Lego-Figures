/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Intermediate server with SSL for PI. Project
 *
 **/

#include "MethodsIntermediate.h"

// LEGO DISCOVER
void MethodsIntermediate::handleDiscover() {
   // Send a UDP broadcast message to all servers on the PIECES_UDP_PORT
   Socket* socket = new Socket('u');
   struct sockaddr_in other;
   // Enable BROADCAST to send a LEGO DISCOVER message
   int enableBroadcast = 1;
   setsockopt(socket->getIDSocket(), SOL_SOCKET, SO_BROADCAST, &enableBroadcast, sizeof(enableBroadcast));

   memset(&other, 0, sizeof(other)); 
   other.sin_family = AF_INET; 
   other.sin_port = htons(PIECES_UDP_PORT);

   // Set IP for the BROADCAST
   inet_pton(AF_INET, "172.16.123.255", &other.sin_addr);

   // Message to Send: Code Separator IP::Port
   /** TODO: Añadir IP de este servidor*/
   std::string message_string = std::to_string(LEGO_DISCOVER) + SEPARATOR + "IP (Pendiente)" + "::" + std::to_string(INTERMEDIARY_UDP_PORT);
   const char* message = message_string.c_str();

   // Send BROADCAST message to every server on the PIECES_UDP_PORT
   socket->sendTo((void *)message, strlen(message), (void *)&other);
}

// LEGO REQUEST
std::string MethodsIntermediate::handleRequest(const std::string& request) {
   // Create a TCP socket
   Socket* socket = new Socket('t');
   // Obtain server IP from map

   /** TODO: Programar obtención de IP Según figura */
   // Utilizar mapa
   char * PIECES_SERVER_IP; // Pendiente

   socket->Connect(PIECES_SERVER_IP, PIECES_TCP_PORT);
   // Construct the request message: Code Separator Figure
   std::string message = std::to_string(LEGO_REQUEST) + SEPARATOR + request;
   // Send the request to the pieces server
   socket->Write(message.c_str(), message.length());
   // Read the response from the pieces server
   char responseBuffer[BUFSIZE];
   int bytesRead = socket->Read(responseBuffer, BUFSIZE);
   std::string response(responseBuffer, bytesRead);
   socket->Close();

   /** TODO: Parsear response*/
   // Utilizar "Parse Response"

   return response;
}

// REQUEST CREATION
std::string MethodsIntermediate::parseResponse(const std::string& html) {
   return "";
}

// MAP

void MethodsIntermediate::addMapEntry(std::string figure, std::string ip){
   if (mapTable.count(figure) == 0) {
      mapTable[figure] = {ip};  // Crear un nuevo vector con la dirección IP
   } else {
      mapTable[figure].push_back(ip);  // Agregar la dirección IP al vector existente
   }
}

void MethodsIntermediate::removeMapEntry(std::string ip){

   bool ipFound = false;

   // Buscar y eliminar la IP en todo el mapa
   for (auto& par : mapTable) {
      std::vector<std::string>& ips = par.second;

      auto it = std::find(ips.begin(), ips.end(), ip);
      if (it != ips.end()) {
         ips.erase(it);
         ipFound = true;
         std::cout << "IP erased from the figure '" << par.first << "'." << std::endl;
         break; // Salir del bucle si se encuentra la IP en alguna figura
      }
   }

   if (!ipFound) {
      std::cout << "IP not found in any figure." << std::endl;
   }


}

std::string MethodsIntermediate::getMapEntry(std::string figure){
   std::string result;
    
    auto it = mapTable.find(figure);
    if (it != mapTable.end()) {
        std::vector<std::string>& ipVector = it->second;
        result += figure + ": ";

        for (const auto& ip : ipVector) {
            result += ip + " ";
        }
    }
    
    return result;
}

std::map< std::string, std::vector<std::string> > MethodsIntermediate::getMap(){
   return mapTable;
}