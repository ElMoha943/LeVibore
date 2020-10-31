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

#include<xc.h>
#include"display.h"

void main(){
    unsigned char x, y, recibido, comando, command_data[10], command_data_index=0, dir=2, contador;
    int points = 0;
    
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
    Rectangulo(4,16,80,100); //enrealidad dibuja 83-115, no se porque
    PutChar(86,16,'0');
    PutChar(92,16,'0');
    PutChar(98,16,'0');
    
    //Dibujar controles
    //up
    for(x=96; x<104; x++){
        Pixel(x,93,1);
        Pixel(x,103,1);
    }
    for(x=93; x<103; x++){
        Pixel(96,x,1);
        Pixel(104,x,1);
    }
    PutChar(98,95,'U');
    //right
    for(x=106; x<114; x++){
        Pixel(x,105,1);
        Pixel(x,115,1);
    }
    for(x=105; x<115; x++){
        Pixel(106,x,1);
        Pixel(114,x,1);
    }
    PutChar(108,107,'R');
    //down
    for(x=96; x<104; x++){
        Pixel(x,105,1);
        Pixel(x,115,1);
    }
    for(x=105; x<115; x++){
        Pixel(96,x,1);
        Pixel(104,x,1);
    }
    PutChar(98,107,'D');
    //left
    for(x=86; x<94; x++){
        Pixel(x,105,1);
        Pixel(x,115,1);
    }
    for(x=105; x<115; x++){
        Pixel(86,x,1);
        Pixel(94,x,1);
    }
    PutChar(88,107,'L');
    
    // 86 - 105
    //Rectangulo(90,16,110,26);
    //Rectangulo(100,16,110,20); NO ANDA, DIBUJA EN 85;16 - NULL;25
    
    //Cursor
    x=40; y=50;
    Pixel(x,y,1);
    while(1){
        //Timer Setup.
        if(T0IF==1)
        {
            TMR0=TMR0+131;
            T0IF=0;
            contador++;
            if (contador==25){
                contador=0;
                Pixel(x,y,0);
                switch(dir){
                    case 1:
                        y--;
                        break;
                    case 2:
                        x++;
                        break;
                    case 3:
                        y++;
                        break;
                    case 4:
                        x--;
                        break;
                    default:
                        break;
                }
                if(x<5)x=82;
                if(x>82)x=5;
                if(y<17)y=114;
                if(y>114)y=17;
                Pixel(x,y,1);
            }
        }
        //RECIBIR COMANDOS POR COM
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
            }
        }
    }
}