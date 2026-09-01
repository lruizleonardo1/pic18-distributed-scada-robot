#include <18F4550.h>
#fuses INTRC,NOLVP,NOPROTECT,NOWDT
#use delay(internal = 4MHz)

#BYTE SSPBUF = 0xFC9

#BYTE TRISC = 0xF94//ENTRADAS/SALIDAS

#BYTE CCP1CON=0xFBD //REGISTRO DE CONTROL ESTÁNDAR DE CCP1
#BYTE CCP2CON=0xFBA //REGISTRO DE CONTROL ESTÁNDAR DE CCP2
#BYTE CCPR1H=0xFBF
#BYTE CCPR1L=0xFBE 
#BYTE CCPR2H=0xFBC 
#BYTE CCPR2L=0xFBB
#BYTE PR2=0xFCB //El periodo se especifica escribiendo el registro PR2

#BYTE T2CON =0xFCA//permitir el Timer2
#BYTE TMR2=0xFCC//valor del prescaler TMR2

//Periodo _ PWM = [(PR + • 4]1)2 •TOSC • (Valor _ del _ presaler _ de _ TMR )
//PWM = (PR2 +1)*4*Tosc*Prescaler
//1/1KHz = (PR2 +1)*4*(1/4MHz)*4
//PR2=249   PR2=F9

//CCPRXL=PWM/Tosc*Prescaler
//(1/1KHz)/(1/4MHz)*4 =1
//1000->100% 

// comandos
#define MOTOR    2
#define ADELA    254
#define ATRAS    253
#define FULLL    252
#define PAROO    0
#define max 249

int data, vel=249;   //
short time = 0;

#int_ssp
void spi_rcv()
{  
   data = SSPBUF;
   
   if(data == PAROO)
   {
      vel=0;
      output_low(pin_d4);
      output_low(pin_d5);
      output_low(pin_d6);
      output_low(pin_d7);
   }
   if(data == MOTOR)
   {
      time=1;
      vel=0;
      output_high(pin_d0);
      output_low(pin_d4);
      output_low(pin_d5);
      output_low(pin_d6);
      output_low(pin_d7);
      delay_us(100);
      output_low(pin_d0);
      time=0;
   }
   else if( (data != ADELA) && (data != ATRAS) && (data != FULLL) && time != 1)
   {
      
      vel = SSPBUF;
      if(vel >= max)
      {
         vel=max;
      }
   }
   else if(data == FULLL)
   {
      vel = max;
   }
   else if(data == ADELA && time != 1)
   {
      output_high(pin_d4);
      output_high(pin_d5);
      output_low(pin_d6);
      output_low(pin_d7);
   }
   else if(data == ATRAS && time != 1)
   {
      output_high(pin_d6);
      output_high(pin_d7);
      output_low(pin_d4);
      output_low(pin_d5);
   }
}

void main()
{
   setup_spi(spi_slave | spi_L_to_H);
   
   enable_interrupts(INT_SSP);
   enable_interrupts(GLOBAL);
   
   //PR2=0xF9;
   PR2=249;
   TRISC = 0x80;
   
   T2CON = 0x05;//T2CON = 0b00000101 //NO POSTSCALE / SI TIMER2 / PRESCALER DE  4
   
   CCP1CON = 0x0C;//0b00001100;11xx = modo PWM
   CCP2CON = 0x0C;//11xx = modo PWM
   
   TMR2 =4;//PRESCALER DE 4
   
   while(TRUE)
   {
      CCPR1L=vel;
      CCPR2L=vel;
   } 
}

