# Proyecto-Programado-CI0123

## **Descripción del problema**

Clientes: Se obtiene las piezas necesarias para construir una figura sencilla de Lego®, el servidor de piezas detallará el inventario de piezas para poder armarla, con
un máximo 100 piezas. El cliente deberá desplegar el detalle de las piezas necesarias para construir la figura solicitada, utilizando los nombres de las piezas provistos 
por el servidor. Los clientes serán capaces de obtener el listado de piezas de los servidores de piezas, en primera instancia y posteriormente de los servidores intermedios, deberán interactuar con ellos por medio del protocolo HTTP en un puerto TCP determinado. El servidor de piezas tendrá un inventario limitado de piezas para entregar a los clientes que será recargado al final del día. Los clientes indicarán al servidor la figura que quieren armar, de ser posible, el servidor devolverá un listado con todas las piezas necesarias para completar la figura, de lo contrario tendrá que informar al cliente que no es posible completar esa figura e indicar las piezas faltantes. El siguiente enlace tiene una muestra de un servidor de piezas.

## **Manual de Usuario**

---

### **Compilacion:**

El programa proporciona un Makefile que facilita la compilación través de la línea de comandos.
* Para compilar y generar el ejecutable, escriba en la terminal el comando **make**
* Para eliminar un ejecutable, escriba en la terminal el comando **make clean**

---

### **Ejecución:**

* Una vez compilado se genera el ejecutable Cliente.out, el cual se puede ejecutar escribiendo en la terminal **./Cliente.out**

---

### **Casos de prueba:**

Para los casos de prueba se intenta escribir entradas de texto no válidas que podrían lograr que el programa se caiga. Entre estos casos de prueba están, escribir caracteres numéricos, entradas de texto que no coincidan con ninguna figura, carácteres especiales etc. Para el manejo de estos casos aparte, se implementa un mensaje de error que imprime el siguiente texto en caso de que la entrada no coincida con alguna de las figuras disponibles: "La figura no existe o no se encontraron piezas de lego para esta figura".

## **Créditos**

Copyright 2023.

Gabriel Gonzáles Flores <gabriel.gonzalesflores@ucr.ac.cr>
Juan Aguilar Torres <juan.aguilartorres@ucr.ac.cr>
Sebastián Rodríguez Tencio <sebastian.rodrigueztencio@ucr.ac.cr>

Universidad de Costa Rica. CC BY 4.0