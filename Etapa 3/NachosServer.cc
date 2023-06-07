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
      std::cerr << "404: No se encontró archivo." << std::endl;
      std::string notFound = "404: No se encontró archivo.";
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

   /*Parsing Start*/
   std::string parsed;
   std::string startTag = "<pref>";
   std::string endTag = "</pref>";
   
   // Find the starting position of the content within the <pref> tag
   size_t startPos = html_string.find(startTag);
   if (startPos == std::string::npos)
      parsed = "404: No se encontró archivo, en formato HTML.";
   
   startPos += startTag.length();

   // Find the ending position of the content within the </pref> tag
   size_t endPos = html_string.find(endTag);
   if (endPos == std::string::npos)
      parsed = "404: No se encontró archivo, en formato HTML.";

   // Extract the content between the start and end positions
   std::string content = html_string.substr(startPos, endPos - startPos);

   // Replace " / " with a newline character '\n'
   size_t delimiterPos = 0;
   while ((delimiterPos = content.find("/", delimiterPos)) != std::string::npos) {
      content.replace(delimiterPos, 3, "\n");
      delimiterPos += 1;  // Move past the inserted newline character
   }

   parsed = content;
   /*Parsing End*/

   char* sendBack = new char[html_string.length() + 1];
   std::strcpy(sendBack, html_string.c_str());
   
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

   s1->Bind( PORT );		// Port to access this mirror server
   s1->Listen( 5 );		// Set backlog queue to 5 conections

   for( ; ; ) {
      client = s1->Accept();	 	// Wait for a client conection
      worker = new std::thread( task, client);
      worker->detach();
   }

}