/*
 * File:   spiRpi.c
 * Author: mosee
 *
 * Created on 22 octobre 2019, 14:19
 */


#include <xc.h>
#include "spiRpi.h"
#include "hardConfig.h"


volatile uint8_t buffer[BUFFER_NUMBER][BUFFER_SIZE];
volatile uint16_t bufToSend;


void __attribute__((__interrupt__,__no_auto_psv__)) _SPI1Interrupt() {
    static uint16_t i = 0;
    static uint16_t curPacket = BUFFER_NUMBER;
    
    IFS0bits.SPI1IF = 0;
    
    //if (SPI1STATbits.SPIRBF == 1) {
        if (SPI1BUF == 0x55) {
            if (--bufToSend == 0) {
                RPI_TRIG = 0;
            }
            if (++curPacket >= BUFFER_NUMBER) {
                curPacket = 0;
            }
            i = 0;
        }
        if (i < BUFFER_SIZE) {
            SPI1BUF = buffer[curPacket][i++];
        } else {
            SPI1BUF = 0x33;
        }
    //}
}


void spiRpiInit() {
    SPI1STATbits.SPIEN = 0;     // disable the SPI to configure it
    IEC0bits.SPI1IE = 0;        // Disable The Interrupt during configuration
    IFS0bits.SPI1IF = 0;        // Clear the Interrupt Flag

    RPINR20bits.SDI1R = RPI_MOSI_PIN;
    RPI_MISO_PIN = 7;
    RPINR20bits.SCK1R = RPI_SCK_PIN;
    RPINR21bits.SS1R = RPI_CE_PIN;
    

    SPI1CON1bits.MODE16 = 0;    // 16 bits data exchange
    SPI1CON1bits.SSEN = 1;      // SS pin is used as slave
    SPI1CON1bits.MSTEN = 0;     // slave mode
    SPI1BUF = 0;
    
    SPI1STATbits.SPIEN = 1;     // enable the SPI
    IEC0bits.SPI1IE = 1;        // enable interrupt
}