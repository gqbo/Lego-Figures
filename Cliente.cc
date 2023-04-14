#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
// SSL
#include <openssl/ssl.h>

#include "Socket.h"

#define SIZE 1024

/**
 * @brief Muestra cuales figuras se encuentran disponibles en el servidor de piezas.
 * @return std::string figures_string con las figuras disponibles.
 */
void showFigures();

/**
 * @brief Pide al cliente que digite la figura que desea
 * @return std::string input_string con la figura solicitada por el cliente
 */
std::string requestFigure();

/**
 * @brief Extrae de la respuesta del servidor web la lista de piezas de Lego y la muestra en pantalla.
 * @param html std::string con la respuesta del servidor web.
 */
void displayLegos(std::string html);

int main( int argc, char * argv[] ) {
   
   // Muestra las figuras que se pueden escoger
   showFigures();

   // Solicitar el nombre de la figura al usuario y lo almacena en input_string.
   std::string input_string = requestFigure();

   printf("\n");

   // Generar la solicitud HTTP.
   std::string url = "GET /lego/list.php?figure=" + input_string + " HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";
   char * os = (char *) "os.ecci.ucr.ac.cr";
   char * reqOS = (char *) new char[url.length() + 1];
   std::strcpy(reqOS, url.c_str());
   Socket socket( 's', false);	// Se crea un nuevo socket stream para IPv4.
   char buffer[ SIZE ];
   int count;
   int itr_count = 0;  // Contador de iteraciones.

   socket.InitSSL(); // Se inicializa el socket SSL.
   socket.SSLConnect( os, (char *) "https" ); // Se conecta el socket SSL al servidor web.
   socket.SSLWrite(  (void *) reqOS, strlen( reqOS ));

   // String para almacenar la respuesta del servidor web.
   std::string html;
   while ( (count = socket.SSLRead( buffer, SIZE )) > 0 ) {
      if(itr_count >= 1) {
         buffer[count] = '\0'; // Se agrega un terminador nulo al final del string.
         html += buffer;       // Se agrega el texto leido al string html
      }
      memset( buffer, 0, SIZE );
      itr_count++;
   }

   displayLegos(html);

   // Borrar el buffer de solicitud.
   delete[] reqOS;
   return 0;
}

void showFigures()  {
   std::vector<std::string> figuras_disponibles = 
   {"blacksheep","dragon","duck","elephant","fish","giraffe","horse","lion",
   "monkey","penguin","roadrunner","shark","squid","swan","turtle","whitesheep"};

   std::cout << "Figuras Disponibles:" << std::endl;

   for(int i = 0; i < figuras_disponibles.size(); i++){
      if(i != figuras_disponibles.size()-1){
         std::cout << figuras_disponibles[i] << ", ";
      } else {
         std::cout << figuras_disponibles[i];
      }
   }

   printf("\n\n");
}

std::string requestFigure() {
   // Solicitar el nombre de la figura al usuario.
   std::string input_string;
   std::cout << "Ingrese el nombre de una figura: ";
   std::getline(std::cin, input_string);
   return input_string;
}

void displayLegos(std::string html) {
   // Inicializar la cantidad total de piezas de Lego en 0.
   int total_quantity = 0;
   // La expresión regular para coincidir con la lista de piezas de Lego.
   std::regex regex(R"(<TR><TD ALIGN=center>\s*(\d+)\s*</TD>\s*<TD ALIGN=center>\s*(.+?)\s*</TD>)");
   // Iterar sobre coincidencias
    std::sregex_iterator it(html.begin(), html.end(), regex); 
    std::sregex_iterator end;
   
   while (it != end) {
      // Obtener la coincidencia.
      std::smatch match = *it;
      // Extraiga la cantidad y descripción de la pieza de Lego.
      int quantity = std::stoi(match[1].str());
      total_quantity += quantity;
      std::string description = match[2].str();
      // Generar el resultado
      std::cout << quantity << " " << description << std::endl;
      // Aumentar el iterador.
      ++it;
   }
   // Verificar si se encontraron piezas de Lego.
   if (total_quantity == 0) {
      std::cout << "La figura no existe o no se encontraron piezas de lego para esta figura." << std::endl;
   } else {
      // Imprimir el total de piezas de Lego.
      std::cout << "Total de piezas para armar esta figura: " << total_quantity << std::endl;
   }
}