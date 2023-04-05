#include "Socket.h"

Socket::Socket( char type, bool IPv6 ){
   if (IPv6){
      if(type == 's') {
         this->idSocket = socket(AF_INET6, SOCK_STREAM, 0);
      } else {
         this->idSocket = socket(AF_INET6, SOCK_DGRAM, 0);
      }
   } else {
      if(type == 's') {
         this->idSocket = socket(AF_INET, SOCK_STREAM, 0);
      } else {
         this->idSocket = socket(AF_INET, SOCK_DGRAM, 0);
      }
   }
   this->ipv6 = IPv6;
}


Socket::~Socket(){
    Close();
}


void Socket::Close(){
   if(this->ssl) {
      SSL_free((SSL*) this->SSLStruct);
      SSL_CTX_free((SSL_CTX *) this->SSLContext);
   }
   close(this->idSocket);
}


int Socket::Connect( const char * host, int port ) {
   int st;
   struct sockaddr_in  host4;
   struct sockaddr * ha;

   memset( (char *) &host4, 0, sizeof( host4 ) );
   host4.sin_family = AF_INET;
   inet_pton( AF_INET, host, &host4.sin_addr );
   host4.sin_port = htons( port );
   ha = (struct sockaddr *) &host4;
   st = connect( idSocket, (sockaddr *) ha, sizeof( host4 ) );
   if ( -1 == st ) {	// check for errors
      perror( "Socket::Connect" );
      exit( 2 );
   }

   return st;
}


int Socket::Connect( const char *host, const char *service ) {
   int st = -1;
   struct addrinfo hints, *result, *rp;

   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
   hints.ai_socktype = SOCK_STREAM; /* Stream socket */
   hints.ai_flags = 0;
   hints.ai_protocol = 0;          /* Any protocol */
   st = getaddrinfo(host, service, &hints, &result );

   for ( rp = result; rp; rp = rp->ai_next ) {
      st = connect(this->idSocket, rp->ai_addr, rp->ai_addrlen );
      if ( 0 == st )
         break;
   }

   freeaddrinfo( result );
   return st;
}


int Socket::Read( void * text, size_t size ) {
   int st = -1;
   st = read(this->idSocket, text, size);
   return st;

}


int Socket::Write( const void *text, size_t size ) {
   int st = -1;
   st = write(this->idSocket, text, size);
   return st;

}


int Socket::Write( const char *text ) {
   int st = -1;
   st = write(this->idSocket, text, strlen(text));
   return st;
}


int Socket::Listen( int queue ) {
    int st = -1;

    return st;

}


int Socket::Bind( int port ) {
   int st = -1;
   struct sockaddr * ha;
   struct sockaddr_in host4;
   struct sockaddr_in6 host6;

   host4.sin_family = AF_INET; 
   host4.sin_port = htons(port); 
   host4.sin_addr.s_addr = INADDR_ANY; 

   st = bind(this->idSocket, (struct sockaddr *) &host4, sizeof(sockaddr_in));

   if ( -1 == st ) {	// check for errors
      perror( "Socket::Bind");
      exit( 2 );
   }

   return st;

}


Socket * Socket::Accept(){

   return (Socket *) 0;

}


int Socket::Shutdown( int mode ) {
   int st = -1;

   return st;

}


void Socket::SetIDSocket(int id){

    this->idSocket = id;

}

int Socket::sendTo(const void *buf, size_t len, const void *other) {
   int st = -1;

   struct sockaddr * ha = (struct sockaddr *)other;
   st = sendto(this->idSocket, buf,len, 0, ha, sizeof(struct sockaddr_in));

   if ( -1 == st ) {
      perror( "Socket::Send to" );
      exit( 2 );
   }
   return st;
}

int Socket::recvFrom( void * buf, size_t len, const void * other ) {
   int st = -1;

   int addrlen = sizeof(sockaddr_in);
   st = recvfrom(this->idSocket, buf, len, 0, (struct sockaddr*) other, (socklen_t*) &addrlen);

   if ( -1 == st ) {
      perror( "Socket::recvFrom");
      exit( 2 );
   }
   return st;
}

void Socket::InitSSLContext(){
   // We must create a method to define our context
   const SSL_METHOD * method = TLS_client_method();
   // Check for errors
   SSL_CTX * context = SSL_CTX_new( method );
   // Check for errors
   this->SSLContext = (void *) context;
}

void Socket::InitSSL() {
   this->InitSSLContext();
   SSL * ssl = SSL_new( (SSL_CTX *) this->SSLContext);
   // Check for errors
   this->SSLStruct = (void *) ssl;
   this->ssl = true;
}	// Create a SSL socket, a new context must be created before

int Socket::SSLConnect(char * host, int port) {
   int st = -1;

   Connect( host, port );	// Establish a non SSL connection first
   SSL_set_fd( (SSL *) this->SSLStruct, this->idSocket );
   st = SSL_connect( (SSL *) this->SSLStruct );

   if ( -1 == st ) {
      perror( "Socket::SSLConnect1" );
      exit( 2 );
   }

   // printf( "Connected with %s encryption\n", SSL_get_cipher( (SSL *) this->SSLStruct ) );

   return st;
}

int Socket::SSLConnect( char * host, char * port) {
   int st = -1;

   Connect( host, port );	// Establish a non SSL connection first
   SSL_set_fd( (SSL *) this->SSLStruct, this->idSocket );
   st = SSL_connect( (SSL *) this->SSLStruct );

   if ( -1 == st ) {
      perror( "Socket::SSLConnect2" );
      exit( 2 );
   }

   // printf( "Connected with %s encryption\n", SSL_get_cipher( (SSL *) this->SSLStruct ) );

   return st;
}

int Socket::SSLRead( void * buf, int num) {
   int st = -1;

   st = SSL_read((SSL *) this->SSLStruct, buf, num);
   
   if ( -1 == st ) {
      perror( "Socket::SSLRead" );
      exit( 2 );
   }

   return st;
}

int Socket::SSLWrite( void * buf, int num) {
   int st = -1;

   st = SSL_write((SSL *) this->SSLStruct, buf, num);

   if ( -1 == st ) {
      perror( "Socket::SSLRead" );
      exit( 2 );
   }

   return st;
}