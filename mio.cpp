#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <curses.h>
#include <unistd.h>
//compila asi g++ mio.cpp -lncurses -o a
using namespace std;
vector< vector<char> > matrix;
int dimension=50;
int dispara=0;
int direccion=0;
vector <char> naves;
int tecla=0;
char space= '_';
char hurt;

int mod (int a,int b )
{
    if(a<0)
    {
        return a+b;
    }
    return a%b;
}

void inicia(int dim)
{

     vector<char> linea;
     matrix.clear();
     for(int i =0; i<dim; i++)
     {
        linea.push_back(space);

     }
     for(int i =0; i<dim; i++)
     {
        matrix.push_back(linea);
     }
}

void imprimir ()
{   
    //system ("/usr/bin/clear");
    //cout<<"*******************************************************"<<endl;
    clear();
    for(int i =0; i<dimension; i++)
     {  
        for(int j =0; j<dimension; j++)
         {
            //cout<<matrix[i][j];
            printw ("%c", matrix[i][j]);
         }
        // cout<<endl;
         //printf("\n");
         printw("\n");
     }
     //cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++w"<<endl;
     cbreak();
     refresh();
     //cout<<endl;
     
}

void balas (int a , int b, int direccion,char id[1] , int SocketFD)
{   //inicia(dimension);
    hurt=0;
    char bala= '|';
    //cout<<"HERE"<<direccion<<endl;
    int dir=direccion-48;
    switch (dir) 
    {
        case 8://Arriba

          for(int i=0; i<a;i++)///a es y 
            {   
                if(matrix[i][b]!= space)
                {
                    hurt=matrix[i][b]-'A';
                    matrix[i][b]=hurt;
                    string herido;
                    herido[0] = id[0];
                    herido[1] = 'H';
                    herido[2] = hurt;
                    write(SocketFD, herido.c_str(),3);

                }
                else
                    matrix[i][b]=bala;
                  /*sleep(1/2);
                matrix[i][b]='-';*/
            }
          break;
        case 2:///abajo
          for(int i=a+2; i<dimension ;i++)
          {
              if(matrix[i][b]!= space)
              {
                 hurt= matrix[i][b] -'A';
                 matrix[i][b]=hurt;

              }
              else
                 matrix[i][b]=bala;
          }
          break;
        case 6: //derecha
          for(int i=b+2; i<dimension ;i++)
          {
              if(matrix[a][i]!=space)
              {
                hurt= matrix[a][i]-'A';
                matrix[a][i]=hurt;
              }
              else
                matrix[a][i]=bala;
          }
          break;
        case 4: ///izquierda
          for(int i=0; i<b ;i++)
          {
              if(matrix[a][i]!=space)
              {
                hurt= matrix[a][i]-'A';
                matrix[a][i]=hurt;
              }
              else
                 matrix[a][i]=bala;
          }
           break;
        case 9:
          for(int i=b+2; i<dimension;i++)//derecha
            for(int j=a; j>=0; j--)//arriba
                if(i+j==a+b+1)
                    matrix[j][i]=bala;
          break;
        case 3:
          for(int i=b+2; i<dimension;i++)//derecha
            for(int j=a; j<dimension; j++)//abajo
                if(j-i==a-b)
                    matrix[j][i]=bala;
          break;
        case 1:
          for(int i=b-1; i>=0;i--)//izquierda
            for(int j=a; j<dimension; j++)//abajo
                if(i+j==a+b+1)
                    matrix[j][i]=bala;
          break;
        case 7:
          for(int i=b-1; i>=0;i--)//derecha
            for(int j=a; j>=0; j--)//abajo
                if(j-i==a-b)
                    matrix[j][i]=bala;
          break;
        default:
          break;
        
        
    }
    
}

int a=0;
int b=0;    
vector< pair <int, pair<int, int> > > usuarios;
int teclas ()
{
    
    //initscr ();
    //cbreak ();
    tecla = getch ();

    if(tecla==115)
    {
        a=(a+1)%(dimension-1);//S
        direccion=5;
    }
    if(tecla==97 )
    {   b=(b-1)%(dimension-1);//A
        direccion=4;
    }
    if(tecla==119)
    {   
        a=(a-1)%(dimension-1);//W
        direccion=8;
    }
    if(tecla==100)
    {
        b=(b+1)%(dimension-1);//D
        direccion=6;
    }
    if(tecla==56)dispara=8; //8
    if(tecla==57)dispara=9; //9
    if(tecla==54)dispara=6; //6
    if(tecla==51)dispara=3; //3
    if(tecla==50)dispara=2; //2
    if(tecla==49)dispara=1; //1
    if(tecla==52)dispara=4; //4
    if(tecla==55)dispara=7; //7
    if(a<0)a=dimension-2;
    if(b<0)b=dimension-2;
    //endwin ();
}

void cuadrado( int x, int y, char letra )
{   //inicia(dimension);
    matrix[x][y]=letra;
    matrix[x+1][y]=letra;
    matrix[x][y+1]=letra;
    matrix[x+1][y+1]=letra;
    //teclas();
    //imprimir();
}
void main_no_main(string &buffer)
{
    //cout<<"HEREL"<<endl;
    //teclas();
    string integer_string;

    
    if(dispara>0)buffer[1]='S';//strcat(buffer,"1");
    else         buffer[1]='M';//strcat(buffer,"0");

    //sprintf(integer_string, "%d", a);
    integer_string=to_string(a);
    if(a<=9)
    {
      buffer[2]='0';
      buffer[3]=integer_string[0];
    } 
    else
    {
      buffer[2]=integer_string[0];
      buffer[3]=integer_string[1];
    }

    //strcat(buffer,integer_string); 
    integer_string=to_string(b);

    if(b<=9)
    {
      buffer[4]='0';
      buffer[5]=integer_string[0]; 

    } 
    else
    {
      buffer[4]=integer_string[0];
      buffer[5]=integer_string[1];
    }
    //sprintf(integer_string, "%d", b);
    //strcat(buffer,integer_string); 
    
    if(dispara>0)
    {
        integer_string=to_string(dispara);
        //sprintf(integer_string, "%d", dispara);
        //strcat(buffer,integer_string); 
        buffer[6]=integer_string[0];
    }
    else
    {
        //sprintf(integer_string, "%d", direccion);
        integer_string=to_string(direccion);
        //strcat(buffer,integer_string); 
        buffer[6]=integer_string[0];
    }
   
    //if(dispara>0) balas(a,b, dispara);
    dispara=0;
    //imprimir();

}