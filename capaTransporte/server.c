/*Librerias requeridas*/
#include <sys/socket.h>
#include <netdb.h>
//GNU\Linux implementa el protocolo IP, version 4, descrito en RFC 791 y RFC 1122.
//Tambien contiene un router IP y un filtro de paquetes. 
//referencia: https://linux.die.net/man/7/ip

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


int main()
{
  char str[100];

  //////////////////////////////////////
  // Definir la direccion del server
  //////////////////////////////////////

  struct sockaddr_in  my_server_addr;
  //Struct para contener una direccion IP y un numero de Puerto
  // struct sockaddr_in {
  //    sa_family_t    sin_family; /* address family: AF_INET */
  //    struct in_addr sin_addr;   /* internet address */
  //    in_port_t      sin_port;   /* port in network byte order ; in_port_t es equivalente al tipo uint16_t */
  //};
  // Internet address.
  //struct in_addr {
  //    uint32_t       s_addr;     /* address in network byte order */
  //};

  //Limpiar o borrar my_server_addr.
  bzero( &my_server_addr, sizeof(my_server_addr));

  //Setear el esquema de direcciones a – AF_INET ( IP )
  //Permitir conectarse a cualquier IP – htons(INADDR_ANY)
  //Escuchar en el puerto 22000 – htons(22000)
  //La funcion htons() se asegura de que los numeros son almacenados en memoria en orden "network byte order", que es el byte mas significativo primero (big-endian).
  my_server_addr.sin_family = AF_INET;
  my_server_addr.sin_addr.s_addr = htons(INADDR_ANY);
  my_server_addr.sin_port = htons(22000);


  
  //////////////////////////////////////
  // Definir el socket
  //////////////////////////////////////


  //File Descriptors (descriptores de archivo) a ser usados
  //En la familia Unix de sistemas operativos, un file descriptor (FD) es un indicador abstracto (handle) usado para acceder a un archivo oa algun otro recurso input/output, como por ejemplo un socket. Los File Descriptors forman parte de la interfaz POSIX. Un File Descriptor se implementa como un entero no-negativo
  int listen_fd, comm_fd;
  
  //Cada servidor necesita escuchar “listen” por conecciones. La siguiente funcion crea un socket 
  listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  //AF_INET, es una familia de direcciones que es usada para designar el tipo de direcciones a los que se puede comunicar tu socket (en este caso, direcciones Internet Protocol v4). El kernel de Linux, por ejemplo, soporta otras 29 familias tales como sockets UNIX (AF_UNIX) y IPX (AF_IPX), y tambien comunicaniones con IRDA y Bluetooth (AF_IRDA and AF_BLUETOOTH). Existe tambien AF_INET6 para direcciones Internet Protocol v6.
  //SOCK_STREAM, corresponde a la conexion TCP
  //Los datos de todos los dispositivos que Data desean conectarse a este socket seran redirigidos a listen_fd.
  
  bind(listen_fd, (struct sockaddr *) &my_server_addr, sizeof(my_server_addr));
  //Prepara para escuchar por conecciones de direcciones/puertos especificados en my_server_addr ( cualquier IP en el puerto 22000 ).
  //int bind(int listen_fd, const struct sockaddr *my_server_addr, socklen_t addrlen);
  //bind() asigna la direccion especifica my_server_addr al socket referenciado por el file
  //      descriptor listen_fd.  Tradicionalmente, esta operacion se llama  “asignar un nombre al socket”.

  listen(listen_fd, 10);
  //Empiezo a escuchar por connecciones
  //listen()  marca el  socket referenciado por listen_fd como un socket que sera usado para aceptar hasta 10 conecciones

    
  comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
  //Acepta una coneccion, si no hay nadie espera. Se retorna un file descriptor comm_fd que se usa para comunicarse. Todo lo que se envia al servidor se puede leer de comm_fd, todo lo que se escriba en comm_fd es enviado al otro dispositivo que se conectó.


  ///////////////////////////////////////////
  // Empieza el ciclo principal del servidor
  ///////////////////////////////////////////

  bool salir_ciclo = false;
  while(!salir_ciclo){
    //El servidor hace lo siguiente en cada iteracion

    //limpiamos el buffer str
    //la funcion bzero() pone los primeros 100 bytes de str en zero (bytes conteniendo '\0')
    bzero(str, 100);

    //read(int comm_fd, void *str, size_t count);
    //la funcion read() lee hasta 100 bytes del file descriptor comm_fd y lo guarda en el buffer str
    //si tiene exito, el numero de bytes leidos es retornado 
    read(comm_fd, str, 100);

    //mostrar lo que leemos del cliente
    printf("Un cliente dice: %s",str);
        
	
    //Ahora proceso lo que nos envio el cliente
    //creo una variable target y copio ahi los primeros 5 caracteres de los que nos envio el cliente
    char target[100];
    strncpy(target, str, 5);
    target[5] = '\0'; //importante para terminar el string

    //ahora comparo target con la palabra salir
    if(strcmp(target,"salir")==0){
      write(comm_fd, "chau\n", strlen("chau\n")+1); 
      salir_ciclo=true;
    }
    else{
      //devolvemos un eco al cliente
      write(comm_fd, str, strlen(str)+1);
    }
  }
  printf("Adios amigos!\n");    
  return 0; //EXIT_SUCCESS;
}
