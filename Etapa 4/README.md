# Proyecto-Programado-CI0123

## **Descripción del problema**

El servidor inicia con los encabezados necesarios para la programación de sockets, la manipulación de cadenas, el manejo de archivos y el subprocesamiento. Una directiva del preprocesador define el número de puerto que se utilizará para el servidor. Se define una función llamada ”task” que acepta un puntero a un objeto Socket como argumento y es responsable de leer una cadena desde el socket, abrir y leer un archivo HTML almacenado en el servidor, en nuestro caso nos referimos a “Dalmata.html”, enviar el contenido del archivo de al cliente y cerrar el socket. La función principal define un puntero a un subproceso y dos punteros a objetos Socket. Se crea un objeto Socket llamado ”s1” con el argumento ’s’ para indicar que se utilizará como socket del servidor. Se llama al método Bind en el objeto de socket ”s1” para enlazarlo al número de puerto especificado. Se llama al método Listen en el objeto de socket ”s1” para establecer la cola de espera en 5 conexiones. Se llama al método SSLInitServer, cabe recalcar que estamos utilizando conexiones SSL para definir conexiones seguras, utilizamos un archivo “.pem” para esto. Se inicia un bucle infinito que espera conexiones de clientes utilizando el método Accept del objeto de socket ”s1”, para cada conexión de cliente, se crea un nuevo objeto Socket llamado ”cliente" utilizando el método Accept del objeto de socket ”s1”. Se llama al método SSLCreate en el objeto de socket ”cliente” para crear la conexión SSL. Se crea un nuevo subproceso utilizando el constructor std::thread, pasando la función ”task” y el objeto de socket ”cliente” como argumentos. La función principal termina, dejando que los subprocesos creados manejan las conexiones de los clientes.


## **Manual de Usuario**

---

### **Compilacion:**

El programa proporciona un Makefile que facilita la compilación través de la línea de comandos.
* Para compilar y generar el ejecutable, escriba en la terminal el comando **make SSLServer.out** y en otra terminal escriba **make SSLClient.out**
* Para compilar el cliente de NachOS es necesario levantar el servidor sin SSL, para esto se utiliza la instrucción **make NachosServer.out**
* Para eliminar un ejecutable, escriba en la terminal el comando **make clean**

---

### **Ejecución:**

* Una vez compilado se genera el ejecutable Cliente.out, el cual se puede ejecutar escribiendo en la terminal **./Cliente.out**
* Para correr el cliente de NachOS es necesario situarse en userprog y asegurarse de que el ejecutable **nachos** se encuentre, si no se puede generar haciendo **make**, una vez comprobado que se encuentre se procede a correr el siguiente comando **./nachos -x ../test/client**

---

### **Casos de prueba:**

Para los casos de prueba se intenta escribir entradas de texto no válidas que podrían lograr que el programa se caiga. Entre estos casos de prueba están, escribir caracteres numéricos, entradas de texto que no coincidan con ninguna figura, carácteres especiales etc. Para el manejo de estos casos aparte, se implementa un mensaje de error que imprime el siguiente texto en caso de que la entrada no coincida con alguna de las figuras disponibles: "La figura no existe o no se encontraron piezas de lego para esta figura".

## **Créditos**

Copyright 2023.

Gabriel Gonzáles Flores <gabriel.gonzalesflores@ucr.ac.cr>
Juan Aguilar Torres <juan.aguilartorres@ucr.ac.cr>
Sebastián Rodríguez Tencio <sebastian.rodrigueztencio@ucr.ac.cr>

Universidad de Costa Rica. CC BY 4.0
