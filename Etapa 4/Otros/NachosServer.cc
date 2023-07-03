/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Socket client/server example with threads
 *
 **/
 
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <thread>
#include <regex>

#include "Socket.h"

#define PORT 3141
#define BUFSIZE 512


/**
 *   Task each new thread will run
 *      Read string from socket
 *      Write it back to client
 *
 **/
void task( Socket * client) {
   char a[ BUFSIZE ];
   memset( a, 0, BUFSIZE );
   int bytesRead = client->Read( a, BUFSIZE );	// Read a string from client, data will be limited by BUFSIZE bytes
   
   if (bytesRead <= 0) {
      std::cerr << "Error reading from client" << std::endl;
      return;
   }
   
   std::cout << "Server received: " << a << std::endl;
   std::string htmlName(a);
   htmlName = "figures/" + htmlName + ".html";
    
   std::ifstream file(htmlName, std::ios::binary);
   if (!file.is_open()) {
      std::cerr << "Error 404: No se encontró el archivo solicitado." << std::endl;
      std::string notFound = "Error 404: No hay figura con ese nombre.";
      char* sendBack = new char[notFound.length() + 1];
      std::strcpy(sendBack, notFound.c_str());
      client->Write( sendBack, strlen(sendBack) );
      client->Close();
      return;
   }

   std::string html_string;
   std::string line;
   while (std::getline(file, line)) {
      html_string += line;
   }

   file.close();

   // Inicializar la cantidad total de piezas de Lego en 0.
   int total_quantity = 0;
   std::string html;
   
   // La expresión regular para coincidir con la lista de piezas de Lego.
   std::regex regex(R"((\d+)\s*(brick.*?)\s*\/)");
   // Iterar sobre coincidencias
    std::sregex_iterator it(html_string.begin(), html_string.end(), regex); 
    std::sregex_iterator end;
    //std::cout << "Hola displayLegos" << html << std::endl;
   while (it != end) {
      // Obtener la coincidencia.
      std::smatch match = *it;
      // Extraiga la cantidad y descripción de la pieza de Lego.
      int quantity = std::stoi(match[1].str());
      total_quantity += quantity;
      std::string description = match[2].str();
      // Generar el resultado
      html =  html + std::to_string(quantity) + " " + description + "\n";
      // Aumentar el iterador.
      ++it;
   }
   // Verificar si se encontraron piezas de Lego.
   if (total_quantity == 0) {
      html = " ";
      html = html + "La figura no existe o no se encontraron piezas de lego para esta figura." + "\n";
   } else {
      // Imprimir el total de piezas de Lego.
      html =  html + "Total de piezas para armar esta figura: " + std::to_string(total_quantity) + "\n";
   }

   std::string parsed = html;

   char* sendBack = new char[parsed.length() + 1];
   std::strcpy(sendBack, parsed.c_str());
   
   client->Write( sendBack, strlen(sendBack) );		// Write it back to client, this is the mirror function
   client->Close();		// Close socket in parent
   return;
}


/**
 *   Create server code
 *      Infinite for
 *         Wait for client conection
 *         Spawn a new thread to handle client request
 *
 **/
int main( int argc, char ** argv ) {
   std::thread * worker;
   Socket * s1, * client;

   s1 = new Socket( 's' );

   s1->Bind( PORT );
   s1->Listen( 5 );

   for( ; ; ) {
      client = s1->Accept();
      worker = new std::thread( task, client);
      worker->detach();
   }

}t