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
         * @brief Construye un nuevo objeto de socket.
         * 
         * @param IPv6 
         */
        Socket( char, bool IPv6 = false );
        
        /**
         * @brief Construye un nuevo objeto de socket.
         * 
         */
        Socket( int );

        /**
         * @brief Destruye el objeto de socket
         * 
         */
        ~Socket();

        /**
         * @brief Establece una conexión con un servidor.
         * 
         * @param host Dirección IP o nombre del servidor.
         * @param port Puerto en el que escucha el servidor.
         * @return int 0 si la conexión fue exitosa, -1 en caso contrario.
         */
        int Connect(const char* host, int port);

        /**
         * @brief Establece una conexión con un servidor.
         * 
         * @param host Dirección IP o nombre del servidor.
         * @param service Servicio al que se desea conectar.
         * @return int 0 si la conexión fue exitosa, -1 en caso contrario.
         */
        int Connect(const char* host, const char* service);

        /**
         * @brief Cierra el socket.
         * 
         */
        void Close();

        /**
         * @brief Lee datos del socket.
         * 
         * @param buffer Puntero al buffer en el que se almacenarán los datos leídos.
         * @param bufferSize Tamaño del buffer.
         * @return int Número de bytes leídos. Si se produce un error, devuelve -1.
         */
        int Read(void* buffer, size_t bufferSize);

        /**
         * @brief Escribe datos en el socket.
         * 
         * @param buffer Puntero al buffer que contiene los datos a escribir.
         * @param bufferSize Tamaño del buffer.
         * @return int Número de bytes escritos. Si se produce un error, devuelve -1.
         */
        int Write(const void* buffer, size_t bufferSize);

        /**
         * @brief Escribe una cadena de caracteres en el socket.
         * 
         * @param buffer Puntero a la cadena de caracteres a escribir.
         * @return int Número de bytes escritos. Si se produce un error, devuelve -1.
         */
        int Write(const char* buffer);

        /**
         * @brief Establece el socket en modo pasivo para escuchar conexiones entrantes.
         * 
         * @param backlog Número máximo de conexiones en cola.
         * @return int 0 si la operación fue exitosa, -1 en caso contrario.
         */
        int Listen(int backlog);

        /**
         * @brief Asocia el socket a un puerto.
         * 
         * @param port Puerto al que se desea asociar el socket.
         * @return int 0 si la operación fue exitosa, -1 en caso contrario.
         */
        int Bind(int port);

        /**
         * @brief Acepta una nueva conexión en el socket.
         * @return Puntero a un objeto Socket que representa la nueva conexión.
        */
        Socket * Accept();

        /**
         * @brief Cierra la comunicación del socket en el modo especificado.
         * @param mode Un entero que especifica el tipo de cierre.
         * @return Un entero que indica si la operación de cierre fue exitosa o no.
        */
        int Shutdown(int mode);

        /**
         * @brief Establece un nuevo ID para el socket.
         * @param newId Un entero que representa el nuevo ID que se va a establecer.
        */
        void SetIDSocket(int newId);

        /**
         * @brief Envía datos a otro socket.
         * @param buf Un puntero al búfer de datos que se va a enviar.
         * @param len La longitud del búfer de datos que se va a enviar.
         * @param other Un puntero al socket al que se enviarán los datos.
         * @return Un entero que indica si la operación de envío fue exitosa o no.
        */
        int sendTo(const void * buf, size_t len, const void * other);

        /**
         * @brief Recibe datos de otro socket.
         * @param buf Un puntero al búfer de datos donde se almacenarán los datos recibidos.
         * @param len La longitud del búfer de datos donde se almacenarán los datos recibidos.
         * @param other Un puntero al socket del que se recibirán los datos.
         * @return Un entero que indica si la operación de recepción fue exitosa o no.
        */
        int recvFrom(void * buf, size_t len, const void * other);

        // SSL

        /**
        @brief Inicializa el contexto SSL para el socket.
        */
        void InitSSLContext();

        /**
        @brief Inicializa SSL para el socket.
        */
        void InitSSL();

        /**
        @brief Conecta con un servidor habilitado para SSL en el host y puerto especificados.
        @param host Una cadena de caracteres que representa el host al que se desea conectar.
        @param port Un entero que representa el puerto al que se desea conectar.
        @return Un entero que indica si la conexión SSL fue exitosa o no.
        */
        int SSLConnect(char * host, int port);

        /**
        @brief Conecta con un servidor habilitado para SSL en el host y puerto especificados.
        @param host Una cadena de caracteres que representa el host al que se desea conectar.
        @param port Una cadena de caracteres que representa el puerto al que se desea conectar.
        @return Un entero que indica si la conexión SSL fue exitosa o no.
        */
        int SSLConnect(char * host, char * port);

        /**
        @brief Lee datos de una conexión SSL.
        @param buf Un puntero al buffer donde se almacenarán los datos leídos.
        @param num El número máximo de bytes a leer.
        @return Un entero que indica cuántos bytes se leyeron.
        */
        int SSLRead(void * buf, int num);

        /**
        @brief Escribe datos en una conexión SSL.
        @param buf Un puntero al buffer de datos que se desea escribir.
        @param num La longitud del buffer de datos que se desea escribir.
        @return Un entero que indica cuántos bytes se escribieron.
        */
        int SSLWrite(void * buf, int num);

        void SSLInitServerContext();
        void SSLInitServer(const char * cert, const char * key);
        void SSLCreate(Socket * parent);
        void SSLShowCerts();
        const char * SSLGetCipher();
        int SSLAccept();

    private:
        /// @brief ID del socket
        int idSocket;
        /// @brief Puerto utilizado por el socket
        int port;
        /// @brief Indica si el socket utiliza IPv6
        bool ipv6;

        // SSL
        /// @brief Contexto SSL utilizado por el socket
        void * SSLContext;	// SSL context
	    /// @brief Estructura SSL BIO utilizada por el socket
        void * SSLStruct;	// SSL BIO basis input output
        /// @brief Indica si el socket utiliza SSL
        bool ssl;
};

#endif