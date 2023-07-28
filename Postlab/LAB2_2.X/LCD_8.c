
// Define la frecuencia del microcontrolador y función de retardo
#define _XTAL_FREQ 4000000
#define __delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000000.0)))

// Incluir las librerías
#include "LCD_8.h"

void Lcd_Cmd(char a) {
    RS = 0; // => RS = 0
    PORTD = a;
    EN = 1; // => E = 1
    __delay_ms(5);
    EN = 0; // => E = 0
}

void Lcd_Clear(void) {
    Lcd_Cmd(0); // Comando para borrar la memoria interna del LCD
    Lcd_Cmd(1); // Comando para desplazar el cursor a la posición inicial y borrar la pantalla
}

void Lcd_Set(char a, char b) {
    if (a == 1){
        Lcd_Cmd((b & 0x0F)|0x80); // Posición del cursor en la línea 1
    }
	else if (a == 2){
        Lcd_Cmd((b & 0x0F)|0xC0); // Posición del cursor en la línea 2
    }
}

void Lcd_Init(void) {
    PORTD = (0x00); // Establece el puerto de datos en 0x00
    __delay_ms(20);
    // Configura el LCD en modo 8 bits y 2 líneas
    Lcd_Cmd(0x30);      //0011 0000
    __delay_ms(5);
    Lcd_Cmd(0x30);      //0011 0000
    __delay_ms(11);
    Lcd_Cmd(0x30);      //0011 0000
    // Configura el modo de pantalla (2 líneas y matriz de 5x8)
    Lcd_Cmd(0x38);      //0011 1000 
    // Configura el desplazamiento del cursor
    Lcd_Cmd(0x10);      //0000 1000 
    // Borra la pantalla y coloca el cursor en la posición inicial
    Lcd_Cmd(0x01);      //0000 0001 
    // Configura el modo de entrada del LCD
    Lcd_Cmd(0x06);      //0000 0110 
    // Apaga el desplazamiento del cursor
    Lcd_Cmd(0x10);      //0001 0000 
    // Enciende el LCD sin mostrar el cursor ni el parpadeo
    Lcd_Cmd(0x0C);      //0000 1100 
    __delay_ms(100); 
}

void Lcd_Write(char a) {
    RS = 1; // => RS = 1 
    PORTD = a; // Data transfer
    EN = 1; // => E = 1 
    __delay_us(40);
    EN = 0; // => E = 0 
}

void Lcd_Write_Str(char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++) // Recorre la cadena de caracteres hasta el final ('\0')
        Lcd_Write(a[i]); // Escribe cada carácter en el LCD utilizando la función Lcd_Write
}

void Lcd_ShiftR(void) {
    Lcd_Cmd(0x01); // Envía comando para borrar la pantalla
    Lcd_Cmd(0x0C); // Envía comando para activar el desplazamiento del texto hacia la derecha
}

void Lcd_ShiftL(void) {
    Lcd_Cmd(0x01); // Envía comando para borrar la pantalla
    Lcd_Cmd(0x08); // Envía comando para activar el desplazamiento del texto hacia la izquierda
}