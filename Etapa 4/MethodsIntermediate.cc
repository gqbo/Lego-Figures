/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Intermediate server with SSL for PI. Project
 *
 **/

#include "MethodsIntermediate.h"
#include "MethodsCommon.h"

// #define IP_ADDRESS "172.16.123.84"
// #define IP_BROADCAST "172.16.123.95"

void MethodsIntermediate::sendDiscover() {
   /*----------- CREATES SOCKET UDP ------------*/
   /*---------AND ENABLES BROADCATS CONFIG ------------*/
   Socket* socket = new Socket('u');
   struct sockaddr_in other;
   int enableBroadcast = 1;
   setsockopt(socket->getIDSocket(), SOL_SOCKET, SO_BROADCAST, &enableBroadcast, sizeof(enableBroadcast));
   memset(&other, 0, sizeof(other));
   other.sin_family = AF_INET; 
   other.sin_port = htons(PIECES_UDP_PORT);
   /*----------- GETS IP ADDRESS OF THE PC ------------*/
   /*----------- AND SETS .255 AT THE END ------------*/
   std::string IPAddress = getIPAddress();
   std::string IPBroadcast;
   size_t thirdDot = IPAddress.find('.', IPAddress.find('.', IPAddress.find('.') + 1) + 1);

   if (thirdDot != std::string::npos) {
      IPBroadcast = IPAddress.substr(0, thirdDot + 1) + "255";
   }
   // inet_pton(AF_INET, IP_BROADCAST, &other.sin_addr);
   other.sin_addr.s_addr = inet_addr(IPBroadcast.c_str());

   printf("sendDiscover BROADCAST: Se crea socket hacia %s:%d\n", IPBroadcast.c_str(), PIECES_UDP_PORT);

   /*----------- CREATES BROADCAST MESSAGE ------------*/
   std::string message_string = std::to_string(LEGO_DISCOVER) + SEPARATOR + IPAddress + ":" + std::to_string(INTERMEDIARY_UDP_PORT);
   const char* message = message_string.c_str();

   /*---- SENDS BROADCAST MESSAGE TO PIECES_UDP_PORT ------*/
   socket->sendTo((void *)message, strlen(message), (void *)&other);
   printf("sendDiscover BROADCAST: Se envia mensaje DISCOVER con info %s:%d\n", IPAddress.c_str(), INTERMEDIARY_UDP_PORT);
}

void MethodsIntermediate::handlePresent(std::string buffer){
   std::vector<std::string>infoPresent = splitPresent(buffer);
   for (size_t i = 3; i < infoPresent.size(); i++) {
      addMapEntry(infoPresent[i], infoPresent[1]);
   }
}

std::string MethodsIntermediate::createRequest(std::string figure) {
   std::string message = std::to_string(LEGO_REQUEST) + SEPARATOR + figure;
   return message;
}

std::string MethodsIntermediate::sendRequest(std::string ip_piezas, std::string request_string){
   Socket client('s');
   char buffer[BUFSIZE];
   client.InitSSL();
   client.SSLConnect((char*)ip_piezas.c_str(), PIECES_TCP_PORT);

   char* request = new char[request_string.length() + 1];
   std::strcpy(request, request_string.c_str());

   printf("Intermediate Server (LOCAL): Nuevo socket TCP envía %s al servidor de piezas %s\n", request, ip_piezas.c_str());
   client.SSLWrite(request, strlen(request));
   // READ
   int count, itr_count = 0;
   std::string html;
   while ((count = client.SSLRead(buffer, BUFSIZE)) > 0) {
      buffer[count] = '\0';
      html += buffer;
      itr_count++;
   }
   printf("Intermediate Server (LOCAL): Nuevo socket TCP recibe RESPONSE que trae HTML\n");
   // TODO: QUITAR CODIGO SEPARATOR FIGURE SEPARATOR, para dejar el html solo
   return html;
}

std::string MethodsIntermediate::parseResponse(const std::string& html) {
   int total_quantity = 0;
   std::string parsed_html;
   std::regex regex(R"(<TR><TD ALIGN=center>\s*(\d+)\s*</TD>\s*<TD ALIGN=center>\s*(.+?)\s*</TD>)");
   std::sregex_iterator it(html.begin(), html.end(), regex); 
   std::sregex_iterator end;
   
   while (it != end) {
      std::smatch match = *it;
      int quantity = std::stoi(match[1].str());
      total_quantity += quantity;
      std::string description = match[2].str();
      parsed_html +=  std::to_string(quantity) + " " + description + "\n";
      ++it;
   }
   
   if (total_quantity == 0) {
      parsed_html = "La figura no existe o no se encontraron piezas de lego para esta figura.\n";
   } else {
      parsed_html += "Total de piezas para armar esta figura: " + std::to_string(total_quantity) + "\n";
   }
   return parsed_html;
}

void MethodsIntermediate::addMapEntry(std::string figure, std::string ip){
   printf("Intermediate Server (LOCAL): Se añade al mapa la figura: %s -> %s\n", figure.c_str(), ip.c_str());
   if (mapTable.count(figure) == 0) {
      mapTable[figure] = {ip};  // Crear un nuevo vector con la dirección IP
   } else {
      mapTable[figure].push_back(ip);  // Agregar la dirección IP al vector existente
   }
   
   // for (const auto& pair : mapTable) {
   //    std::cout << "FIGURA: " << pair.first << ", IPS:";
   //    for (const auto& value : pair.second) {
   //       std::cout << " " << value;
   //    }
   //    std::cout << std::endl;
   // }
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

std::string MethodsIntermediate::getMapEntry(std::string figure) {
    std::string result = "";
    auto it = mapTable.find(figure);
    if (it != mapTable.end()) {
        std::vector<std::string>& ipVector = it->second;

        if (!ipVector.empty()) {
            result = ipVector[0];
        }
    }
    return result;
}

std::map< std::string, std::vector<std::string> > MethodsIntermediate::getMap(){
   return mapTable;
}

bool MethodsIntermediate::containsFigure(const std::string& figure) {
    auto it = mapTable.find(figure);
    return it != mapTable.end();
}