#include <18F46k22.h>
#fuses INTRC,NOLVP,NOPROTECT,NOWDT
#use delay(internal = 4MHz)

#use rs232(xmit = PIN_C6, rcv = PIN_C7, baud =9600, bits = 8, parity = N)

#include <math.h>

#define SLAVE1 PIN_B2
#define SLAVE2 PIN_D2
#define SLAVE3 PIN_A2

// comando para el esclavo
#define ULTRA    1

#define MOTOR    2
#define ADELA    254
#define ATRAS    253
#define FULLL    252
#define PAROO    0

#define RADAR    3

#define pilon  0.8
#define demons 20

char cc = ' ';
float32 distancia=0.0, distd=0.0;
short bandera=0, posicion=0;
int32 vel=0;
int distanciaa=0, MIKE=0;

#INT_RDA
void datosrec()
{
   if (kbhit())          
   {
      cc=getc();
      printf("%c",cc);
      
      if(cc == 'I')
      {
         bandera = 1;
         printf("\n\rBandera inicio\n\r");
      }
      if(cc == 'P')
      {
         bandera = 0;
         output_low(SLAVE2);
         spi_write(PAROO);
         output_high(SLAVE2);
         delay_us(100);
         printf("\n\rPAROOO\n\r"); 
      }
      if(cc == 'U')
      {
         distd = 7.0;posicion=1;
         printf("\n\rDistancia 1\n\r");
      }
      if(cc == 'D')
      {
         distd = 15.0;posicion=1;
         printf("\n\rDistancia 2\n\r");
      }
      if(cc == 'T')
      {
         distd = 19.0;posicion=1;
         printf("\n\rDistancia 3\n\r");
      }
      if(cc == 'R')
      {
         output_low(SLAVE2);
         spi_write(PAROO);
         output_high(SLAVE2);
         delay_us(100);
         output_low(SLAVE2);
         vel=0;
         spi_write(vel);
         output_high(SLAVE2);
         delay_us(100);
         cc = ' ',distancia=0.0, distd=0.0, bandera=0, posicion=0, vel=0, distanciaa=0, MIKE=0;
      }
   }
}

void main ()
{
   output_high(SLAVE1);
   output_high(SLAVE2);
   output_high(SLAVE3);
   enable_interrupts(GLOBAL);
   enable_interrupts(INT_RDA);
   
   printf("\n\rComandos validos\n\r");
   delay_ms(1);
   printf("I: Start\n\r");
   delay_ms(1);
   printf("P: Paro\n\r");
   delay_ms(1);
   printf("U: Distancia 1\n\r");
   delay_ms(1);
   printf("D: Distancia 2\n\r");
   delay_ms(1);
   printf("T: Distancia 3\n\r");
   
   setup_spi(spi_master | spi_l_to_h | spi_clk_div_4);
  
   
   while(true)
   {
      if(bandera == 1)
      {
         output_low(SLAVE1);
         spi_write(ULTRA);
         output_high(SLAVE1);
         delay_us(100);
         output_low(SLAVE1);
         distanciaa = spi_read(0);
         output_high(SLAVE1);
         MIKE = distanciaa / 10 * 2;
         
         if (MIKE <= 30 )
         {
            output_low(SLAVE3);
            spi_write(MIKE);
            output_high(SLAVE3);
            delay_us(100);
            printf("\n\rCoordenda: %i\n\r",MIKE);
         }
         distancia = distanciaa / 10.0 * 2.0;
         printf("Valor de la distancia: %0.2f \n\r", distancia);
         
         if( (distancia+pilon >= distd && (distancia-pilon<= distd+pilon)) && posicion == 1 && bandera == 1 )
         {  
            output_low(SLAVE2);
            spi_write(MOTOR);
            output_high(SLAVE2);
            delay_us(100);
            printf("\n\rTIEMPO\n\r");
         }
         
         else if( (distancia >= 25) && posicion == 1 && bandera == 1 )
         {
            output_low(SLAVE2);
            spi_write(ADELA);
            output_high(SLAVE2);
            delay_us(100);
            printf("\n\rPA DELANTE\n\r");
            
            output_low(SLAVE2);
            vel=249;
            spi_write(vel);
            output_high(SLAVE2);
            delay_us(100);
            printf("\n\rVelocidad: %lu\n\r",vel);
         }
         
         else if( (distancia >= distd+pilon) && posicion == 1 && bandera == 1 )
         {
            output_low(SLAVE2);
            spi_write(ADELA);
            output_high(SLAVE2);
            delay_us(100);
            printf("\n\rPA DELANTE\n\r");
            
            output_low(SLAVE2);
            vel=180;
            spi_write(vel);
            output_high(SLAVE2);
            delay_us(100);
            printf("\n\rVelocidad: %lu\n\r",vel);
         }
         
         else if( (distancia <= distd-pilon)  && posicion == 1 && bandera == 1)
         {
            output_low(SLAVE2);
            spi_write(ATRAS);
            output_high(SLAVE2);
            delay_us(100);
            printf("\n\rPA TRAS\n\r");
            
            output_low(SLAVE2);
            vel=180;
            spi_write(vel);
            output_high(SLAVE2);
            delay_us(100);
            printf("\n\rVelocidad: %lu\n\r",vel);
         }
         
         if( (distancia >= distd-pilon) && (distancia <= distd+pilon)  && posicion == 1 && bandera == 1)
         {
            output_low(SLAVE2);
            spi_write(PAROO);
            output_high(SLAVE2);
            delay_us(100);
            printf("\n\rPAROOO\n\r"); 
         }
         
         else if(distancia > distd+demons && posicion == 1 && bandera == 1)
         {
            output_low(SLAVE2);
            spi_write(FULLL);
            output_high(SLAVE2);
            delay_us(100);
            printf("\n\rAH TODO\n\r");
         }
         /*
         if(posicion == 1 && bandera == 1)
         {
            output_low(SLAVE2);
            vel=veloz;
            spi_write(vel);
            output_high(SLAVE2);
            delay_us(100);
            printf("\n\rVelocidad: %lu\n\r",vel);
         }
         else if(distancia >= distd && posicion == 1 && bandera == 1)
         {
            output_low(SLAVE2);
            vel = distancia * veloz / distd;
            if(vel>=veloz){vel=veloz;}
            else if(vel<=0){vel=0;}
            spi_write(vel);
            output_high(SLAVE2);
            delay_us(100);
            printf("\n\rVelocidad: %lu\n\r",vel);
         }
         
         else if(distancia <= distd && posicion == 1 && bandera == 1)
         {
            output_low(SLAVE2);
            vel = ( abs((distd)) * veloz / distd)+30;
            if(vel>=veloz){vel=veloz;}
            else if(vel<=0){vel=0;}
            spi_write(vel);
            output_high(SLAVE2);
            delay_us(100);
            printf("\n\rVelocidad: %lu\n\r",vel);
         }
         */
      }
      
   }
}

