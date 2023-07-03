#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <thread>
#include <regex>

#include "Socket.h"
#include "ProtocolHeader.hpp"

void task(Socket* client)
{
}

// =================== SERVIDOR INTERMEDIO ===================== //

std::string handleDiscover(const std::string& discover)
{
}

std::string handleRequest(const std::string& request)
{
}

std::string createRequest(const std::string& figureName)
{
}

// =================== SERVIDOR DE PIEZAS ===================== //

std::string handlePresent(const std::string& present)
{
}

std::string handleResponse(const std::string& request)
{
}

std::string createResponse(const std::string& figureName, const std::string& buffer)
{
}

void handleRelease()
{
}

// ==================== MAIN ====================== //

int main(int argc, char** argv)
{
    std::thread* worker;
    Socket* s1, * client;

    s1 = new Socket('s');

    s1->Bind(INTERMEDIARY_TCP_PORT);
    s1->Listen(5);
    s1->SSLInitServer("cert/ci0123.pem", "cert/ci0123.pem");

    for (;;)
    {
        client = s1->Accept();
        client->SSLCreate(s1);
        worker = new std::thread(task, client);
    }
}