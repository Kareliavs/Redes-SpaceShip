/* Server code in C++ */
// Compilar: g++ -std=c++11 -pthread -o s.exe server.cpp
//  Ejecutar: ./s.exe
 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <chrono>

//Cambiando a C++
#include <iostream>
#include <thread>         // std::thread, std::this_thread::sleep_for
#include <vector>
using namespace std;


struct sockaddr_in stSockAddr;                
int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); //Se trabaja con protocolo TCP Handshaking
char buffer[7];
int n;
vector<int> iD;


void aceptClient(int ConnectFD) {
  do {
   //bzero(buffer,256); //Llena de 0 la cadena
     n = read(ConnectFD,buffer,sizeof(buffer));
     if (n < 0) perror("ERROR reading from socket");
     cout<<"Client "<<ConnectFD<< " replay: "<<buffer<<endl;
     //out<<"Server Write your Message "<<endl;
     
     char id_usuario[7];
     char aux[7];
     sprintf(id_usuario,"%d",ConnectFD); //De entero a char
     cout<<"id_usuario: "<<id_usuario<<endl;
     strcat(id_usuario,buffer);

     for(int i=0;i<7;i++)
      buffer[i] = id_usuario[i];
     cout<<"Nuevo buffer: "<<buffer<<endl;;
     for(int i=0;i<iD.size();i++)
     {
         cout<<"Enviando a Cliente: "<<iD[i]<<endl;
         char user = '0' + ConnectFD;
         buffer[0] = user;
         n = write(iD[i],buffer,sizeof(buffer));
         if (n < 0) perror("ERROR writing to socket");
      
     }
 
     /* perform read write operations ... */

 } while(buffer != "END");

    shutdown(ConnectFD, SHUT_RDWR);

    close(ConnectFD); //Cierra el Socket ( Socket : puente para que 2 computadoras se comuniquen remota o localmente) CIERRA la comunicación
}

  

int main(void)
{
  /*
  This function creates a socket and specifies communication style style, 
  which should be one of the socket styles listed in Communication Styles. (TCP en este caso)
  The namespace argument specifies the namespace; it must be PF_LOCAL (see Local Namespace) or PF_INET (see Internet Namespace). 

  */

  if(-1 == SocketFD)
  {
    perror("can not create socket");
    exit(EXIT_FAILURE);
  }

  memset(&stSockAddr, 0, sizeof(struct sockaddr_in)); //Separa espcio en memoria para la Estructura

  stSockAddr.sin_family = AF_INET;      //Protocolo para la conexión
  stSockAddr.sin_port = htons(1100);      //Puerto para la conexión
  stSockAddr.sin_addr.s_addr = INADDR_ANY;

  if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
  {
    perror("error bind failed");
    close(SocketFD);
    exit(EXIT_FAILURE);
  }

  if(-1 == listen(SocketFD, 10)) 
  {
    perror("error listen failed");
    close(SocketFD);
    exit(EXIT_FAILURE);
  }


//Hace que el Servidor siempre escuche

while(1)
{
  int ConnectFD = accept(SocketFD, NULL, NULL);
  
  if(0 > ConnectFD) {
    perror("error accept failed");
    close(SocketFD);
    exit(EXIT_FAILURE);
  } else {
    iD.push_back(ConnectFD);
    std::thread (aceptClient, ConnectFD).detach();
  }
}

close(SocketFD);
return 0;
}