#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main(int argc,char **argv)
{
  //////////////////////////////////////
  // Definir la direccion del server
  //////////////////////////////////////
  struct sockaddr_in my_server_addr;
  bzero(&my_server_addr,sizeof my_server_addr); 
  my_server_addr.sin_family=AF_INET;
  my_server_addr.sin_port=htons(22000);

  //Setear la direccion IP en my_server_addr a “127.0.0.1” (localhost) si el servidor esta en la misma maquina.
  // inet_pton - convierte direcciones IPv4/IPv6 de texto a forma binaria
  inet_pton(AF_INET,"192.168.1.61",&(my_server_addr.sin_addr));


  //////////////////////////////////////
  // Definir el socket
  //////////////////////////////////////

  int socket_client_fd;
  socket_client_fd = socket(AF_INET, SOCK_STREAM, 0);
 

  //la funcion connect() conecta al dispositivo cuya direccion y puerto es my_server_addr. 
  connect(socket_client_fd, (struct sockaddr *)&my_server_addr,sizeof(my_server_addr));

  //Ahora el cliente puede enviar y recibir datos  
  char sendline[100];
  char recvline[100];

  //////////////////////////////////////////
  // Empieza el ciclo principal del cliente
  //////////////////////////////////////////

  while(1){
    //Primero limpiar los buffers sendline and recvline
    bzero( sendline, 100);
    bzero( recvline, 100);

    //leer un string de stdin (la terminal) y guardarlo en sendline 
    //el usuario tiene que escribir algo
    fgets(sendline, 100, stdin); /*stdin = 0 , for standard input */

    //escribir  sendline en  socket_client_fd
    write(socket_client_fd, sendline, strlen(sendline)+1);
    
    //leer de  socket_client_fd  en srecvline
    read(socket_client_fd,recvline,100);

    //Imprimir en pantalla lo que lei
    printf("El servidor respondió:");
    printf("%s",recvline);
    
    /////////////////////////////////////////////////////////////
    //solucion
    /////////////////////////////////////////////////////////////
    //completar...
	
  }
  return 0; //EXIT_SUCCESS;   
}
