/* 
 * File:   hardConfig.h
 * Author: mosee
 *
 * Created on 14 janvier 2020, 9:20
 */

#ifndef HARDCONFIG_H
#define	HARDCONFIG_H

#define ANALOG_PINS_CFG     0xFFFF

#define MAIN_LOOP_LED       LATAbits.LATA0
#define MAIN_LOOP_LED_TRIS  TRISAbits.TRISA0
#define ACQ_LED             LATAbits.LATA1
#define ACQ_LED_TRIS        TRISAbits.TRISA1

#define RPI_TRIG            LATBbits.LATB11
#define RPI_TRIG_TRIS       TRISBbits.TRISB11
#define RPI_DISABLE         PORTBbits.RB10
#define RPI_DISABLE_TRIS    TRISBbits.TRISB10
#define RPI_DISABLE_PUE     CNPU2bits.CN16PUE

#define RPI_MOSI_PIN    15
#define RPI_MISO_PIN    _RP14R
#define RPI_SCK_PIN     12
#define RPI_CE_PIN      13

#define ADC_SDI_PIN         5
#define ADC_SDO_PIN         _RP2R
#define ADC_SCK_PIN         _RP4R
#define ADC_CE_PIN          _LATB3
#define ADC_CE_PIN_TRIG     _TRISB3



/* Configures PLL prescaler, PLL postscaler, PLL divisor to obtain Fosc = 80MHz
 * with the FRC oscillator (Fin = 7.37MHz). 
 * We obtain Fosc = 7.37MHz*217/(10*2) = 79.96MHz */
void frcPllConfig(void);


#endif	/* HARDCONFIG_H */

