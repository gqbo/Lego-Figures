// addrspace.cc 
//	Routines to manage address spaces (executing user programs).
//
//	In order to run a user program, you must:
//
//	1. link with the -N -T 0 option 
//	2. run coff2noff to convert the object file to Nachos format
//		(Nachos object code format is essentially just a simpler
//		version of the UNIX executable object code format)
//	3. load the NOFF file into the Nachos file system
//		(if you haven't implemented the file system yet, you
//		don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"

//----------------------------------------------------------------------
// SwapHeader
// 	Do little endian to big endian conversion on the bytes in the 
//	object file header, in case the file was generated on a little
//	endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void 
SwapHeader (NoffHeader *noffH)
{
	noffH->noffMagic = WordToHost(noffH->noffMagic);
	noffH->code.size = WordToHost(noffH->code.size);
	noffH->code.virtualAddr = WordToHost(noffH->code.virtualAddr);
	noffH->code.inFileAddr = WordToHost(noffH->code.inFileAddr);
	noffH->initData.size = WordToHost(noffH->initData.size);
	noffH->initData.virtualAddr = WordToHost(noffH->initData.virtualAddr);
	noffH->initData.inFileAddr = WordToHost(noffH->initData.inFileAddr);
	noffH->uninitData.size = WordToHost(noffH->uninitData.size);
	noffH->uninitData.virtualAddr = WordToHost(noffH->uninitData.virtualAddr);
	noffH->uninitData.inFileAddr = WordToHost(noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
// 	Create an address space to run a user program.
//	Load the program from a file "executable", and set everything
//	up so that we can start executing user instructions.
//
//	Assumes that the object code file is in NOFF format.
//
//	First, set up the translation from program memory to physical 
//	memory.  For now, this is really simple (1:1), since we are
//	only uniprogramming, and we have a single unsegmented page table
//
//	"executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace( AddrSpace* other)
{
    // Este metodo es para copiar el espacio de direcciones de
    // un proceso a otro. 
    // Inicializacion de variables
	numPages = other->numPages;
	pageTable = new TranslationEntry[ numPages ];
    // Ocupo un mapa de bits para saber que paginas estan ocupadas
	long Primeras = numPages - 8;
	long contador;
    // Se copian las primeras 8 paginas
	for (contador = 0; contador < Primeras; ++ contador )
	{
		pageTable[contador].virtualPage =  other->pageTable[contador].virtualPage;
		pageTable[contador].physicalPage = other->pageTable[contador].physicalPage;
		pageTable[contador].valid = other->pageTable[contador].valid;
		pageTable[contador].use = other->pageTable[contador].use;
		pageTable[contador].dirty = other->pageTable[contador].dirty;
		pageTable[contador].readOnly = other->pageTable[contador].readOnly;
	}
    // Se copian las ultimas 8 paginas
	for (contador = Primeras; contador < numPages ; ++ contador )
	{
		pageTable[contador].virtualPage =  contador;
		pageTable[contador].physicalPage = MiMapa->Find();
		pageTable[contador].valid = true;
		pageTable[contador].use = false;
		pageTable[contador].dirty = false;
		pageTable[contador].readOnly = false;
	}
}

AddrSpace::AddrSpace(OpenFile *executable)
{
    // Inicializacion de variables
    NoffHeader noffH;
    unsigned int i, size;

    // Se lee el encabezado del archivo ejecutable
    executable->ReadAt((char *)&noffH, sizeof(noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) && 
		(WordToHost(noffH.noffMagic) == NOFFMAGIC))
    	SwapHeader(&noffH);
    if (noffH.noffMagic != NOFFMAGIC) {
    	printf("El archivo no es un ejecutable de Nachos!\n");
    	ASSERT(false);
    } 
    
        // How big is the address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStackSize;
    numPages = divRoundUp(size, PageSize);
    size = numPages * PageSize;

    // Se verifica que haya suficiente memoria fisica para cargar el programa
    if (numPages > NumPhysPages) {
        printf("Not enough physical memory to load program!\n");
        ASSERT(false);
    }

    DEBUG('a', "Initializing address space, num pages %d, size %d\n", numPages, size);

    // First, set up the translation
    pageTable = new TranslationEntry[numPages];

    // Create a bitmap for tracking the allocation of physical pages
    // BitMap *pageMap = new BitMap(NumPhysPages);
    
    // El espacio de direcciones se carga en memoria fisica
    for (i = 0; i < numPages; i++) {
        // int physPage = pageMap->Find();  // Find a free physical page
        // ASSERT(physPage != -1);  // Make sure a free page is available

        pageTable[i].virtualPage = i;
        pageTable[i].physicalPage = MiMapa->Find();
        pageTable[i].valid = true;
        pageTable[i].use = false;
        pageTable[i].dirty = false;
        pageTable[i].readOnly = false;

        // Mark the corresponding physical page as occupied
        // pageMap->Mark(physPage);
    }

    // delete pageMap;  // Free the page map

    // Zero out the entire address space to initialize the uninitialized data segment and stack segment
    // bzero(machine->mainMemory, size);

    // Then, copy in the code and data segments into memory
    /*
    if (noffH.code.size > 0) {
        DEBUG('a', "Initializing code segment, at 0x%x, size %d\n", noffH.code.virtualAddr, noffH.code.size);
        executable->ReadAt(&(machine->mainMemory[noffH.code.virtualAddr]), noffH.code.size, noffH.code.inFileAddr);
    }
    if (noffH.initData.size > 0) {
        DEBUG('a', "Initializing data segment, at 0x%x, size %d\n", noffH.initData.virtualAddr, noffH.initData.size);
        executable->ReadAt(&(machine->mainMemory[noffH.initData.virtualAddr]), noffH.initData.size, noffH.initData.inFileAddr);
    }
    */
    // Se copia el espacio de direcciones a memoria fisica
	int codeFile = noffH.code.inFileAddr;
	int initFile = noffH.initData.inFileAddr;

	int pagsCodigo = divRoundUp(noffH.code.size, numPages);
	int pagsSegmento = divRoundUp(noffH.initData.size, numPages);

    int contador;
    for (contador = 0; contador < pagsCodigo; ++ contador )
    {
    	executable->ReadAt(&(machine->mainMemory[pageTable[contador].physicalPage * PageSize]), PageSize, codeFile);
    	codeFile += PageSize;
    }

    if (noffH.initData.size > 0) {
        for (contador = pagsCodigo; contador < pagsSegmento; ++ contador )
        {
        	executable->ReadAt(&(machine->mainMemory[pageTable[contador].physicalPage * PageSize]), PageSize, initFile);
        	initFile += PageSize;
        }
    }
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
// 	Dealloate an address space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace()
{
    // Se libera la memoria fisica ocupada por el espacio de direcciones
    for(size_t i = 0; i < numPages; i++){
        if(pageTable[i].valid){
            MiMapa->Clear(pageTable[i].physicalPage);
        }
    }

    delete pageTable;
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
// 	Set the initial values for the user-level register set.
//
// 	We write these directly into the "machine" registers, so
//	that we can immediately jump to user code.  Note that these
//	will be saved/restored into the currentThread->userRegisters
//	when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister(i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister(PCReg, 0);	

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister(NextPCReg, 4);

   // Set the stack register to the end of the address space, where we
   // allocated the stack; but subtract off a bit, to make sure we don't
   // accidentally reference off the end!
    machine->WriteRegister(StackReg, numPages * PageSize - 16);
    DEBUG('a', "Initializing stack register to %d\n", numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
// 	On a context switch, save any machine state, specific
//	to this address space, that needs saving.
//
//	For now, nothing!
//----------------------------------------------------------------------

void AddrSpace::SaveState() 
{}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
// 	On a context switch, restore the machine state so that
//	this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void AddrSpace::RestoreState() 
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}
