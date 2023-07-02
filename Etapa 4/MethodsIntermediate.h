/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Intermediate server with SSL for PI. Project
 *
 **/
 
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <thread>

#include "ProtocolHeader.h"
#include "Socket.h"

#ifndef METHODSINTERMEDIATE_H
#define METHODSINTERMEDIATE_H

class MethodsIntermediate {

    public:

        // LEGO DISCOVER
        void handleDiscover();

        // LEGO REQUEST
        std::string handleRequest(const std::string& request);

        // REQUEST CREATION
        std::string parseResponse(const std::string& html);

        // MAP
        void addMapEntry(std::string figure, std::string ip);

        void removeMapEntry(std::string figure);

        std::string getMapEntry(std::string figure);

        std::map< std::string, std::vector<std::string> > getMap();

    private:
    
        std::map< std::string, std::vector<std::string> > mapTable;
};

#endif

    
    
