#include <syscall.h>

#define PORT 3141
#define BUFSIZE 512

int main( int argc, char ** argv ) {
    SpaceId newProc;
    OpenFileId input = ConsoleInput;
    OpenFileId output = ConsoleOutput;
    char prompt[8], buffer[60], receive[BUFSIZE];
    int i;

    prompt[0] = 'F';
    prompt[1] = 'i';
    prompt[2] = 'g';
    prompt[3] = 'u';
    prompt[4] = 'r';
    prompt[5] = 'a';
    prompt[6] = ':';
    prompt[7] = ' ';

    Write(prompt, 8, output);
    
    i = 0;
    do 
    {
        Read(&buffer[i], 1, input); 
    } while( buffer[i++] != '\n' );

    buffer[--i] = '\0';

    int id;
    id = Socket( AF_INET_NachOS, SOCK_STREAM_NachOS );

    Connect( id, "127.0.0.1", 3141 );

    Write( buffer, 60, id );

    Read( receive, BUFSIZE, id );
    
    Write(receive, BUFSIZE, output);

    
}
