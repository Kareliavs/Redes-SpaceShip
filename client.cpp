 /* Client code in C */
 
 #include <sys/types.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <unistd.h>

 //Cambiando a C++
 #include <iostream>
 #include <thread>
 #include "mio.cpp"
 using namespace std;
//COMPILAR g++ client.cpp -std=c++11  -pthread -lncurses -o c

 struct sockaddr_in stSockAddr;
 int Res;
 int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
 int n;
 int s=7;


 void update_bala(char dir,int a, int b)
 {
  switch (dir) 
  {
      case '8'://Arriba
        for(int i=0; i<a;i++)///a es y 
          {
              matrix[i][b]='_';
          }
        break;
      case '2':///abajo
        for(int i=a+2; i<dimension ;i++)
          matrix[i][b]='_';
        break;
      case '6': //derecha
        for(int i=b+2; i<dimension ;i++)
          matrix[a][i]='_';
        break;
      case '4': ///izquierda
        for(int i=0; i<b ;i++)
          matrix[a][i]='_';
         break;
      case '9':
        for(int i=b+2; i<dimension;i++)//derecha
          for(int j=a; j>=0; j--)//arriba
              if(i+j==a+b+1)
                  matrix[j][i]='_';
        break;
      case '3':
        for(int i=b+2; i<dimension;i++)//derecha
          for(int j=a; j<dimension; j++)//abajo
              if(j-i==a-b)
                  matrix[j][i]='_';
        break;
      case '1':
        for(int i=b-1; i>=0;i--)//izquierda
          for(int j=a; j<dimension; j++)//abajo
              if(i+j==a+b+1)
                  matrix[j][i]='_';
        break;
      case '7':
        for(int i=b-1; i>=0;i--)//derecha
          for(int j=a; j>=0; j--)//abajo
              if(j-i==a-b)
                  matrix[j][i]='_';
        break;
      default:
        break;
      
      
  }
 }
 void update(char dir, int x, int y, char dis)
 { 
         if(dir=='6')
         {
           matrix[x][mod(y-1,dimension)]='_';
           matrix[(x+1)][mod(y-1,dimension)]='_';
           if(y-1<0)
           {
             matrix[x][mod(y-2,dimension)]='_';
             matrix[(x+1)][mod(y-2,dimension)]='_';
           }
         }
         if(dir=='4')
         {
           matrix[x][mod(y+2,dimension)]='_';matrix[(x+1)][mod(y+2,dimension)]='_';
           if(y+2>dimension-1)
           {
             // cout<<"SUPER COUT!"<<mod(y+3,dimension)<<endl;
             matrix[x][mod(y+3,dimension)]='_';matrix[(x+1)][mod(y+3,dimension)]='_';
           }
   
         }
             if(dir=='8')  //esto es arriba
         {
           matrix[mod(x+2,dimension)][y]='_';matrix[mod(x+2,dimension)][(y+1)]='_';
           if(x+2>dimension-1)
           {
             matrix[mod(x+3,dimension)][y]='_';matrix[mod(x+3,dimension)][(y+1)]='_';
           }
         }
         if(dir=='5')  //8 es bajo
         {
           matrix[mod(x-1,dimension)][y]='_';matrix[mod((x-1),(dimension))][(y+1)]='_';
           if(x-1<0)
           {
             matrix[mod(x-2,dimension)][y]='_';matrix[mod((x-2),(dimension))][(y+1)]='_';
           }
         }
   
 
 }

 void writeS(char id[1])
 {   
     while(true)
     {
       //cout<<"HERE"<<endl;
       string buffer;
      
       //initscr ();
       teclas();
       //cbreak ();
       buffer="";
       buffer[0]=id[0];
       main_no_main(buffer);
       n = write(SocketFD, buffer.c_str(),7);
      //cout<<"BUFFER "<<buffer<<endl;
       //cin>>n;
       /* perform read write operations ... */
       
     }

 }
 void readS(char id[1])
 {  
    while(true)
    {
      string buffer;
      char* buff;
      buff=new char[s];
      //cout<<"BufferREadS: "<<buffer<<endl;
      n = read(SocketFD,buff,7);
      string aux(buff);
      buffer=aux;
      if (n < 0) perror("ERROR reading from socket");
      //cout<<"EXPLOTO"<<buffer<<endl;
      //cin>>n;
      string ac ;
      string ab ;
      ac += buffer[2];
      ac += buffer[3];
      ab += buffer[4];
      ab += buffer[5];
      //cout<<"ac: "<<ac<<" ab: "<<ab<<endl;
      int a = stoi(ac);                  //De char a entero
      int b = stoi(ab);
      //cout<<"a: "<<a<<" b: "<<b<<endl;
      //cout<<"A "<<a<<" B "<<b<<endl;

      cuadrado(a,b,'A'+ buffer[0]);
      update(buffer[6],a,b,buffer[1]);
      
      if(buffer[1]=='S')balas(a,b, buffer[6],id,SocketFD);
      imprimir();
      update_bala(buffer[6],a,b);

      
      //cout<<"Server replay: "<<buffer<<endl;
     
    }
 }


 int main(void)
 {

   initscr ();  
   inicia(dimension);


   if (-1 == SocketFD)
   {
     perror("cannot create socket");
     exit(EXIT_FAILURE);
   }

   memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

   stSockAddr.sin_family = AF_INET;
   stSockAddr.sin_port = htons(1100);
   Res = inet_pton(AF_INET, "192.168.199.171", &stSockAddr.sin_addr);

   if (0 > Res)
   {
     perror("error: first parameter is not a valid address family");
     close(SocketFD);
     exit(EXIT_FAILURE);
   }
   else if (0 == Res)
   {
     perror("char string (second parameter does not contain valid ipaddress");
     close(SocketFD);
     exit(EXIT_FAILURE);
   }

   if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
   {
     perror("connect failed");
     close(SocketFD);
     exit(EXIT_FAILURE);
   }
   char id_usuario[1];
   int cosa_rara = read(SocketFD,id_usuario,1);
   if (cosa_rara < 0) perror("ERROR reading from socket");

   //cout<<"AQUI "<<id_usuario<<endl;
   std::thread t1 (readS,id_usuario); 
   std::thread t2 (writeS,id_usuario); 
   t1.join();
   t2.join();
     /*t1.join();
     t2.join();
     t3.join();*/

   shutdown(SocketFD, SHUT_RDWR); 
   close(SocketFD);
   endwin ();
   return 0;
 }