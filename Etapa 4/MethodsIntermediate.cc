/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Intermediate server with SSL for PI. Project
 *
 **/

#include "MethodsIntermediate.h"

void MethodsIntermediate::handleDiscover() {
   // Send a UDP broadcast message to all servers on the PIECES_UDP_PORT
   Socket* socket = new Socket('u');
   struct sockaddr_in other;

   // Enable BROADCAST to send a LEGO DISCOVER message
   int enableBroadcast = 1;
   setsockopt(socket->getIDSocket(), SOL_SOCKET, SO_BROADCAST, &enableBroadcast, sizeof(enableBroadcast));
   printf("handleDiscover: Se configura socket para BROADCAST\n");

   // Set memory, family and port into the variable other.
   memset(&other, 0, sizeof(other)); 
   other.sin_family = AF_INET; 
   other.sin_port = htons(PIECES_UDP_PORT);
   printf("handleDiscover: Va a enviar BROADCAST al puerto PIECES_UDP_PORT\n");

   // Set IP for the BROADCAST
   inet_pton(AF_INET, "255.255.255.255", &other.sin_addr);
   printf("handleDiscover: Va a enviar BROADCAST a la IP 255.255.255.255\n");

   // Message to Send: Code Separator IP::Port
   /** TODO: Add broadcast IP after testing*/
   std::string message_string = std::to_string(LEGO_DISCOVER) + SEPARATOR + "255.255.255.255" + ":" + std::to_string(INTERMEDIARY_UDP_PORT);
   const char* message = message_string.c_str();
   printf("handleDiscover: Se crea mensaje con formato LEGO_DISCOVER\n");

   // Send BROADCAST message to every server on the PIECES_UDP_PORT
   socket->sendTo((void *)message, strlen(message), (void *)&other);
   printf("handleDiscover: Se envia mensaje BROADCAST en el socket al puerto PIECES_UDP_PORT con la IP 255.255.255.255\n");
}


std::string MethodsIntermediate::handleRequest(const std::string& request) {
   // Create a TCP socket
   Socket* socket = new Socket('s');
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

std::string MethodsIntermediate::parseResponse(const std::string& html) {
   return "";
}

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