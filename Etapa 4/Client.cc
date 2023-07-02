// SSL
#include <openssl/ssl.h>

#include "ProtocolHeader.hpp"
#include "MethodsClient.h"
#include "Socket.h"

#define BUFSIZE 512

int main( int argc, char ** argv ) {
   Socket s('s');     // Crea un socket de IPv4, tipo "stream"
   char buffer[ BUFSIZE + 1];
   
   MethodsClient client;
   // Muestra las figuras que se pueden escoger
   client.showFigures();

    // Solicitar el nombre de la figura al usuario y lo almacena en input_string.
   std::string input_string = client.requestFigure();
   char* input = new char[input_string.length() + 1];
   strcpy(input, input_string.c_str());

   printf("\n");
   s.InitSSL();
   char* host =  (char *) "127.0.0.1";
   s.SSLConnect( host, INTERMEDIARY_TCP_PORT ); // Same port as server
   //printf( "\n\nConnected with %s encryption\n", s.SSLGetCipher() );
   //s.SSLShowCerts();	
   if ( argc > 1 ) {
      s.SSLWrite( argv[1], sizeof(argv[1]) );		// Send first program argument to server
   } else {
      s.SSLWrite( input, sizeof(input) );
   }
   // s.Read( buffer, BUFSIZE );	// Read answer sent back from server
   // printf( "%s\n", buffer );	// Print received string
   // std::cout << html << std::endl;

   int count, itr_count = 0;  // Contador de iteraciones.

   // String para almacenar la respuesta del servidor web.
   std::string html;

   while ((count = s.SSLRead(buffer, BUFSIZE)) > 0) {
      buffer[count] = '\0';
      html += buffer;
      itr_count++;
   }
   client.displayLegos(html);

   delete[] input;
}