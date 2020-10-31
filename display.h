#ifndef display_h
#define display_h

void Pixel(unsigned char x, unsigned char y, unsigned char on);
void SendSerial(unsigned char *data, unsigned char lenght);
void Linea(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void Rectangulo(unsigned char x, unsigned char y, unsigned char w, unsigned char h);
void Clear(void);
void PutChar(unsigned char x, unsigned char y, char ch);

#define _XTAL_FREQ 4000000

#endif