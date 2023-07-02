#include "Socket.h"

/**
  *  Class constructor
  *     use Unix socket system call
  *
  *  @param	char type: socket type to define
  *     's' for stream
  *     'd' for datagram
  *  @param	bool ipv6: if we need a IPv6 socket
  *
 **/
Socket::Socket( char type, bool IPv6 ){
   if (IPv6){
      if(type == 's') {  // change
         this->idSocket = socket(AF_INET6, SOCK_STREAM, 0);
      } else {
         this->idSocket = socket(AF_INET6, SOCK_DGRAM, 0);
      }
   } else {
      if(type == 's') {  // change
         this->idSocket = socket(AF_INET, SOCK_STREAM, 0);
      } else {
         this->idSocket = socket(AF_INET, SOCK_DGRAM, 0);
      }
   }
   this->ipv6 = IPv6;
}


/**
  *  Class constructor
  *     use Unix socket system call
  *
  *  @param	int socket descriptor
  *
 **/
Socket::Socket(int socket_descriptor){
   this->idSocket = socket_descriptor;
   this->ipv6 = false;
}


/**
  * Class destructor
  *
 **/
Socket::~Socket(){
    Close();
}


/**
  * Close method
  *    use Unix close system call (once opened a socket is managed like a file in Unix)
  *
 **/
void Socket::Close(){
   // if(this->ssl) {
   //    SSL_free((SSL*) this->SSLStruct);
   //    SSL_CTX_free((SSL_CTX *) this->SSLContext);
   // }
   close(this->idSocket);
}

/**
  * Connect method
  *   use "connect" Unix system call
  *
  * @param	char * host: host address in dot notation, example "10.1.104.187"
  * @param	int port: process address, example 80
  *
 **/
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


/**
  * Connect method
  *
  * @param	char * host: host address in dot notation, example "163.178.104.187"
  * @param	char * service: service name, example "http"
  *
 **/
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


/**
  * Read method
  *   use "read" Unix system call (man 3 read)
  *
  * @param	void * text: buffer to store data read from socket
  * @param	int size: buffer capacity, read will stop if buffer is full
  *
 **/
int Socket::Read( void * text, size_t size ) {
   int st = -1;
   st = read(this->idSocket, text, size);  // change
   return st;

}


/**
  * Write method
  *   use "write" Unix system call (man 3 write)
  *
  * @param	void * buffer: buffer to store data write to socket
  * @param	size_t size: buffer capacity, number of bytes to write
  *
 **/
int Socket::Write( const void *text, size_t size ) {
   int st = -1;
   st = write(this->idSocket, text, size); // change
   return st;
}


/**
  * Write method
  *
  * @param	char * text: string to store data write to socket
  *
  *  This method write a string to socket, use strlen to determine how many bytes
  *
 **/
int Socket::Write( const char *text ) {
   int st = -1;
   st = write(this->idSocket, text, strlen(text));
   return st;
}


/**
  * Bind method
  *    use "bind" Unix system call (man 3 bind) (server mode)
  *
  * @param	int port: bind a socket to a port defined in sockaddr structure
  *
  *  Links the calling process to a service at port
  *
 **/
int Socket::Bind( int port ) {
   int st = -1;
   struct sockaddr * ha;
   struct sockaddr_in host4;
   struct sockaddr_in6 host6;

   if (!ipv6) {
      host4.sin_family = AF_INET; 
      host4.sin_port = htons(port); 
      host4.sin_addr.s_addr = htonl(INADDR_ANY); 

      st = bind(this->idSocket, (struct sockaddr *) &host4, sizeof(sockaddr_in));

      if ( -1 == st ) {	// check for errors
         perror( "Socket::Bind");
         exit( 2 );
      }
   }

   return st;
}


/**
  * Listen method
  *   use "listen" Unix system call (server mode)
  *
  * @param	int queue: max pending connections to enqueue 
  *
  *  This method define how many elements can wait in queue
  *
 **/
int Socket::Listen( int queue ) {
   int st = -1;
   st = listen(this->idSocket, queue);
   if ( -1 == st ) {	// check for errors
      perror( "Socket::Listen");
      exit( 2 );
   }
   return st;
}


/**
  * Accept method
  *    use "accept" Unix system call (man 3 accept) (server mode) 
  *
  *  @returns	a new class instance
  *
  *  Waits for a new connection to service (TCP  mode: stream)
  *
 **/
Socket * Socket::Accept(){
   int st = -1;
   int host_size = 0;
   struct sockaddr_in host4;
   struct sockaddr_in6 host6;

   if (ipv6) {
      host_size = sizeof(sockaddr_in6);
      host6.sin6_family = AF_INET6; 
      host6.sin6_port = htons(port); 
      host6.sin6_addr = in6addr_any; 

      st = accept(this->idSocket, (struct sockaddr *) &host6, (socklen_t *) &host_size); 
   } else {
      host_size = sizeof(sockaddr_in);
      host4.sin_family = AF_INET; 
      host4.sin_port = htons(port); 
      host4.sin_addr.s_addr = INADDR_ANY; 

      st = accept(this->idSocket, (struct sockaddr *) &host4, (socklen_t *) &host_size);
   }
   Socket* new_socket = new Socket(st);
   return (Socket *) new_socket;
}


/**
  * Shutdown method
  *    use "shutdown" Unix system call (man 3 shutdown)
  *
  *  @param	int mode define how to cease socket operation
  *
  *  Partial close the connection (TCP mode)
  *
 **/
int Socket::Shutdown( int mode ) {
   int st = -1;
   st = shutdown(this->idSocket, mode);
   if ( -1 == st ) {	// check for errors
      perror( "Socket::Shutdown");
      exit( 2 );
   }
   return st;

}


/**
  *  SetIDSocket
  *
  *  @param	int id assigns a new value to field
  *  
 **/
void Socket::SetIDSocket(int id){
   this->idSocket = id;
}

int Socket::getIDSocket(){
   return this->idSocket;
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

void Socket::SSLInitServerContext() {
    SSL_library_init();

    // We must create a method to define our context
    const SSL_METHOD *method = TLS_server_method();
    // Check for errors
    SSL_CTX *context = SSL_CTX_new(method);

    if (context == NULL) {
        perror("Error creando contexto SSL");
        exit(1);
    }

    SSL_CTX_set_options(context, SSL_OP_ALL);

    this->SSLContext = static_cast<void*>(context);
}

void Socket::SSLInitServer(const char * cert, const char * key) {
    // Create a new SSL context for the server
    this->SSLInitServerContext();

    // Create a new SSL structure for the server
    SSL *ssl = SSL_new((SSL_CTX *) this->SSLContext);
    if (!ssl) {
        // Error
        SSL_free(ssl);
        perror("Error in SSL_new()");
        exit(1);
    }

    // Attach the SSL structure to the socket
    this->SSLStruct = (void *) ssl;

    // Set the SSL structure to operate in server mode
    SSL_set_accept_state(static_cast<SSL*>(this->SSLStruct));

    // Load the certificate into the SSL context
    if (SSL_CTX_use_certificate_file(static_cast<SSL_CTX*>(this->SSLContext), cert, SSL_FILETYPE_PEM) <= 0) {
        perror("Error en SSL_CTX_use_certificate_file()");
        exit(1);
    }

    // Load the private-key corresponding to the client certificate
    if (SSL_CTX_use_PrivateKey_file(static_cast<SSL_CTX*>(this->SSLContext), key, SSL_FILETYPE_PEM) <= 0) {
        perror("Error en SSL_CTX_use_PrivateKey_file()");
        exit(1);
    }

    // Check if the client certificate and private-key matches
    if (!SSL_CTX_check_private_key(static_cast<SSL_CTX*>(this->SSLContext))) {
        perror("Error en SSL_CTX_check_private_key()");
        exit(1);
    }

    // Set the SSL structure to operate in server mode
    SSL_set_fd(static_cast<SSL*>(this->SSLStruct), this->idSocket);

    this->SSLAccept();
}

void Socket::SSLCreate(Socket *parent_socket) {
    // Create a new SSL structure for the server
    SSL *ssl = SSL_new((SSL_CTX *) parent_socket->SSLContext);
    if (!ssl) {
        // Error
        SSL_free(ssl);
        perror("Error in SSL_new()");
        exit(1);
    }
    // Attach the SSL structure to the socket
    this->SSLStruct = (void *) ssl;
    // Set the SSL structure to operate in server mode
    SSL_set_accept_state(static_cast<SSL*>(this->SSLStruct));
    
    // Set the SSL structure to operate in server mode
    SSL_set_fd(static_cast<SSL*>(this->SSLStruct), this->idSocket);

    // Accept the SSL connection
    int st = SSL_accept(static_cast<SSL*>(this->SSLStruct));
    if (st == -1) {
        perror("Error en SSL_accept()");
        exit(1);
    }
}

void Socket::SSLShowCerts() {
    // Get the remote certificate into the X509 structure
    X509 *cert = SSL_get_peer_certificate(static_cast<SSL*>(this->SSLStruct));

    if (cert == nullptr) {
        return;
    }

    // Get the remote common name
    char *line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);

    if (line == nullptr) {
        printf("Error processing certificate.\n");
        X509_free(cert);
        return;
    }

    // Print the common name
    printf("Common name: %s\n", line);

    // Free the malloc'ed string
    free(line);
    // Free the malloc'ed certificate
    X509_free(cert);
}

const char * Socket::SSLGetCipher(){
    return SSL_get_cipher(static_cast<SSL*>(this->SSLStruct));
}

int Socket::SSLAccept(){
    return SSL_accept(static_cast<SSL*>(this->SSLStruct));
}