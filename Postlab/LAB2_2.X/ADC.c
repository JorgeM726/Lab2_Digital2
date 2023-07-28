#include <xc.h>
#include <stdint.h>
#include "ADC.h"
#define _XTAL_FREQ 4000000

void adc_init(int channel) {
    // CONFIGURACION ADC
    ADCON0bits.ADCS = 0b01;     // Fosc/8
    ADCON1bits.VCFG0 = 0;       // VDD 
    ADCON1bits.VCFG1 = 0;       // VSS 
    ADCON0bits.CHS = channel;    // Seleccionar el AN12
    
    
    ADCON1bits.ADFM = 0;        // Justificado a la izquierda
    __delay_us(50);             // Sample time
    ADCON0bits.ADON = 1;        // HAbilitar modulo ADC
    PIR1bits.ADIF = 0;          
    PIE1bits.ADIE = 1;
}

int adc_read() {
    return ADRESH;
}

void adc_change_channel(const int channel[], int length) {
    static int temp_channel = 0; // Make temp_channel static to retain its value between function calls

    if (ADCON0bits.GO == 0) {
        temp_channel++;
        if (temp_channel >= length) {
            temp_channel = 0;
        }

        ADCON0bits.CHS = channel[temp_channel];
        __delay_us(100);       // Tiempo de adquisicion
        ADCON0bits.GO = 1;     // Iniciar conversion
    }
}

int adc_get_channel() {
    return  ADCON0bits.CHS;
}