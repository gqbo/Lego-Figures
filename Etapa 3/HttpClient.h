#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>

#ifndef HttpClient_h
#define HttpClient_h

class HttpClient {

    public:
    /**
     * @brief Muestra cuales figuras se encuentran disponibles en el servidor de piezas.
     * @return std::string figures_string con las figuras disponibles.
     */
    void showFigures();

    /**
     * @brief Pide al cliente que digite la figura que desea
     * @return std::string input_string con la figura solicitada por el cliente
     */
    std::string requestFigure();

    /**
     * @brief Extrae de la respuesta del servidor web la lista de piezas de Lego y la muestra en pantalla.
     * @param html std::string con la respuesta del servidor web.
     */
    void displayLegos(std::string html);

    private:

};

#endif