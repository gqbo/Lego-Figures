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

int main( int argc, char * argv[] ) {
   // Solicitar el nombre de la figura al usuario.
   std::string input_string;
   std::cout << "Ingrese el nombre de una figura: ";
   std::getline(std::cin, input_string);

   // Generar la solicitud HTTP.
   std::string url = "GET /lego/list.php?figure=" + input_string + " HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";
   char * os = (char *) "os.ecci.ucr.ac.cr";
   char * reqOS = (char *) new char[url.length() + 1];
   std::strcpy(reqOS, url.c_str());
   Socket socket( 's', false);	// Se crea un nuevo socket stream para IPv4.
   char buffer[ SIZE ];
   int count;
   int itr_count = 0;  // Contador de iteraciones.
   // String para almacenar la respuesta del servidor web.
   std::string html;

   socket.InitSSL(); // Se inicializa el socket SSL.
   socket.SSLConnect( os, (char *) "https" ); // Se conecta el socket SSL al servidor web.
   socket.SSLWrite(  (void *) reqOS, strlen( reqOS ));
   // Leer la respuesta del servidor web.
   while (  (count = socket.SSLRead( buffer, SIZE )) > 0 ) {
    
      if(itr_count >= 1) {
         buffer[count] = '\0'; // Se agrega un terminador nulo al final del string.
         html += buffer;  // Se agrega el texto leido al string html
      }
      memset( buffer, 0, SIZE );
      itr_count++;
   }
   // Inicializar la cantidad total de piezas de Lego en 0.
   int totalQuantity = 0;
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
      totalQuantity += quantity;
      std::string description = match[2].str();
      // Generar el resultado
      std::cout << quantity << " " << description << std::endl;
      // Aumentar el iterador.
      ++it;
   }
   // Verificar si se encontraron piezas de Lego.
   if (totalQuantity == 0) {
      std::cout << "La figura no existe o no se encontraron piezas de lego para esta figura." << std::endl;
   } else {
      // Imprimir el total de piezas de Lego.
      std::cout << "Total de piezas para armar esta figura: " << totalQuantity << std::endl;
   }
   // Borrar el buffer de solicitud.
   delete[] reqOS;
   return 0;
}
