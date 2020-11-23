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
    
    //INICIALIZA EL JUEGO
    Iniciar();
    
    while(1){
        //Timer Setup.
        if(T0IF==1)
        {
            TMR0=TMR0+131;
            T0IF=0;
            contador++;
            //contador de 1 segundo
            if (contador==25 && death!=1){
                contador=0;
                //BORRA ANTIGUA VIBORA
                for(i=0;i<=cont2;i++){
                    Pixel(x[i],y[i],0);
                }
                //DESPLAZAMIENTO
                for(i=cont2;i>0;i--){
                    y[i]=y[i-1];
                    x[i]=x[i-1];
                }
                switch(dir){
                    case 1:
                        y[0]--;
                        break;
                    case 2:
                        x[0]++;
                        break;
                    case 3:
                        y[0]++;
                        break;
                    case 4:
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
                    if(cont==cont2+1){
                        cont2=points;;
                        cont=0;
                        flag=0;
                    }
                }
            }
        }
        //RECIBIR COMANDOS POR COM
        if(RCSTAbits.OERR==1){
            RCSTAbits.CREN=0;                
            RCSTAbits.CREN=1;
        }
        recibir_comando();
        //SISTEMA DE COLISIONES
        for(i=1;i<=points;i++){
            if(x[0]==x[i] && y[0]==y[i]){
                death=1;
                break;
            }
        }
        if(death==1){
            PutChar(6,1,'P');
            PutChar(12,1,'E');
            PutChar(18,1,'R');
            PutChar(24,1,'D');
            PutChar(30,1,'I');
            PutChar(36,1,'S');
            PutChar(42,1,'T');
            PutChar(48,1,'E');
            ReplayDraw();
            while(death==1){
                //ESPERAR A QUE SE PRESIONE EL BOTON REPLAY
                recibir_comando();
            }
            Iniciar();
        }
        //SISTEMA DE PUNTOS
        if(x[0]==pointx && y[0]==pointy){
            Pixel(pointx,pointy,0); 
            flag=1;
            points++;
            if(points==10){
                win=1;
            }
            else
            {
                GenerarPunto();
            }
            MostrarPuntos();
        }
        if(win==1){
            PutChar(6,1,'G');
            PutChar(12,1,'A');
            PutChar(18,1,'N');
            PutChar(24,1,'A');
            PutChar(30,1,'S');
            PutChar(36,1,'T');
            PutChar(42,1,'E');
            ReplayDraw();
            while(win==1){
                //ESPERAR A QUE SE PRESIONE EL BOTON REPLAY
                recibir_comando();
            }
            Iniciar();
        }   
    }
}