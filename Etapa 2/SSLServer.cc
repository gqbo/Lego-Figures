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

#define PORT 5678
#define BUFSIZE 512


/**
 *   Task each new thread will run
 *      Read string from socket
 *      Write it back to client
 *
 **/
void task( Socket * client ) {
   char a[ BUFSIZE ];
   
   client->SSLAccept();
   client->SSLRead( a, BUFSIZE );	// Read a string from client, data will be limited by BUFSIZE bytes
   std::cout << "Server received: " << a << std::endl;
   std::string htmlName(a);
   htmlName += ".html";
    
   std::ifstream file(htmlName, std::ios::binary);
   if (!file.is_open()) {
      std::cerr << "Error al abrir el archivo" << std::endl;
   }

   std::string html_string;
   std::string line;
   while (std::getline(file, line)) {
      html_string += line;
   }

   file.close();

   char* sendBack = new char[html_string.length() + 1];
   std::strcpy(sendBack, html_string.c_str());
   
   //TODO
   client->SSLWrite( sendBack, strlen(sendBack) );		// Write it back to client, this is the mirror function
   client->Close();		// Close socket in parent

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

   s1->Bind( PORT );		// Port to access this mirror server
   s1->Listen( 5 );		// Set backlog queue to 5 conections
   s1->SSLInitServer( "ci0123.pem", "ci0123.pem" );

   for( ; ; ) {
      client = s1->Accept();	 	// Wait for a client conection
      client->SSLCreate( s1 );
      worker = new std::thread( task, client );
   }

}