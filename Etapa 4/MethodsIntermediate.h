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

#include "ProtocolHeader.hpp"
#include "Socket.h"

#ifndef METHODSINTERMEDIATE_H
#define METHODSINTERMEDIATE_H

class MethodsIntermediate {

    public:

    /**
     * @brief Realiza un broadcast a traves de UDP el cual solicita a los servidores de piezas 
     * que reciban este mensaje que se reporten respecto al servidor emisor.
     */
    void sendDiscover();

    /**
     * @brief Solicita las piezas de una figura al servidor de piezas
     */
    std::string handleRequest(const std::string& request);

    /**
     * @brief Parsea el HTML para la respuesta que se envia al cliente
     */
    std::string parseResponse(const std::string& html);

    /**
     * @brief 
     * 
     * @param buffer 
     */
    void handlePresent(std::string buffer);

    /**
     * @brief Se agrega una entrada en el mapa relacionando una figura con una IP.
     * @param figure Figura que pertenece a un servidor de piezas.
     * @param ip IP que pertenece a un servidor de piezas.
     */
    void addMapEntry(std::string figure, std::string ip);

    /**
     * @brief Se elimina un servidor de piezas del mapa.
     * @param ip IP que pertenece a un servidor de piezas.
     */
    void removeMapEntry(std::string ip);

    /**
     * @brief Se obtiene la figura y las ip's que estan relacionadas.
     * @param ip IP que pertenece a un servidor de piezas.
     * @return Devuelve la figura y las ip's que estan relacionadas.
     */
    std::string getMapEntry(std::string figure);

    /**
     * @brief Obtiene el mapa con las figuras y las ip's relacionadas.
     * @return Returna la variable mapTable con toda su informacion.
     */
    std::map< std::string, std::vector<std::string> > getMap();

    private:
    
    /**
     * @brief Mapa que contiene a las figuras y las relaciona con las ip's de cada servidor de piezas.
     */
    std::map< std::string, std::vector<std::string> > mapTable;
};

#endif

    
    
