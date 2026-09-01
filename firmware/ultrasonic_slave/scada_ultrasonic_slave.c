#include <18F2550.h>
#fuses INTRC,NOLVP,NOPROTECT,NOWDT
#use delay(internal = 4MHz)
#use rs232(baud=9600, xmit=PIN_A2, rcv=PIN_A3)

#BYTE SSPBUF = 0xFC9
signed int16 data, dis, dis2;

// comandos
#define ULTRA  1

#define TRIG_PIN PIN_A1
#define ECHO_PIN PIN_A0

#int_ssp
void spi_rcv()
{  
  data = SSPBUF;
  
  switch(data)
  {
   case ULTRA:   
     SSPBUF = dis2;
     
   break;
  }  
}

void enviarPulso() ;
float32 medirDistancia();

void main()
{
  
  //setup_spi(spi_slave | spi_L_to_H);
  setup_spi(spi_slave | spi_l_to_h | spi_clk_div_4);
  
  enable_interrupts(INT_SSP);
  setup_timer_1(T1_INTERNAL | T1_DIV_BY_1); // Configura Timer1
  enable_interrupts(GLOBAL);
  printf("Iniciando...\n\r");
  output_low(TRIG_PIN); // Inicialmente, el pin TRIG debe estar en bajo
  while(TRUE)
  {
   enviarPulso(); // Genera un pulso de ultrasonido para iniciar la medición
   medirDistancia(); // Mide la distancia
   //dis=distancia;
   //printf("Distancia: %.2f cm\n\r", distancia);
   //printf("Distancia: %i cm\n\r", dis);
   //for(int32 i=0; i<50000; i++){};   // Espera antes de la próxima medición
  } 
}

void enviarPulso() 
{
   output_high(TRIG_PIN);
   delay_us(10); // Genera un pulso de al menos 10 microsegundos
   output_low(TRIG_PIN);
}

float medirDistancia() 
{
   float tiempo_us;
   
   // Espera hasta que el pin ECHO se vuelva alto
   while (!input(ECHO_PIN));
   
   // Inicia el temporizador
   set_timer1(0);
   
   // Espera hasta que el pin ECHO se vuelva bajo
   while (input(ECHO_PIN));
   
   // Lee el valor del temporizador en microsegundos
   tiempo_us = get_timer1();
   
   // Calcula la distancia en centímetros
   dis = (tiempo_us / 58) * 10;
   printf("Distancia entera: %lu cm\n\r", dis);
   dis2 = (tiempo_us / 58) * 10 / 2;
   if(dis2 >= 150)
   {
      dis2=150;
   }
   printf("Distancia xd: %lu cm\n\r", dis2);
   return tiempo_us / 58;///2;
}
