#include <unistd.h>
#include <stdio.h>	// for perror
#include <stdlib.h>	// for exit
#include <string.h>	// for memset
#include <arpa/inet.h>	// for inet_pton
#include <sys/types.h>	// for connect 
#include <sys/socket.h>
#include <netdb.h>

// SSL
#include <openssl/ssl.h>

#ifndef Socket_h
#define Socket_h

class Socket {

    public:
        /**
         * @brief Construct a new Socket object
         * 
         * @param IPv6 
         */
        Socket( char, bool IPv6 = false );
        
        /**
         * @brief Construct a new Socket object
         * 
         */
        Socket( int );

        /**
         * @brief Destroy the Socket object
         * 
         */
        ~Socket();

        /**
         * @brief 
         * 
         * @param host 
         * @param port 
         * @return int 
         */
        int Connect( const char * host, int port );
        
        /**
         * @brief 
         * 
         * @param host 
         * @param service 
         * @return int 
         */
        int Connect( const char * host, const char * service );
        
        /**
         * @brief 
         * 
         */
        void Close();

        /**
         * @brief 
         * 
         * @param buffer 
         * @param bufferSize 
         * @return int 
         */
        int Read( void * buffer, size_t bufferSize );

        /**
         * @brief 
         * 
         * @param buffer 
         * @param bufferSize 
         * @return int 
         */
        int Write( const void * buffer, size_t bufferSize );

        /**
         * @brief 
         * 
         * @param buffer 
         * @return int 
         */
        int Write( const char * buffer );
        
        /**
         * @brief 
         * 
         * @param backlog 
         * @return int 
         */
        int Listen( int backlog );

        /**
         * @brief 
         * 
         * @param port 
         * @return int 
         */
        int Bind( int port );

        /**
         * @brief 
         * 
         * @return Socket* 
         */
        Socket * Accept();

        /**
         * @brief 
         * 
         * @param mode 
         * @return int 
         */
        int Shutdown( int mode );

        /**
         * @brief 
         * 
         * @param newId 
         */
        void SetIDSocket( int newId );

        /**
         * @brief 
         * 
         * @param buf 
         * @param len 
         * @param other 
         * @return int 
         */
        int sendTo( const void * buf, size_t len, const void * other );

        /**
         * @brief 
         * 
         * @param buf 
         * @param len 
         * @param other 
         * @return int 
         */
        int recvFrom( void * buf, size_t len, const void * other );

        // SSL
        void InitSSLContext();
        void InitSSL();
        int SSLConnect( char * host, int port);
	    int SSLConnect( char * host, char * port);
	    int SSLRead( void * buf, int num );
	    int SSLWrite( void *, int );

    private:
        int idSocket;
        int port;
        bool ipv6;
        // SSL
        void * SSLContext;	// SSL context
	    void * SSLStruct;	// SSL BIO basis input output
        bool ssl;
};

#endif