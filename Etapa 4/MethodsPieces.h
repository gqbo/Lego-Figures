/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Intermediate server with SSL for PI. Project
 *
 **/
 
#include <fstream>
#include <filesystem>
#include <iostream>
#include <map>
#include <regex>
#include <string.h>
#include <stdio.h>
#include <thread>

#include "ProtocolHeader.h"
#include "Socket.h"

#ifndef METHODSPIECES_H
#define METHODSPIECES_H

class MethodsPieces{

    public:
    
    void task( Socket * client );

    // LEGO PRESENT
    std::string handlePresent(bool isResponse);

    // LEGO RESPONSE
    std::string handleResponse(const std::string& request);

    // LEGO RELEASE
    std::string handleRelease(const std::string& figureName);

    std::vector<std::string> getFigureNames(const std::string& folderPath);

    private:

    std::vector<std::string> figureNames;
};

#endif

    
    
