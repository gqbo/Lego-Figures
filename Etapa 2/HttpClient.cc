#include "HttpClient.h"

void HttpClient::showFigures()  {
   std::vector<std::string> figuras_disponibles = 
   {"dalmata"};

   std::cout << "Figuras Disponibles:" << std::endl;

   for(int i = 0; i < figuras_disponibles.size(); i++){
      if(i != figuras_disponibles.size()-1){
         std::cout << figuras_disponibles[i] << ", ";
      } else {
         std::cout << figuras_disponibles[i];
      }
   }

   printf("\n\n");
}

std::string HttpClient::requestFigure() {
   // Solicitar el nombre de la figura al usuario.
   std::string input_string;
   std::cout << "Ingrese el nombre de una figura: ";
   std::getline(std::cin, input_string);
   return input_string;
}

void HttpClient::displayLegos(std::string html) {

   // Inicializar la cantidad total de piezas de Lego en 0.
   int total_quantity = 0;
   
   // La expresión regular para coincidir con la lista de piezas de Lego.
   std::regex regex(R"((\d+)\s*(brick.*?)\s*\/)");
   // Iterar sobre coincidencias
    std::sregex_iterator it(html.begin(), html.end(), regex); 
    std::sregex_iterator end;
    //std::cout << "Hola displayLegos" << html << std::endl;
   while (it != end) {
      // Obtener la coincidencia.
      std::smatch match = *it;
      // Extraiga la cantidad y descripción de la pieza de Lego.
      int quantity = std::stoi(match[1].str());
      total_quantity += quantity;
      std::string description = match[2].str();
      // Generar el resultado
      std::cout << quantity << " " << description << std::endl;
      // Aumentar el iterador.
      ++it;
   }
   // Verificar si se encontraron piezas de Lego.
   if (total_quantity == 0) {
      std::cout << "La figura no existe o no se encontraron piezas de lego para esta figura." << std::endl;
   } else {
      // Imprimir el total de piezas de Lego.
      std::cout << "Total de piezas para armar esta figura: " << total_quantity << std::endl;
   }
}