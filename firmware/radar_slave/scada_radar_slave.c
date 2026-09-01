#include <18F4550.h>
#fuses INTRC,NOLVP,NOPROTECT,NOWDT
#use delay(internal = 4MHz)

#use rs232(xmit = PIN_b2, rcv = PIN_b3, baud =9600, bits = 8, parity = N)
#BYTE SSPBUF = 0xFC9

#define GLCD_DI  PIN_A0   // Data or Instruction input
#define GLCD_RW  PIN_A1   // Read/Write
#define GLCD_E   PIN_A2   // Enable
#define GLCD_CS1 PIN_A3   // Chip Selection 1
#define GLCD_CS2 PIN_A4   // Chip Selection 2
#define GLCD_RST PIN_E0   // Reset
// comandos
#define RADAR    3
#define limit    3

#include <glcd.c>

int data, dist=3;
int16 ii=2, jj=2;
void glcd_semicircle(int x, int y, int radius, int1 fill, int1 color);
void R();

#int_ssp
void spi_rcv()
{  
   data = SSPBUF;
   
   if ( data!=0 && data <=29 )
   {
      dist = SSPBUF;
      glcd_circle(63,72-(dist*2),2,1,1);
      printf("\n\rCoordenda: %i\n\r",dist);
   }
}

void main()
{
   
   setup_spi(spi_slave | spi_L_to_H);
   glcd_init(on);
   enable_interrupts(INT_SSP);
   enable_interrupts(GLOBAL);
   glcd_rect(0,0,127,63,0,1); 
   R();
   while(TRUE)
   {
      for(int kk=0; kk<limit; kk++){glcd_semicircle(64,62,ii++,0,1);}
      for(int kk=0; kk<limit; kk++){glcd_semicircle(64,62,jj++,0,0);}
      if(ii>=61){ii=0;jj=0;}
      R();
   }
}

void glcd_semicircle(int x, int y, int radius, int1 fill, int1 color)
{
   signed int a, b, P;
   a = 0;
   b = radius;
   P = 1 - radius;
   
   do
   {
      glcd_pixel(b+x, y-a, color);
      glcd_pixel(a+x, y-b, color);
      glcd_pixel(x-a, y-b, color);
      glcd_pixel(x-b, y-a, color);
      
      if(P < 0)
         P += 3 + 2*a++;
      else
         P += 5 + 2*(a++ - b--);
   } while(a <= b);
}

void R()
{
   for(int i=15;i<=65;i+=20)
   {
      glcd_semicircle(63,63,i,NO,YES);
   }
}



