/*
 * File:   spiRpi.c
 * Author: mosee
 *
 * Created on 22 octobre 2019, 14:19
 */


#include <xc.h>
#include "spiRpi.h"
#include "buffer.h"
#include "hardConfig.h"

//volatile uint16_t*  rpi_paramPtr[] = {&packetSize, &samplingPeriod, &bufCurSize, &bufferMax};
rpiCommand_t        rpi_command;
uint16_t            rpi_param;

#define RPI_RX_BUFFER_LENGTH    15
volatile uint8_t    rpi_rxBuffer[RPI_RX_BUFFER_LENGTH];
volatile int16_t    rpi_RxDataNb = 0;



#define SOF 0x55
#define EOF 0xAA

void __attribute__((__interrupt__,__auto_psv__)) _SPI1Interrupt() {
    uint8_t data;
    
    IFS0bits.SPI1IF = 0;
    
    if (SPI1STATbits.SPIRBF == 1) {
        if (rpi_RxDataNb < RPI_RX_BUFFER_LENGTH) {
            rpi_rxBuffer[rpi_RxDataNb++] = SPI1BUF;
        } else {
            SPI1BUF;    // dummy read to avoid overrun
        }
        if ( bufPopData(&data) == BUFFER_EMPTY ) {
            SPI1BUF = 0;
        }
        else {
            SPI1BUF = data;
        }
    }
}


uint16_t commandAvailable(void)  {
    int16_t i = 0;
    int16_t err;
    
    err = (i > rpi_RxDataNb-5);
    while ( (rpi_rxBuffer[i] != SOF) && (err == 0) ) {
        i++;

        if ( i > rpi_RxDataNb-5) {
            err = 1;
        }
    }
    if ( (err==0) && (rpi_rxBuffer[i+4] == EOF) ) {
        rpi_command.cmd = rpi_rxBuffer[i+1];
        rpi_command.param = rpi_rxBuffer[i+2] + 256*rpi_rxBuffer[i+3];
        rpi_RxDataNb = 0;
        return (1);
    }
    else {
        return (0);
    }
}

rpiCommand_t getCommand(void) {
    return (rpi_command);
}


void spiRpiInit() {
    rpi_command.cmd = 0;
    rpi_command.param = 0;


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
//    SPI1CON1bits.SMP = 1;
    SPI1BUF = 0;
    
    SPI1STATbits.SPIEN = 1;     // enable the SPI
    IEC0bits.SPI1IE = 1;        // enable interrupt
}