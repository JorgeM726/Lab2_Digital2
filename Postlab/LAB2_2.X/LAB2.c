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
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits Write protection off)

#include <xc.h>
#include <stdint.h>
#include "LCD_8.h"
#include "ADC.h"
#include "USART.h"
#define _XTAL_FREQ 4000000
// Definición de pines para la interfaz con la pantalla LCD
#define RS RC4
#define EN RC5
#define D0 RD0
#define D1 RD1
#define D2 RD2
#define D3 RD3
#define D4 RD4
#define D5 RB5
#define D6 RD6
#define D7 RD7
unsigned int numero = 0;
unsigned int numero2 = 0;
uint8_t cont = 0;
void setup(void);
float volt = 0;
void reverse(char str[], int length);
int intToStr(int x, char str[], int d);
int uint8ToStr(uint8_t x, char str[]);
void floatToStr(float value, char* buffer, int precision);
char buffer[20];
char buffer2[20];
int channels[] = {0,1};

unsigned int x;
void __interrupt() isr (void){
   
    
    
    if(PIR1bits.ADIF){              // Revisar interrupción de ADC
        if(adc_get_channel() == 0){    // Revisar canal 12
            numero = adc_read();          // Mostrar conversión en PORTA
            volt = (float)numero*1.961/100;
            
        }
        
        else 
            numero2 = adc_read();            //Enviar número a display si es en el canal 10
            PIR1bits.ADIF = 0;          // limpiar interrupción
    }
    if (RCIF){
        PORTD = 0;
        if (RCREG == '+'){
            cont++; // Incrementar el valor del contador al recibir el carácter '+'
        }
        else if (RCREG == '-'){
            cont--; // Decrementar el valor del contador al recibir el carácter '-'
        }
        else if (RCREG == '1'){
            x = 1; // Activar una bandera para mostrar el valor del pot por USART
        }
        
        RCIF = 0; // Limpiar la bandera de interrupción del USART
    }
}

void main(void) {
    setup();
    __delay_us(50);
    ADCON0bits.GO = 1;
     USART_print(" "); //IMPORTANTEEEEEEEEE, SI USAS ESTO EN OTROS LABS, COPIA ESTE ESPACIO PORQUE SI NO, NO SE PUEDE MANDAR DATOS DESDE CONSOLA POR ALGUNA RAZÓN

    while(1){
        
       adc_change_channel(channels,2);
       
       
       floatToStr(volt, buffer, 2);
       Lcd_Set(2,1);
       Lcd_Write_Str(buffer);
       
       
       
       uint8ToStr(cont,buffer2);
       Lcd_Set(2,10);
       
       Lcd_Write_Str(buffer2);
       if (x==1){
           x=0;
           USART_print(buffer);
       }
       
    }
    return;
}

void setup(void){
    USART_init_baud(9600);
    ANSEL = 0b00000011; 
    ANSELH = 0; 
    TRISE = 0;
    TRISD = 0x00;
    PORTD = 0;
    TRISA = 0XFF;
    PORTA = 0;
     TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 0;
    PORTC = 0;
    PORTE = 0b1111;
    
    adc_init(0);
    Lcd_Init();
    Lcd_Clear();
    Lcd_Set(1,1);
    Lcd_Write_Str("POT:    CONT:");
    
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD3 = 0; 
    
     // CONFIGURACION DEL OSCILADOR
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0; // 4MHZ
    OSCCONbits.SCS = 1;  // OSCILADOR INTERNO 
    
    
                      
     
    
    // CONFIGURACION INTERRUPCIONES
   
    
    INTCONbits.PEIE = 1;        // Int. de perifericos
    INTCONbits.GIE = 1;         // Int. globales
    
}
void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

int intToStr(int x, char str[], int d) {
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }
    while (i < d) {
        str[i++] = '0';
    }
    reverse(str, i);
    str[i] = '\0';
    return i;
}

int uint8ToStr(uint8_t x, char str[]) {
    int i = 0;
    do {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    } while (x > 0);
    
    str[i] = '\0';
    reverse(str, i);
    
    return i;
}

void floatToStr(float value, char* buffer, int precision) {
    // Handle negative numbers
    if (value < 0) {
        value = -value;
        *buffer++ = '-';
    }

    // Extract integer part
    int integerPart = (int)value;

    // Extract floating part and round it
    float floatingPart = value - integerPart;
    for (int i = 0; i < precision; i++) {
        floatingPart *= 10;
    }
    int roundedFloatingPart = (int)(floatingPart + 0.5);

    // Convert integer part to string
    int integerLength = intToStr(integerPart, buffer, 0);
    buffer += integerLength;

    // Add decimal point if needed
    if (precision > 0) {
        *buffer++ = '.';
    }

    // Convert floating part to string
    int floatingLength = intToStr(roundedFloatingPart, buffer, precision);
    buffer += floatingLength;

    // Null-terminate the string
    *buffer = '\0';
}