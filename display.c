#include<xc.h>
#include"display.h"
#include <stdlib.h>
#include <stdio.h>

//static unsigned long next = 1;
//
//int myrand(unsigned char rand_max) {
//    next = next * 1103515245 + 12345;
//    return((unsigned)(next/65536) % rand_max);
//}

void Pixel(unsigned char x, unsigned char y, unsigned char on)
{
    unsigned char datos[3];
    datos[1]=x;
    datos[2]=y;
    if(on) datos[0]=128;
    else datos[0]=129;
    SendSerial(datos,3);
}

void SendSerial(unsigned char *data, unsigned char lenght)
{
    unsigned char x=0;
    while(x<lenght){
        while(PIR1bits.TXIF==0){}        
        TXREG=data[x];        
        x++;
    }            
}

//void Linea(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
//{
//    unsigned char datos[5];
//    datos[0]=131;
//    datos[1]=x1;
//    datos[2]=y1;
//    datos[3]=x2;
//    datos[4]=y2;
//    SendSerial(datos,5);
//}

void Clear(void)
{
    unsigned char datos[1];
    datos[0]=134;
    SendSerial(datos,1);   
}

void PutChar(unsigned char x, unsigned char y, char ch)
{
    unsigned char datos[4];
    datos[0]=130;
    datos[1]=x;
    datos[2]=y;
    datos[3]=ch;
    SendSerial(datos,4);   
    
}
 
void recibir_comando(){
    if(RCIF==1){
        recibido=RCREG;
        if(recibido>127){
            comando=recibido;
            command_data_index=0;
        }else{
            command_data[command_data_index]=recibido;
            if(command_data_index<9) command_data_index++;
        }
        if(comando==200 && command_data_index==2){
            //Recibe comando de toque por parte del display
            if(command_data[0]>=96 && command_data[0]<=104 && command_data[1]>=93 && command_data[1]<=103){
                //FLECHA ARRIBA
                dir=1;
            }
            if(command_data[0]>=106 && command_data[0]<=114 && command_data[1]>=105 && command_data[1]<=115){
               //FLECHA DERECHA
                dir=2;
            }
            if(command_data[0]>=96 && command_data[0]<=104 && command_data[1]>=105 && command_data[1]<=115){
                //FLECHA ABAJO
                dir=3;
            }
            if(command_data[0]>=86 && command_data[0]<=94 && command_data[1]>=105 && command_data[1]<=115){
               //FLECHA IZQUIERDA
                dir=4;
            }
            if(command_data[0]>=40 && command_data[0]<=68 && command_data[1]>=50 && command_data[1]<=61 && win==1){
               //RESTART
                win=0;
                rs=1;
            }

        }
    }
}

void GenerarPunto(void){
    pointx=rand() % 78 + 5;
    pointy=rand() % 98 + 17;
    Pixel(pointx,pointy,1);
}

void Iniciar(void){
    x[0]=40; y[0]=50;
    Pixel(x[0],y[0],1);
    GenerarPunto();
}

void MostrarPuntos(){
    c=points/100;
    d=points/10;
    u=points%10;
    PutChar(86,16,(c+48));
    PutChar(92,16,(d+48));
    PutChar(98,16,(u+48));
}

void MyRectangle(unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2){
        for(i=x1; i<x2; i++){
            Pixel(i,y1,1);
            Pixel(i,y2,1);
        }
        for(i=y1; i<y2; i++){
            Pixel(x1,i,1);
            Pixel(x2,i,1);
        }
    }