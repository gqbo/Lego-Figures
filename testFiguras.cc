#include <stdio.h>
#include <string.h>
#include <regex>
#include <fstream>
#include <iostream>
#include <regex>
// SSL
#include <openssl/ssl.h>

#include "Socket.h"

#define SIZE 1024

int main( int argc, char * argv[] ) {
   char * os = (char *) "os.ecci.ucr.ac.cr";
   char * reqOS = (char *) "GET /lego/list.php?figure=shark HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";
   Socket s( 's', false);	// Create a new stream socket for IPv4
   char a[ SIZE ];
   int count;
   int itr_count = 0;

   std::ofstream myfile; // declare output file stream
   myfile.open("texto.html"); // open HTML file

   if (myfile.is_open()) {
      myfile << "<HTML>\n";
   } else {
      std::cout << "Unable to open file";
   }

   s.InitSSL();
   s.SSLConnect( os, (char *) "https" );
   s.SSLWrite(  (void *) reqOS, strlen( reqOS ));
   while (  (count = s.SSLRead( a, SIZE )) > 0 ) {
    
      if(itr_count >= 1) {
         //printf( "%s\n", a );
         a[count] = '\0'; // Add null terminator to end of string
         myfile << a ;
      }
      memset( a, 0, SIZE );
      itr_count++;
   }
   myfile.close(); // close HTML file
}