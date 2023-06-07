#include "nachostabla.h"

NachosOpenFilesTable::NachosOpenFilesTable() {
    DEBUG('t', "TABLA: Initializing Open Files Table...\n");
	this->openFiles = new int[ MAX_FILES ];
    // 0 = stdin
	this->openFiles[0] = 0;
    // 1 = stdout
	this->openFiles[1] = 1;
    // 2 = stderr
	this->openFiles[2] = 2;

	for (int x = 3; x < MAX_FILES; x++)
		 this->openFiles[x] = 0;

	this->openFilesMap = new BitMap( MAX_FILES );
	this->openFilesMap->Mark(0);
	this->openFilesMap->Mark(1);
	this->openFilesMap->Mark(2);
}

/*
*  Destructor
*/
NachosOpenFilesTable::~NachosOpenFilesTable() {
    DEBUG('t', "TABLA: Deleting Open Files Table...\n");
	if(usage <= 0){
			delete[] openFiles;
			delete openFilesMap;
			DEBUG('f', "OpenFilesTable deleted.\n");
	}
}

/*
*  IsOpened, Returns true if the file is opened
*  parameter: NachosHandle, the handle of the file
*/
bool NachosOpenFilesTable::isOpened( int NachosHandle ) {
    DEBUG('t', "TABLA: Checking if file is opened...\n");
    if( NachosHandle >= 0 && NachosHandle < MAX_FILES ) {
        return openFilesMap->Test(NachosHandle);
    }

    return false;
}

int NachosOpenFilesTable::Open( int UnixHandle ) {
    DEBUG('t', "TABLA: Opening file...\n");
	int myFile = this->openFilesMap->Find();

	if (myFile != -1) {
		this->openFiles[ myFile ] = UnixHandle;
	}
	return myFile;
}

int NachosOpenFilesTable::Close( int NachosHandle ) {
    DEBUG('t', "TABLA: Closing file...\n");
    if(isOpened(NachosHandle)) {
	    this->openFilesMap->Clear( NachosHandle );
	    this->openFiles[ NachosHandle ] = 0;
        return	 0;
    }

    return -1;
}

int NachosOpenFilesTable::getUnixHandle( int NachosHandle ) {
    DEBUG('t', "TABLA: Getting Unix Handle...\n");
    if(isOpened(NachosHandle)) {
        return openFiles[NachosHandle];
    }
    return -1;
}

void NachosOpenFilesTable::addThread() {
    usage++;
}

void NachosOpenFilesTable::delThread() {
    usage--;
}

void NachosOpenFilesTable::Print() {
    DEBUG('t', "TABLA: Printing Open Files Table:\n");
    for(int i = 0; i < MAX_FILES; ++i) {
        printf("Nachos handle is %d, Unix handle is %d\n", i, openFiles[i]);
    }
    
    printf("\n");
}