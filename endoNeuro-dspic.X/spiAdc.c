/*
 * File:   spiRpi.c
 * Author: mosee
 *
 * Created on 22 octobre 2019, 14:19
 */


#include <xc.h>
#include <p33FJ128MC802.h>
#include "spiAdc.h"
#include "buffer.h"


void adcSpiWriteRead(uint16_t *data, uint16_t length);

volatile uint16_t adcCurChannel = 0;
volatile uint16_t tmr2Channels[4] = {CHANNEL0, CHANNEL1, CHANNEL2, CHANNEL3};

volatile enum {
    WORD0,
    WORD1
} adcSpiState = WORD0;

volatile uint16_t spi2IsrData = 0;
volatile uint16_t newSampleFlag = 0;


void spiAdcInit() {
    uint16_t data[2];
    
    SPI2STATbits.SPIEN = 0;     // disable the SPI to configure it
    _SPI2IE = 0;                // Disable The Interrupt during configuration
    _SPI2IF = 0;                // Clear the Interrupt Flag

    RPINR22bits.SDI2R = ADC_SDI_PIN;
    ADC_SDO_PIN = 10;
    ADC_SCK_PIN = 11;
    ADC_CE_PIN = 1;
    ADC_CE_PIN_TRIG = 0;

    SPI2CON1bits.DISSCK = 0;    // SCK pin is enabled
    SPI2CON1bits.DISSDO = 0;    // SDO pin is enabled
    SPI2CON1bits.MODE16 = 1;    // 16-bit data exchange
    SPI2CON1bits.SMP = 0;       // input data are sampled at the middle
    SPI2CON1bits.CKE = 0;       // mode 0,0
    SPI2CON1bits.CKP = 0;       // mode 0,0
    
    SPI2CON1bits.PPRE = 1;      // Primary prescaler 16:1
    SPI2CON1bits.SPRE = 2;      // Secondary prescaler 6:1 => 416.67kHz
    
    SPI2CON1bits.MSTEN = 1;     // master mode
    SPI2STATbits.SPIEN = 1;     // enable the SPI

    data[0] = 0x8500;
    data[1] = 0;
    adcSpiWriteRead(data, 2);   // Reset the ADC
    data[0] = 0x0B01;
    data[1] = 0;
    adcSpiWriteRead(data, 2);   // Channel 0 voltage range 0 -> 2.5*Vref ==> 10.24V
    data[0] = 0x0D01;           
    data[1] = 0;
    adcSpiWriteRead(data, 2);   // Channel 1 voltage range 0 -> 2.5*Vref ==> 10.24V
    data[0] = 0x0F01;
    data[1] = 0;
    adcSpiWriteRead(data, 2);   // Channel 2 voltage range 0 -> 2.5*Vref ==> 10.24V
    data[0] = 0x1101;
    data[1] = 0;
    adcSpiWriteRead(data, 2);   // Channel 3 voltage range 0 -> 2.5*Vref ==> 10.24V
    
    _SPI2IE = 1;
}


void __attribute__((__interrupt__,__auto_psv__)) _T2Interrupt() { 
    _LATB8 = 1;

    _T2IF = 0;
    
    ADC_CE_PIN = 0;
    adcSpiState = WORD0;
    SPI2BUF = tmr2Channels[adcCurChannel++];
    if (adcCurChannel > 3) {
        adcCurChannel = 0;
    }
    
    _LATB8 = 0;
}


void __attribute__((__interrupt__,__auto_psv__)) _SPI2Interrupt() {
    _SPI2IF = 0;
    switch (adcSpiState) {
        case WORD0:
            SPI2BUF = 0;
            SPI2BUF;
            adcSpiState = WORD1;
            break;
        case WORD1:
            spi2IsrData = SPI2BUF;
            newSampleFlag = 1;
            ADC_CE_PIN = 1;
            break;
    }
}


void adcSpiWriteRead(uint16_t *data, uint16_t length) {
    uint16_t i;
    ADC_CE_PIN = 0;
    for (i=0; i<length; i++) {
        while(SPI2STATbits.SPITBF == 1);    // Wait for Tx buffer to be empty before writing  in it
        SPI2BUF = data[i];
        while(_SPI2IF == 0);    // Wait for Rx buffer to be filled before reading it
        _SPI2IF = 0;
        data[i] = SPI2BUF;
    }
    ADC_CE_PIN = 1;
}

/*
uint16_t adcRead(uint16_t channel) {
    uint16_t data[2];
    
    data[0] = channel;
    adcSpiWriteRead(data, 2);
    
    return (data[1]);
}*/