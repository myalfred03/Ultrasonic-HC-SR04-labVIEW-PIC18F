#include <18F4550.h>
#DEVICE ADC=10
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,PUT,NOBROWNOUT,NODEBUG,USBDIV,PLL5,CPUDIV1,VREGEN
#use delay(clock=48M)
#DEFINE USB_HID_DEVICE  TRUE
#define USB_EP1_TX_ENABLE  USB_ENABLE_INTERRUPT   //turn on EP1 for IN bulk/interrupt transfers
#define USB_EP1_TX_SIZE    8
#define USB_EP1_RX_ENABLE  USB_ENABLE_INTERRUPT   //turn on EP1 for OUT bulk/interrupt transfers
#define USB_EP1_RX_SIZE    8
#include <pic18_usb.h>//Funciones de bajo nivel(hardware) para la serie PIC 18Fxx5x que serviran en usb.c
#include <usb_desc_hid.h>//Aqui es donde van las descripciones de este dispositivo (como lo reconocera windows)
#include <usb.c>        //libreria para el manejo del usb
#define LEDR PIN_E2  //Led rojo para la espera de la conexion USB
#define LEDV PIN_D7  //Led  verde, se enciende cuando el USB esta conectado
#define LED_ON output_low
#define LED_OFF output_high
                          

int8 datoTX[2];
int16 distancia, tiempo;
#define trig pin_B1
#define echo pin_B0
#USE standard_io(b)

void main()
{
  LED_ON(LEDR);
   LED_OFF(LEDV); 
   usb_init();                      
   usb_task();   //Monitorea el estado de la coneccion conectandose y desconectandose automaticamente
   usb_wait_for_enumeration();  //espera infinitamente hasta que el dispositivo fue enumerado    
   LED_ON(LEDV);
   LED_OFF(LEDR); 
 disable_interrupts(global);                            
setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
enable_interrupts(global);

while(1)
{

    
      usb_task();
      if (usb_enumerated()) 
      {
      
         output_high(trig);                   
         delay_us(10);                       
         output_low(trig);
         while(!input(echo))                     
         {}
         set_timer1(150);                         
         while(input(echo))                     
         {}
         tiempo=get_timer1();
         delay_ms(5);
  //       distancia=(tiempo*10)/(58.0);
 
         datoTX[0]=tiempo >> 8; //Asignacion de desplazamiento hacia la derecha
         datoTX[1]=tiempo & 0xFF; //Asignacion AND de Bits   
      
      }
      
//! 
         
  usb_put_packet(1, datoTX, 2 , USB_DTS_TOGGLE);   // salida de datos hacia LABVIEW, ADC,PUSH,CONTADOR DE PULSOS 
  
  

}
}

