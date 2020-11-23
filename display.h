#ifndef display_h
#define display_h

unsigned char rs=1, win=0, prevdir=2, dir=2, death=0;;
unsigned char i, x[10]={40,0,0,0,0,0,0,0,0,0}, y[10]={50,0,0,0,0,0,0,0,0,0};
unsigned char contador, pointx, pointy, c=0, d=0, u=0;
unsigned char recibido, comando, command_data[10], command_data_index=0;
int points = 0;

void Pixel(unsigned char x, unsigned char y, unsigned char on);
void SendSerial(unsigned char *data, unsigned char lenght);
void Linea(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void Clear(void);
void PutChar(unsigned char x, unsigned char y, char ch);
void recibir_comando(void);
void Iniciar(void);
void GenerarPunto(void);
void MostrarPuntos(void);
void ReplayDraw(void);
void MyRectangle(unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2);
#define _XTAL_FREQ 4000000

#endif