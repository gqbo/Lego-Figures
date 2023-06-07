// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"
#include "machine.h"
#include "string.h"
#include "filesys.h"
#include "openfile.h"
#include "synch.h"
#include "addrspace.h"
#include "thread.h"
#include "bitmap.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

/*
 *  Return from a system call.  Update the PC registers.
*/
void returnFromSystemCall() {

        machine->WriteRegister( PrevPCReg, machine->ReadRegister( PCReg ) );		// PrevPC <- PC
        machine->WriteRegister( PCReg, machine->ReadRegister( NextPCReg ));			// PC <- NextPC
        machine->WriteRegister( NextPCReg, machine->ReadRegister( NextPCReg ) + 4 );	// NextPC <- NextPC + 4

}       // returnFromSystemCall

/*
   *  Structure to store information about a thread that is executing a file
   */
struct thrStruct
{
  long threadId;
  std::string fileName; // File name
  Semaphore* s; // Semaphore to synchronize the thread
  inline thrStruct():threadId(-1),fileName(),s(NULL){}
};
// File data
thrStruct** fileData = new thrStruct*[128];
// File data mapping
BitMap* fileDataMapping = new BitMap(128);

/*
 *  System call interface: Halt()
 */
void NachOS_Halt() {		// System call 0
   // NachOS is halting.
	DEBUG('a', "Shutdown, initiated by user program.\n");
   	interrupt->Halt();

}


/*
 *  System call interface: void Exit( int )
 */
void NachOS_Exit() {		// System call 1
   // Esta llamada al sistema termina la ejecución del thread actual
   DEBUG('t', "Entrando en la llamada al sistema EXIT\n");
   Thread* sigHilo;
   // Desactivar interrupciones
   IntStatus Enabled = interrupt->SetLevel(IntOff);
   DEBUG('t', "End of the thread %s\n", currentThread->getName());
   // Cerar los archivos abiertos por el hilo actual
   for (int contador = 0; contador < 128; ++contador) {
      if (fileDataMapping->Test(contador)) {
         if ((long)currentThread == fileData[contador]->threadId) {
            // Cerrar el archivo
            if (fileData[contador]->s != NULL) {
               fileData[contador]->s->V();
            } else {
               // Borra el archivo de la lista de archivos abiertos
               delete fileData[contador];
               fileDataMapping->Clear(contador);
            }
         }
      }
   }
   // Leer el estado de salida del registro 4
   int status = machine->ReadRegister(4);  // Read the exit status from register 4
   printf("\nExiting NachOS with status %d\n", status);
   machine->WriteRegister(2, status);
   sigHilo = scheduler->FindNextToRun();
   // Revise si hay un hilo para ejecutar
   if (sigHilo != NULL) {
      scheduler->Run(sigHilo);
   } else {
      // Si no hay un hilo para ejecutar, se termina la ejecución de NachOS
      currentThread->Finish();
   }
   // Realizar la limpieza necesaria
   
   currentThread->Finish();  // Terminar la ejecución del hilo actual
   // Restaurar el estado de las interrupciones
   interrupt->SetLevel(Enabled);
   /*
   // Verificar si el hilo actual es el hilo principal
   if (currentThread->getName() == "main") {
      // Si es el hilo principal, se termina la ejecución de NachOS
      interrupt->Halt();
   }
   */
}

/*
   *  System call interface: SpaceId Exec( char * )
*/
void NachosExecThread( void* id)
{
   // Obtener información del archivo ejecutable correspondiente al identificador 'id'
   thrStruct* dataS = fileData[(long)id];

   // Abrir el archivo ejecutable
   OpenFile* executable = fileSystem->Open(dataS->fileName.c_str());
   AddrSpace* space;

   // Verificar si el archivo no se pudo abrir correctamente
   if (NULL == executable) {
      printf("No se puede abrir el archivo %s\n", dataS->fileName.c_str());
      return;
   }

   // Crear un nuevo espacio de direcciones basado en el archivo ejecutable
   space = new AddrSpace(executable);

   // Liberar el espacio de direcciones actual del subproceso actual
   delete currentThread->space;

   // Establecer el nuevo espacio de direcciones para el subproceso actual
   currentThread->space = space;

   // Cerrar el archivo ejecutable
   delete executable;

   // Inicializar los registros del espacio de direcciones
   space->InitRegisters();

   // Restaurar el estado de la tabla de páginas
   space->RestoreState();

   // Ejecutar el programa de usuario
   machine->Run();

   // Si se llega a esta línea, se trata de un error inesperado
   printf("\nError\n");
   ASSERT(false);  // Nunca regresa, se interrumpe el programa
} // NachosExecThread

/*
 *  System call interface: SpaceId Exec( char * )
 */
void NachOS_Exec() {		// System call 2
// Ejecutar el archivo ejecutable almacenado en el archivo Nachos "name" y devolver el identificador del espacio de direcciones
// SpaceId Exec(char *name);

   DEBUG('t', "Entrando en la llamada al sistema EXEC\n");
   long addr = machine->ReadRegister(4); // leer desde la dirección de registro 4
   char name[256] = {0}; 

   int value; 

   for(int i = 0; i < 256; i++) {
      machine->ReadMem(addr, 1, &value);
      addr++;
      name[i] = (char)value;
      if (value == 0) break;
   }

   // Crear un nuevo hilo
   std::string s = name;
   thrStruct* dataStruct = new thrStruct();
   Thread* miThread = new Thread("Thread Exec");
   // Buscar un espacio en la tabla de archivos
   long executingFile = fileDataMapping->Find();
   if (executingFile == -1) {
      machine->WriteRegister(2, executingFile);
      return;
   }

   // Guardar la información del archivo
   dataStruct->threadId = (long)miThread;
   dataStruct->fileName = s;
   fileData[executingFile] = dataStruct;

   // Ejecutar el hilo
   miThread->Fork(NachosExecThread, (void*)executingFile);
   machine->WriteRegister(2, executingFile);

   returnFromSystemCall(); // Esto ajusta los registros PrevPC, PC y NextPC
} // NachOS_Exec


/*
 *  System call interface: int Join( SpaceId )
 */
void NachOS_Join() {		// System call 3
   DEBUG('t', "Entrando en la llamada al sistema JOIN\n");
   long id = machine->ReadRegister(4); // leer desde la dirección de registro 4
   if(fileDataMapping->Test(id)){
      // Si el archivo existe, esperar a que termine
      // Crear un semáforo
      Semaphore* joinSemaphore = new Semaphore("Join Sem", 0);
      fileData[id]->s = joinSemaphore;
      joinSemaphore->P();
      delete fileData[id];
      // Eliminar el archivo de la lista de archivos abiertos
      fileDataMapping->Clear(id);

      machine->WriteRegister(2, 0); // Success
   } else {
      machine->WriteRegister(2, -1); // Error
      printf("Error: Join\n");
   }

   returnFromSystemCall();
}


/*
 *  System call interface: void Create( char * )
 */
void NachOS_Create() {		// System call 4
   DEBUG('t', "Entrando en la llamada al sistema CREATE\n");
   int addr = machine->ReadRegister( 4 );
   int value = 0;
   char fileName [256] = {0};
   for(int i = 0; i < 128; i++) {
      machine->ReadMem(addr, 1, &value);
      addr++;
      fileName[i] = (char)value;
      if (value == 0) break;
   }
   // Crear un nuevo archivo en el sistema de archivos UNIX
   int file = creat(fileName, O_CREAT | S_IRWXU );
   close(file);
   // Verificar si hubo un error
   if (-1 == file ) {
      printf("Error: unable to create the new file\n");
   } else {
      DEBUG('t', "Archivo %s creado correctamente\n", fileName);
   }

   returnFromSystemCall();
}


/*
 *  System call interface: OpenFileId Open( char * )
 */
void NachOS_Open() {		// System call 5
   DEBUG('t', "Entrando en la llamada al sistema OPEN\n");
	// Read the name from the user memory, see 5 below
   int addr = machine->ReadRegister( 4 );
	// Uilizar nachos tabla para abrir el archivo
	// en una relación entre unix y nachos
	// Se verifica si hubo errores
   
   char fileName [128] = {0};
   int value = 0;
  
   for(int i = 0; i < 128; i++) {
      machine->ReadMem(addr, 1, &value);
      addr++;
      fileName[i] = (char)value;
      if (value == 0) break;
   }

   // Abrir el archivo en el sistema de archivos UNIX
   int openFile = open( fileName, O_RDWR );
   // Verificar si hubo un error
   if( -1 != openFile ) {
      int openFileNachos = currentThread->miTabla->Open(openFile);
      if( -1 != openFileNachos ) {
         machine->WriteRegister(2, openFileNachos);
         DEBUG('t', "Archivo %s abierto correctamente, id %i\n", fileName, openFileNachos);
         returnFromSystemCall(); // Refresh registers PC, PrevPC, NextPC
         return; // Success
      }
   }
   printf("Error: unable to open the file %s\n", fileName);
   machine->WriteRegister(2, -1); // Error
   returnFromSystemCall(); // Refrescar registros PC, PrevPC, NextPC
} // Nachos_Open


/*
 *  System call interface: OpenFileId Write( char *, int, OpenFileId )
 */
void NachOS_Write() {		// System call 6
   DEBUG('t', "Entrando en la llamada al sistema WRITE\n");
   int size = machine->ReadRegister(5); // Read size to write
   char buffer[size+1] = {0};
   int addr = machine->ReadRegister(4); // Read starting address of buffer
   // Leer el buffer desde la memoria del usuario

   for (int i = 0; i < size; i++) {
      int value;
      if (!machine->ReadMem(addr, 1, &value)) {
         // Failed to read memory
         machine->WriteRegister(2, -1); // Retornar -1 para indicar error
         returnFromSystemCall();
         return;
      }
      addr++;
      buffer[i] = (char)value;
   }
   buffer[size] = '\0'; // Null-terminar el buffer
   OpenFileId descriptor = machine->ReadRegister(6); // Leer el descriptor de archivo
   // Handle writing
   DEBUG('t', "Writing...\n");
   switch (descriptor) {
      case ConsoleInput:
         // Usuario no puede escribir en la entrada estándar
         machine->WriteRegister(2, -1); // User cannot write to standard input
         returnFromSystemCall();
         return;
      case ConsoleOutput:
         // Escribir en la salida estándar
         printf("%s", buffer);
         break;
      case ConsoleError:
         // Escribir en la salida de error estándar
         printf("%s\n", buffer);
         break;
      default:
         // Handle writing to other opened files
         if(!currentThread->miTabla->isOpened(descriptor)){
            machine->WriteRegister( 2, -1 );
            returnFromSystemCall();
            return;
         }
         int file = currentThread->miTabla->getUnixHandle(descriptor);
         size = write(file, buffer, size);
         machine->WriteRegister( 2, size );
         break;
   }
   returnFromSystemCall();

} // Nachos_Write


/*
 *  System call interface: OpenFileId Read( char *, int, OpenFileId )
 */
void NachOS_Read() {		// System call 7
   DEBUG('t', "Entrando en la llamada al sistema READ\n");
   int addr = machine->ReadRegister(4); // Read starting address of buffer
   int size = machine->ReadRegister(5); // Read size to write
   OpenFileId descriptor = machine->ReadRegister(6); // Read file descriptor
   int readed = 0;
   char buffer[size + 1] = {0};
   int quantity = 0;
   
  // Verificar si es output/input, error, ...
   DEBUG('t', "Descriptor: %d\n", descriptor);
   switch ( descriptor ) {
      case ConsoleOutput:
         printf("%s\n", "Error, cant read");
         break;
      case ConsoleError:
         printf("%s\n", "Error, cant read");
         break;
      case ConsoleInput:
         // Read the buffer
         DEBUG('t', "Reading...\n");
         while (readed < size )
         {
            int value = getchar();
            if (value == EOF) {
               break;
            }
            buffer[readed] = (char)value;
            readed++;
         }
         buffer[size+1] = '\0'; // Null-terminar el buffer
         quantity = strlen(buffer);
         // Write the buffer to user space
         for (int i = 0; i < quantity; i++) {
            if (!machine->WriteMem(addr + i, 1, (int)buffer[i])) {
               // Failed to write memory
               printf("Error: unable to write to memory at Read\n");
               machine->WriteRegister(2, -1); // Return -1 in r2 to indicate an error
               returnFromSystemCall();
               return;
            }
         }
         machine->WriteRegister(2, quantity); // Return number of characters read
         break;
      default:
         if ( currentThread->miTabla->isOpened(descriptor)) {
            quantity =  read( currentThread->miTabla->getUnixHandle(descriptor), (void*)buffer, size);
            // Write the buffer to user space
            for (int i = 0; i < quantity; i++) {
               if (!machine->WriteMem(addr + i, 1, (int)buffer[i])) {
                  // Failed to write memory
                  printf("Error: unable to write to memory at Read\n");
                  machine->WriteRegister(2, -1); // Return -1 in r2 to indicate an error
                  returnFromSystemCall();
                  return;
               }
            }
            machine->WriteRegister(2, quantity); // Return number of characters read
         } else {
            DEBUG('t',"Error: unable to read.\n");
            machine->WriteRegister(2, -1); // Return -1 in r2 to indicate an error
         }
         break;
   }

   returnFromSystemCall();
}


/*
 *  System call interface: void Close( OpenFileId )
 */
void NachOS_Close() {		// System call 8
   DEBUG('t', "Entrando en la llamada al sistema CLOSE\n");
   int fileDescriptor = machine->ReadRegister(4);

   // Handle closing
   int unixFile = currentThread->miTabla->getUnixHandle( fileDescriptor );
   int nachFile = currentThread->miTabla->Close(fileDescriptor);
   // Si el archivo no está abierto, retornar -1
   if( unixFile != -1 || nachFile != -1 ) {
      returnFromSystemCall();
      return;
   }
   DEBUG('t',"Error: unable to close the file\n");
   returnFromSystemCall();
}

/*
 *  System call interface: void Fork( void (*func)() )
 */	
void NachosForkThread( void * p ) { // for 64 bits version
      // Esta función es llamada por Fork, para crear un nuevo hilo
      AddrSpace *space;

      space = currentThread->space;
      space->InitRegisters(); // set the initial register values
      space->RestoreState();  // load page table register

   // Set the return address for this thread to the same as the main thread
   // This will lead this thread to call the exit system call and finish
      machine->WriteRegister( RetAddrReg, 4 );

      machine->WriteRegister( PCReg, (long) p );
      machine->WriteRegister( NextPCReg, (long) p + 4 );

      machine->Run();   // jump to the user progam
      ASSERT(false);

}


/*
 *  System call interface: void Fork( void (*func)() )
 */
void NachOS_Fork() {		// System call 9
   DEBUG('t', "Entrando en la llamada al sistema FORK\n");
	// We need to create a new kernel thread to execute the user thread
	Thread * miThread = new Thread( "child to execute Fork code" );

	// We need to share the Open File Table structure with this new child
   delete miThread->miTabla; // We do not need the address space
   // Open File Table structure is shared between father and child
   miThread->miTabla = currentThread->miTabla;
   // Child and father will share the same address space, except for the stack
   miThread->miTabla->addThread();

	// Child and father will also share the same address space, except for the stack
	// Text, init data and uninit data are shared, a new stack area must be created
	// for the new child
	// We suggest the use of a new constructor in AddrSpace class,
	// This new constructor will copy the shared segments (space variable) from currentThread, passed
	// as a parameter, and create a new stack for the new child
	miThread->space = new AddrSpace( currentThread->space );

	// We (kernel)-Fork to a new method to execute the child code
	// Pass the user routine address, now in register 4, as a parameter
	// Note: in 64 bits register 4 need to be casted to (void *)
	miThread->Fork( NachosForkThread, (void*)(long)(machine->ReadRegister( 4 )) );
   // Yield the CPU to the new child thread
   currentThread->Yield();
	returnFromSystemCall();	// This adjust the PrevPC, PC, and NextPC registers

	DEBUG( 'u', "Exiting Fork System call\n" );
}


/*
 *  System call interface: void Yield()
 */
void NachOS_Yield() {		// System call 10
   DEBUG('t', "Entrando en la llamada al sistema YIELD\n");
   currentThread->Yield(); // Realizar el yield
   returnFromSystemCall();	// This adjust the PrevPC, PC, and NextPC registers
}


/*
 *  System call interface: Sem_t SemCreate( int )
 */
void NachOS_SemCreate() {		// System call 11
   DEBUG('t', "Creating a semaphore\n"); 
   // Read the initial value of the semaphore
   int initValue = machine->ReadRegister(4);
   // Create a new semaphore
   new Semaphore("semaphore", initValue);
   // Return the semaphore identifier
   machine->WriteRegister(2, 0);
   returnFromSystemCall();
}


/*
 *  System call interface: int SemDestroy( Sem_t )
 */
void NachOS_SemDestroy() {		// System call 12
   DEBUG('t', "Destroying a semaphore\n");
   // Read the semaphore identifier
   int semId = machine->ReadRegister(4);
   // Destroy the semaphore
   Semaphore *sem = reinterpret_cast<Semaphore *>(semId);
   delete sem;
   // Return 0
   machine->WriteRegister(2, 0);
   returnFromSystemCall();
}


/*
 *  System call interface: int SemSignal( Sem_t )
 */
void NachOS_SemSignal() {		// System call 13
   DEBUG('t', "Signaling a semaphore\n");
   // Read the semaphore identifier
   int semId = machine->ReadRegister(4);
   // Signal the semaphore
   Semaphore *sem = reinterpret_cast<Semaphore *>(semId);
   sem->V();
   // Return 0
   machine->WriteRegister(2, 0);
   returnFromSystemCall();
}


/*
 *  System call interface: int SemWait( Sem_t )
 */
void NachOS_SemWait() {		// System call 14
   DEBUG('t', "Waiting a semaphore\n");
   // Read the semaphore identifier
   int semId = machine->ReadRegister(4);
   // Wait the semaphore
   Semaphore *sem = reinterpret_cast<Semaphore *>(semId);
   sem->P();
   // Return 0
   machine->WriteRegister(2, 0);
   returnFromSystemCall();
}


/*
 *  System call interface: Lock_t LockCreate( int )
 */
void NachOS_LockCreate() {		// System call 15

}


/*
 *  System call interface: int LockDestroy( Lock_t )
 */
void NachOS_LockDestroy() {		// System call 16
/*
   DEBUG('t', "Destroying a lock\n");
   // Read the lock identifier
   int lockId = machine->ReadRegister(4);
   // Destroy the lock
   Lock *lock = reinterpret_cast<Lock *>(lockId);
   delete lock;
   // Return 0
   machine->WriteRegister(2, 0);
   returnFromSystemCall();
*/
}


/*
 *  System call interface: int LockAcquire( Lock_t )
 */
void NachOS_LockAcquire() {		// System call 17
/*
   DEBUG('t', "Acquiring a lock\n");
   // Read the lock identifier
   int lockId = machine->ReadRegister(4);
   // Acquire the lock
   Lock *lock = reinterpret_cast<Lock *>(lockId);
   lock->Acquire();
   // Return 0
   machine->WriteRegister(2, 0);
   returnFromSystemCall();
*/
}


/*
 *  System call interface: int LockRelease( Lock_t )
 */
void NachOS_LockRelease() {		// System call 18
   DEBUG('t', "Releasing a lock\n");
}


/*
 *  System call interface: Cond_t LockCreate( int )
 */
void NachOS_CondCreate() {		// System call 19
   DEBUG('t', "Creating a condition\n");
}


/*
 *  System call interface: int CondDestroy( Cond_t )
 */
void NachOS_CondDestroy() {		// System call 20
   DEBUG('t', "Destroying a condition\n");
}


/*
 *  System call interface: int CondSignal( Cond_t )
 */
void NachOS_CondSignal() {		// System call 21
   DEBUG('t', "Signaling a condition\n");
}


/*
 *  System call interface: int CondWait( Cond_t )
 */
void NachOS_CondWait() {		// System call 22
   DEBUG('t', "Waiting a condition\n");
}


/*
 *  System call interface: int CondBroadcast( Cond_t )
 */
void NachOS_CondBroadcast() {		// System call 23
   DEBUG('t', "Broadcasting a condition\n");
}


/*
 *  System call interface: Socket_t Socket( int, int )
 */
void NachOS_Socket() {			// System call 30
   DEBUG('t', "Creating a socket\n");
	int domain = machine->ReadRegister(4);
	int type = machine->ReadRegister(5);

	Socket_t idSocket;

	if (domain == 0) {//Version 4
		if (type == 0) {//SOCKSTREAM
			idSocket = socket(AF_INET, SOCK_STREAM, 0);
			}
		else {
			idSocket = socket(AF_INET, SOCK_DGRAM, 0);
			}
		}
	else {// version 6
		if (type == 0) {//SOCKSTREAM
			idSocket = socket(AF_INET6, SOCK_STREAM, 0);
			}
		else {
			idSocket = socket(AF_INET6, SOCK_DGRAM, 0);
			}
		}

	if (idSocket == -1) {
		printf("socket constructor error\n");
		}

	int valor = currentThread->miTabla->Open(idSocket);
	machine->WriteRegister(2, valor);
   returnFromSystemCall();
}


/*
 *  System call interface: Socket_t Connect( int, char *, int )
 * ID, IP, Port
 */
void NachOS_Connect() {		// System call 31
   DEBUG('g', "Connecting a socket\n");
	struct sockaddr_in servidor;

	int port;
	int sockId;
	int ipAddr;
	int result;

	bzero((char*)&servidor, sizeof(servidor));

	sockId = machine->ReadRegister(4);
	ipAddr = machine->ReadRegister(5);
	port = machine->ReadRegister(6);

	int car = -1;
	int index = 0;
	char* addr = new char[128];

	while (0 != car) {
		machine->ReadMem(ipAddr, 1, &car);
		addr[index] = (char)car;  //Int to char

		ipAddr++;
		index++;
	}

	if (currentThread->miTabla->isOpened(sockId)) {
		int unixHandle = currentThread->miTabla->getUnixHandle(sockId);

		servidor.sin_family = AF_INET;
		inet_pton(AF_INET, addr, &servidor.sin_addr);
		servidor.sin_port = htons(port);

		result = connect(unixHandle, (struct sockaddr*)&servidor, sizeof(servidor));
	}

	if (-1 == result) {
		printf("Error connect\n");
	}

	machine->WriteRegister(2, result);
   DEBUG('g', "Exiting NachOS_Connect\n");
   returnFromSystemCall();
}


/*
 *  System call interface: int Bind( Socket_t, int )
 */
void NachOS_Bind() {		// System call 32
   DEBUG('t', "Binding a socket\n");
	int idSocket = machine->ReadRegister(4);
	int port = machine->ReadRegister(5);

	int sockSize = sizeof(struct sockaddr_in);

	struct sockaddr_in  addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	int unix_handle = currentThread->miTabla->getUnixHandle(idSocket);
	int result = bind(unix_handle, (struct sockaddr*)&addr, sockSize);

	if (result == -1)
		perror("socket bind");

	machine->WriteRegister(2, result);
   returnFromSystemCall();
   }


/*
 *  System call interface: int Listen( Socket_t, int )
 */
void NachOS_Listen() {		// System call 33
   DEBUG('t', "Listening a socket\n");
	int idSocket = machine->ReadRegister(4);
	int backlog = machine->ReadRegister(5);
	int unix_handle = currentThread->miTabla->getUnixHandle(idSocket);

	int resultado = listen(unix_handle, backlog);

	if (resultado == -1)
		perror("socket listen");

	machine->WriteRegister(2, resultado);
   returnFromSystemCall();	// This adjusts the PrevPC, PC, and NextPC registers
}


/*
 *  System call interface: int Accept( Socket_t )
 */
void NachOS_Accept() {		// System call 34
   DEBUG('t', "Accepting a socket\n");
	int idSocket = machine->ReadRegister(4);
	sockaddr_in addr;
	socklen_t size = sizeof(addr);
	int unix_handle = currentThread->miTabla->getUnixHandle(idSocket);
	int resultado = accept(unix_handle, (struct sockaddr*)&addr, &size);

	if (resultado == -1)
		perror("Connection failed....\n");


	machine->WriteRegister(2, resultado);
   returnFromSystemCall();	// This adjusts the PrevPC, PC, and NextPC registers
}


/*
 *  System call interface: int Shutdown( Socket_t, int )
 */
void NachOS_Shutdown() {	// System call 25
   DEBUG('t', "Shutting down a socket\n");
	int idSocket = machine->ReadRegister(4);
	int mode = machine->ReadRegister(5);

	int unix_handle = currentThread->miTabla->getUnixHandle(idSocket);

	int resultado = shutdown(unix_handle, mode);
	machine->WriteRegister(2, resultado);
   returnFromSystemCall();	// This adjusts the PrevPC, PC, and NextPC registers
}


//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);

    switch ( which ) {

       case SyscallException:
          switch ( type ) {
             case SC_Halt:		// System call # 0
                NachOS_Halt();
                break;
             case SC_Exit:		// System call # 1
                NachOS_Exit();
                break;
             case SC_Exec:		// System call # 2
                NachOS_Exec();
                break;
             case SC_Join:		// System call # 3
                NachOS_Join();
                break;

             case SC_Create:		// System call # 4
                NachOS_Create();
                break;
             case SC_Open:		// System call # 5
                NachOS_Open();
                break;
             case SC_Read:		// System call # 6
                NachOS_Read();
                break;
             case SC_Write:		// System call # 7
                NachOS_Write();
                break;
             case SC_Close:		// System call # 8
                NachOS_Close();
                break;

             case SC_Fork:		// System call # 9
                NachOS_Fork();
                break;
             case SC_Yield:		// System call # 10
                NachOS_Yield();
                break;

             case SC_SemCreate:         // System call # 11
                NachOS_SemCreate();
                break;
             case SC_SemDestroy:        // System call # 12
                NachOS_SemDestroy();
                break;
             case SC_SemSignal:         // System call # 13
                NachOS_SemSignal();
                break;
             case SC_SemWait:           // System call # 14
                NachOS_SemWait();
                break;

             case SC_LckCreate:         // System call # 15
                NachOS_LockCreate();
                break;
             case SC_LckDestroy:        // System call # 16
                NachOS_LockDestroy();
                break;
             case SC_LckAcquire:         // System call # 17
                NachOS_LockAcquire();
                break;
             case SC_LckRelease:           // System call # 18
                NachOS_LockRelease();
                break;

             case SC_CondCreate:         // System call # 19
                NachOS_CondCreate();
                break;
             case SC_CondDestroy:        // System call # 20
                NachOS_CondDestroy();
                break;
             case SC_CondSignal:         // System call # 21
                NachOS_CondSignal();
                break;
             case SC_CondWait:           // System call # 22
                NachOS_CondWait();
                break;
             case SC_CondBroadcast:           // System call # 23
                NachOS_CondBroadcast();
                break;

             case SC_Socket:	// System call # 30
		NachOS_Socket();
               break;
             case SC_Connect:	// System call # 31
		NachOS_Connect();
               break;
             case SC_Bind:	// System call # 32
		NachOS_Bind();
               break;
             case SC_Listen:	// System call # 33
		NachOS_Listen();
               break;
             case SC_Accept:	// System call # 32
		NachOS_Accept();
               break;
             case SC_Shutdown:	// System call # 33
		NachOS_Shutdown();
               break;

             default:
                printf("Unexpected syscall exception %d\n", type );
                ASSERT( false );
                break;
          }
          break;

       case PageFaultException: {
          break;
       }

       case ReadOnlyException:
          printf( "Read Only exception (%d)\n", which );
          ASSERT( false );
          break;

       case BusErrorException:
          printf( "Bus error exception (%d)\n", which );
          ASSERT( false );
          break;

       case AddressErrorException:
          printf( "Address error exception (%d)\n", which );
          ASSERT( false );
          break;

       case OverflowException:
          printf( "Overflow exception (%d)\n", which );
          ASSERT( false );
          break;

       case IllegalInstrException:
          printf( "Ilegal instruction exception (%d)\n", which );
          ASSERT( false );
          break;

       default:
          printf( "Unexpected exception %d\n", which );
          ASSERT( false );
          break;
    }

}
