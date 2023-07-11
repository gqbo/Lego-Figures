/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Pieces Server for PI. Project
 * 
 *   Gabriel Gonzalez Flores
 *   Sebastian Rodriguez Tencio
 *   Juan Aguilar Torres
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

#include "ProtocolHeader.hpp"
#include "Socket.h"

#ifndef METHODSPIECES_H
#define METHODSPIECES_H

class MethodsPieces{

    public:
    
    /**
     * @brief Indica su dirección IP y las figuras disponibles al servidor solicitante por medio de un BROADCAST.
     */
    void sendPresentBroadcast();  // LEGO PRESENT

    /**
     * @brief Indica su dirección IP y las figuras disponibles al servidor solicitante.
     */
    void sendPresent(std::string buffer);  // LEGO PRESENT

    /**
     * @brief Responde con las piezas de la figura solicitada en formato HTML
     */
    std::string sendResponse(const std::string& request);

    /**
     * @brief Indica al servidor intermedio que va a dejar de dar servicios
     */
    std::string handleRelease(const std::string& figureName);

    /**
     * @brief Obtiene los nombres de las figuras del servidor de piezas.
     */
    std::vector<std::string> getFigureNames(const std::string& folderPath);

    private:

    /**
     * @brief Contiene los nombres de las figuras del servidor de piezas.
     */
    std::vector<std::string> figureNames;
};

#endif

    
    
