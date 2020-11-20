// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include "display.h"

void main(){
    unsigned int flag=0, cont=0, cont2=0;
    TRISAbits.TRISA0=1;
    ANSEL=0;
    ANSELH=0;
    OPTION_REG=0x04;
    //Configura UART a 9600 baudios
    TXSTAbits.TXEN=1;
    TXSTAbits.BRGH=1;
    RCSTAbits.SPEN=1;
    RCSTAbits.CREN=1;
    BAUDCTLbits.BRG16=0;
    SPBRG=25;
    SPBRGH=0;
    
    //Borra display
    Clear();
    
    //Dibujar pantalla
    MyRectangle(4,83,16,115);
//    Rectangulo(4,16,80,100); //DIBUJA 83-115 (ES ANCHO Y LARGO, NO COORDENADAS)
    PutChar(86,16,'0');
    PutChar(92,16,'0');
    PutChar(98,16,'0');
    
    //Dibujar controles
    //up
    MyRectangle(96,104,93,103);
    PutChar(98,95,'U');
    //right
    MyRectangle(106,114,105,115);
    PutChar(108,107,'R');
    //down
    MyRectangle(96,104,105,115);
    PutChar(98,107,'D');
    //left
    MyRectangle(86,94,105,115);
    PutChar(88,107,'L');
    
    //Iniciar (cursor y primer punto)
    Iniciar();
    
    while(1){
//        do{
            //Timer Setup.
            if(T0IF==1)
            {
                TMR0=TMR0+131;
                T0IF=0;
                contador++;
                //contador de 1 segundo
                if (contador==25){
                    contador=0;
                    //BORRA ANTIGUA VIBORA
                    for(i=0;i<=cont2;i++){
                        Pixel(x[i],y[i],0);
                    }
                    //DESPLAZAMIENTO
                    switch(dir){
                        case 1:
                            for(i=1;i<=cont2;i++){
                                y[i]=y[i-1];
                                x[i]=x[i-1];
                            }
                            y[0]--;
                            break;
                        case 2:
                            for(i=1;i<=cont2;i++){
                                y[i]=y[i-1];
                                x[i]=x[i-1];
                            }
                            x[0]++;
                            break;
                        case 3:
                            for(i=1;i<=cont2;i++){
                                y[i]=y[i-1];
                                x[i]=x[i-1];
                            }
                            y[0]++;
                            break;
                        case 4:
                            for(i=1;i<=cont2;i++){
                                y[i]=y[i-1];
                                x[i]=x[i-1];
                            }
                            x[0]--;
                            break;
                        default:
                            break;
                    }
                    //ENTRA POR POR AQUI Y SALE POR AHI
                    for(i=0;i<=cont2;i++){
                        if(x[i]!=0 && y[i]!=0){
                            if(x[i]<5)x[i]=82;
                            if(x[i]>82)x[i]=5;
                            if(y[i]<17)y[i]=114;
                            if(y[i]>114)y[i]=17;
                            //GRAFICA
                            Pixel(x[i],y[i],1);
                        }
                    }
                    if(flag==1){
                        cont++;
                        if(cont==cont2++){
                            cont2=points;;
                            cont=0;
                            flag=0;
                        }
                    }
                }
            }
            //RECIBIR COMANDOS POR COM
            recibir_comando();
            //SISTEMA DE PUNTOS
            if(x[0]==pointx && y[0]==pointy){
                Pixel(pointx,pointy,0); 
                flag=1;
                points++;
//                if(points==10){
//                    win=1;
//                    //BORRAR VIBORA
//                    for(i=0;i<=points;i++){
//                        Pixel(x[i],y[i],0);
//                    }
//                    //DIBUJAR BOTON DE REPLAY
//                    MyRectangle(40,68,50,61);
//                }
//                else
//                {
                    GenerarPunto();
//                }
                MostrarPuntos();
            }
//            if(win==1){
//                while(win==1){
//                    //ESPERAR A QUE SE PRESIONE EL BOTON REPLAY
//                    recibir_comando();
//                }
//                NoRectangle(40,68,50,61);
//                Iniciar();
//            }
            //REINICIAR JUEGO
//        }while(rs==1); //REINICIAR PARTIDA?     
    }
}