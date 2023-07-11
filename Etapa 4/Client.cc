#include <openssl/ssl.h>
#include "ProtocolHeader.hpp"
#include "MethodsClient.h"
#include "Socket.h"

#define BUFSIZE 512

int main( int argc, char ** argv ) {
   Socket s('s');
   char buffer[ BUFSIZE + 1];

   if (argc < 2) {
      std::cerr << "Debes proporcionar una dirección IP como argumento." << std::endl;
      return 1;
   }
    
   std::string ipAddress = argv[1];
   std::cout << "Dirección IP proporcionada: " << ipAddress << std::endl;
   
   MethodsClient client;
   client.showFigures();

   std::string input_string = client.requestFigure();
   char* input = new char[input_string.length() + 1];
   strcpy(input, input_string.c_str());

   printf("\n");
   s.InitSSL();
   char* host =  (char *) ipAddress.c_str();
   s.SSLConnect( host, INTERMEDIARY_TCP_PORT );
   s.SSLWrite( input, sizeof(input) );
   
   std::cout << "Se envia solicitud de " << input <<" al Intermediario: " << ipAddress << std::endl;
   int count, itr_count = 0;
   std::string html;

   while ((count = s.SSLRead(buffer, BUFSIZE)) > 0) {
      buffer[count] = '\0';
      html += buffer;
      itr_count++;
   }
   std::cout << "Se recibe solicitud de " << input <<" del Intermediario: " << ipAddress << std::endl;
   printf("\n");
   printf("%s", html.c_str());

   delete[] input;
}